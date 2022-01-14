#include "LobbyPlayer.h"
#include <iostream>
#include "StringUtility.h"

LobbyPlayer::LobbyPlayer(const std::string & playerName, const Player::PlayerType playerType, const sf::IntRect & bounds, const sf::Font& font)
	: InteractableRect(bounds), _playerName(playerName), _playerType(playerType), 
	_bgHoverColour(sf::Color(115, 156, 58, 204)), _bgNotHoveredColour(118, 94, 57, 204)
{
	_aiStrategy = AIPlayer::AIStrategy::Random;
	_strategyStr = "Strategy: " + AIPlayer::aiStrategyToString(_aiStrategy);
	_isEnabled = true;
	_playerTypeStr = (playerType == Player::PlayerType::ThisPlayer) ? "You:" : "AI Player:";

	_drawableObject = new DrawableObjectGroup();
	_background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width-2, bounds.height-2)), _bgNotHoveredColour, sf::Vector2f(1, 1));
	_background->setBorder(1, sf::Color::Black);
	_playerNameText = new DrawableText(sf::Vector2f(0, 0), playerName, font, 20, sf::Color::Black, sf::Text::Bold);
	_playerNameText->setOffset(sf::Vector2f(120, 30));
	DrawableText* playerTypeText = new DrawableText(sf::Vector2f(0, 0), _playerTypeStr, font, 20, sf::Color::Black, sf::Text::Bold);
	playerTypeText->setOffset(sf::Vector2f(20, 30));
	_drawableObject->addChild(_background);
	_drawableObject->addChild(_playerNameText);
	_drawableObject->addChild(playerTypeText);

	if (playerType == Player::PlayerType::AIPlayer) {
		_aiStrategyText = new DrawableText(sf::Vector2f(0, 0), _strategyStr, font, 20, sf::Color::Black, sf::Text::Bold);
		_aiStrategyText->setOffset(sf::Vector2f(300, 30));
		DrawableText* actionText = new DrawableText(sf::Vector2f(0, 0), "(Click to cycle strategies)", font, 20, sf::Color::Black, sf::Text::Bold);
		actionText->setOffset(sf::Vector2f(300, 55));
		_drawableObject->addChild(_aiStrategyText);
		_drawableObject->addChild(actionText);
	}
	else {
		DrawableText* actionText = new DrawableText(sf::Vector2f(0, 0), "(Click to change your name)", font, 20, sf::Color::Black, sf::Text::Bold);
		actionText->setOffset(sf::Vector2f(300, 30));
		_drawableObject->addChild(actionText);
	}

	_drawableObject->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));
}

LobbyPlayer::~LobbyPlayer()
{
	if (_drawableObject != nullptr) {
		// All other objects are managed and deleted by this call.
		delete _drawableObject;
		// Cleanup so they don't point to missing objects.
		_background = nullptr;
		_playerNameText = nullptr;
		_aiStrategyText = nullptr;
	}
}

void LobbyPlayer::setPlayerName(const std::string & playerName)
{
	_playerName = playerName;
}

std::string LobbyPlayer::getPlayerName() const
{
	return _playerName;
}

Player::PlayerType LobbyPlayer::getPlayerType() const
{
	return _playerType;
}

void LobbyPlayer::setEnabled(const bool isEnabled)
{
	_isEnabled = isEnabled;
}

bool LobbyPlayer::isEnabled() const
{
	return _isEnabled;
}

AIPlayer::AIStrategy LobbyPlayer::getAIStrategy() const
{
	return _aiStrategy;
}

void LobbyPlayer::handleClick()
{
	if (_playerType == Player::PlayerType::AIPlayer) {
		iterateStrategy();
	}
	else {
		chooseNewName();
	}
}

void LobbyPlayer::draw(sf::RenderWindow & renderWindow) const
{
	_drawableObject->draw(renderWindow);
}

void LobbyPlayer::updateHoverState(const sf::Vector2i & mousePosition)
{
	bool currentValue = _isHovered;
	_isHovered = isPositionInside(mousePosition);
	// If the state changed
	if (currentValue != _isHovered) {
		_background->setColour(_isHovered ? _bgHoverColour :_bgNotHoveredColour);
	}
}

void LobbyPlayer::chooseNewName()
{
	std::cout << "Enter name name: ";
	std::string input;
	std::getline(std::cin, input);
	trim(input);
	if (input.length() > 12) {
		input = input.substr(0, 12);
	}
	if (input.length() > 0) {
		_playerName = input;
		_playerNameText->setText(_playerName);
	}
}

void LobbyPlayer::iterateStrategy()
{
	switch (_aiStrategy) {
	case AIPlayer::AIStrategy::Random: _aiStrategy = AIPlayer::AIStrategy::Offensive; break;
		case AIPlayer::AIStrategy::Offensive: _aiStrategy = AIPlayer::AIStrategy::Defensive; break;
		case AIPlayer::AIStrategy::Defensive: _aiStrategy = AIPlayer::AIStrategy::Chaotic; break;
		case AIPlayer::AIStrategy::Chaotic: _aiStrategy = AIPlayer::AIStrategy::Random; break;
	}
	_strategyStr = "Strategy: " + AIPlayer::aiStrategyToString(_aiStrategy);
	_aiStrategyText->setText(_strategyStr);
}
