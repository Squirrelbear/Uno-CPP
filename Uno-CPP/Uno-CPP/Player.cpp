#include "Player.h"

Player::Player(const int playerID, const std::string & playerName, const PlayerType playerType, const sf::IntRect bounds, const bool showPlayerNameLeft)
	: _playerID(playerID), _playerName(playerName), _playerType(playerType), _bounds(bounds), _showPlayerNameLeft(showPlayerNameLeft)
{
	_showCards = playerType == PlayerType::ThisPlayer;
	resetScore();
}

Player::~Player()
{
	emptyHand();
}

void Player::draw(sf::RenderWindow & renderWindow) const
{
	// TODO
	/* 
	        if(showCards) {
            hand.forEach(card -> card.paint(g));
        } else {
            hand.forEach(card -> Card.paintCardBack(g, card));
        }
        g.setFont(new Font("Arial", Font.BOLD, 20));
        int strWidth = g.getFontMetrics().stringWidth(playerName);
        g.setColor(new Color(1,1,1, 204));
        int nameXOffset = bounds.position.x + (showPlayerNameLeft ? -(strWidth-50) : (bounds.width/2-(strWidth+30)/2));
        int nameYOffset = bounds.position.y + (showPlayerNameLeft ? (bounds.height/2-20) : -10);
        g.fillRect(nameXOffset, nameYOffset, strWidth+30, 40);
        g.setColor(CurrentGameInterface.getCurrentGame().getCurrentPlayer().getPlayerID() == getPlayerID()
                ? Color.ORANGE : Color.WHITE);
        g.drawString(playerName, nameXOffset+15, nameYOffset+25);
	*/
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
	// TODO
	/*
	        Comparator<Card> compareByCard = Comparator
                .comparing(Card::getColourID)
                .thenComparing(Card::getFaceValueID);
        hand.sort(compareByCard);
        recalculateCardPositions();
	*/
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

void Player::removeCard(Card * card)
{
	auto newEndIt = std::remove(_hand.begin(), _hand.end(), card);
	recalculateCardPositions();
}

Card * Player::getCardByID(const int cardID)
{
	for (auto card : _hand) {
		if (card->getUniqueCardID() == cardID) {
			return card;
		}
	}
	return nullptr;
}

Card * Player::chooseCardFromClick(const sf::Vector2i & mousePosition)
{
	updateHover(mousePosition);
	return _hoveredCard;
}

std::vector<Card*> Player::getHand() const
{
	return _hand;
}

std::string Player::getPlayerName() const
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
