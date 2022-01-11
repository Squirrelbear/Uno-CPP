#include "LobbyPlayer.h"

LobbyPlayer::LobbyPlayer(const std::string & playerName, const Player::PlayerType playerType, const sf::IntRect & bounds)
	: InteractableRect(bounds), _playerName(playerName), _playerType(playerType)
{
	_aiStrategy = AIPlayer::AIStrategy::Random;
	_strategyStr = "Strategy: " + AIPlayer::aiStrategyToString(_aiStrategy);
	_isEnabled = true;
	_playerTypeStr = (playerType == Player::PlayerType::ThisPlayer) ? "You:" : "AI Player:";
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
	// TODO
}

void LobbyPlayer::updateHoverState(const sf::Vector2i & mousePosition)
{
	_isHovered = isPositionInside(mousePosition);
}

void LobbyPlayer::chooseNewName()
{
	// TODO
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
}
