#include "Player.h"
#include "DrawableShape.h"
#include <vector>
#include <algorithm>

Player::Player(const int playerID, const std::string & playerName, const PlayerType playerType, const sf::IntRect bounds, const bool showPlayerNameLeft, const sf::Font& font)
	: _playerID(playerID), _playerName(playerName), _playerType(playerType), _bounds(bounds), _showPlayerNameLeft(showPlayerNameLeft)
{
	_showCards = playerType == PlayerType::ThisPlayer;
	resetScore();

	_playerNameText = new DrawableText(playerName, font, 20, sf::Color(255, 215, 0), sf::Text::Bold);
	int strWidth = _playerNameText->getTextWidth();
	int nameXOffset = bounds.left + (_showPlayerNameLeft ? -(strWidth - 50) : (bounds.width / 2 - (strWidth + 30) / 2));
	int nameYOffset = bounds.top + (_showPlayerNameLeft ? (bounds.height / 2 - 20) : -10);
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(strWidth + 30, 40)), sf::Color(1, 1, 1, 204), sf::Vector2f(nameXOffset, nameYOffset));
	_playerNameText->setOffset(sf::Vector2f(nameXOffset + 15, nameYOffset + 5));
	
	_nameTag = new DrawableObjectGroup();
	_nameTag->addChild(background);
	_nameTag->addChild(_playerNameText);
	_nameTag->setPositionWithOffset(sf::Vector2f(0, 0));
}

Player::~Player()
{
	emptyHand();
	delete _nameTag;
}

void Player::draw(sf::RenderWindow & renderWindow, const Player* currentPlayer) const
{
	for (const auto& card : _hand) {
		if (_showCards) {
			card->drawCardFront(renderWindow);
		}
		else {
			card->drawCardBack(renderWindow);
		}
	}

	_playerNameText->setColour((currentPlayer == this) ? sf::Color(255, 215, 0) : sf::Color::White);
	_nameTag->draw(renderWindow);
}

void Player::addCardToHand(Card * card)
{
	_hand.emplace_back(card);
	recalculateCardPositions();
}

void Player::emptyHand()
{
	for (auto p : _hand)
		delete p;
	_hand.clear();
}

void Player::revealHand(const bool reveal)
{
	_showCards = reveal;
}

Player::PlayerType Player::getPlayerType() const
{
	return _playerType;
}

int Player::getPlayerID() const
{
	return _playerID;
}

std::vector<Card*> Player::getValidMoves(const int curFaceValue, const int curColourValue) const
{
	std::vector<Card*> result;

	for (auto card : _hand) {
		if (card->getFaceValueID() == curFaceValue || card->getColourID() == curColourValue
			|| card->getFaceValueID() == 13 || card->getFaceValueID() == 14) {
			result.emplace_back(card);
		}
	}

	return result;
}

void Player::sortHand()
{
	// Sort first by colour and then by faceValue
	std::sort(_hand.begin(), _hand.end(), [](Card* a, Card* b) { 
		if(a->getColourID() != b->getColourID()) return a->getColourID() < b->getColourID();
		else return a->getFaceValueID() < b->getFaceValueID();
		});
	recalculateCardPositions();
}

void Player::updateHover(const sf::Vector2i & mousePosition)
{
	if (_hoveredCard != nullptr && !_hoveredCard->isPositionInside(mousePosition)) {
		_hoveredCard = nullptr;
	}
	for (auto card : _hand) {
		if (card->isPositionInside(mousePosition)) {
			_hoveredCard = card;
			break;
		}
	}
	recalculateCardPositions();
}

Card* Player::removeCard(const int uniqueCardID)
{
	Card* removedCard = nullptr;
	_hand.erase(std::remove_if(_hand.begin(), _hand.end(),
		[&](Card* c) {
			if (c->getUniqueCardID() == uniqueCardID && removedCard == nullptr) {
				removedCard = c;
				return true;
			}
			else {
				return false;
			}
		}), _hand.end());
	recalculateCardPositions();
	return removedCard;
}

const Card * Player::getCardByID(const int cardID)
{
	for (auto card : _hand) {
		if (card->getUniqueCardID() == cardID) {
			return card;
		}
	}
	return nullptr;
}

const Card * Player::chooseCardFromClick(const sf::Vector2i & mousePosition)
{
	updateHover(mousePosition);
	return _hoveredCard;
}

const std::vector<Card*>& Player::getHand() const
{
	return _hand;
}

std::vector<Card*> Player::takeAllHand()
{
	std::vector<Card*> result;
	for (auto card : _hand) {
		result.emplace_back(card);
	}
	_hand.empty();

	return result;
}

const std::string& Player::getPlayerName() const
{
	return _playerName;
}

int Player::getHandTotalScore() const
{
	int score = 0;
	for (auto card : _hand) {
		score += card->getScoreValue();
	}
	return score;
}

sf::Vector2f Player::getCentreOfBounds() const
{
	return sf::Vector2f(_bounds.left+_bounds.width/2, _bounds.top + _bounds.height/2);
}

void Player::setCurrentRoundScore(const int newCurrentRoundScore)
{
	_currentRoundScore = newCurrentRoundScore;
	_totalScore += _currentRoundScore;
}

void Player::setWon()
{
	_wonRound = true;
}

bool Player::getWon() const
{
	return _wonRound;
}

int Player::getTotalScore() const
{
	return _totalScore;
}

int Player::getCurrentRoundScore() const
{
	return _currentRoundScore;
}

void Player::resetScore()
{
	_totalScore = _currentRoundScore = 0;
	_wonRound = false;
	_unoState = UNOState::Safe;
}

void Player::setUnoState(const UNOState unoState)
{
	if (_unoState == UNOState::Called && unoState == UNOState::NotSafe) {
		return;
	}
	_unoState = unoState;
}

bool Player::isSafe() const
{
	return _unoState != UNOState::NotSafe;
}

Player::UNOState Player::getUnoState() const
{
	return _unoState;
}

void Player::recalculateCardPositions()
{
	int paddingX = -15;
	int paddingY = (_playerType == PlayerType::ThisPlayer) ? 10 : -CARD_HEIGHT / 2 - 10;
	int elementsPerRow = (_bounds.width + paddingX) / CARD_WIDTH;
	int rows = static_cast<int>(ceil(_hand.size() / static_cast<double>(elementsPerRow)));
	int startY = _bounds.top + _bounds.height / 2 - rows * (CARD_HEIGHT + paddingY) / 2;
	int x = 0;
	int y = 0;
	int remainingElements = _hand.size();
	int rowXOffset = _bounds.width / 2 - static_cast<int>(elementsPerRow*(CARD_WIDTH + paddingX) / 2.0);

	// True when there is only one not-full row (used to centre in that row).
	if (remainingElements < elementsPerRow) {
		rowXOffset = _bounds.width / 2 - static_cast<int>(remainingElements*(CARD_WIDTH + paddingX) / 2.0);
	}
	for (auto card : _hand) {
		// Apply a visual offset to the hovered card
		int hoverOffset = (card == _hoveredCard) ? -10 : 0;
		card->setPosition(sf::Vector2i(_bounds.left + rowXOffset + x * (CARD_WIDTH + paddingX), 
							startY + y * (CARD_HEIGHT + paddingY) + hoverOffset));
		x++;
		remainingElements--;
		// Check for iterating to the next row.
		if (x >= elementsPerRow) {
			x = 0;
			y++;
			rowXOffset = _bounds.width / 2 - static_cast<int>(elementsPerRow*(CARD_WIDTH + paddingX) / 2.0);
			// Once a not full row has been found (used to centre in that row).
			if (remainingElements < elementsPerRow) {
				rowXOffset = _bounds.width / 2 - static_cast<int>(remainingElements*(CARD_WIDTH + paddingX) / 2.0);
			}
		}
	}

}
