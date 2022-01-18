#include "UnoButton.h"
#include "Game.h"

UnoButton::UnoButton(const sf::Vector2f position, const sf::Font& font, const GameStateData& gameData)
	: OvalHoverButton(sf::IntRect(position.x, position.y, 80, 60), font, "UNO", sf::Color(147, 44, 44)), _gameState(gameData)
{
	setEnabled(true);
	_bottomPlayer = _gameState.bottomPlayer;
}

void UnoButton::update(const float deltaTime)
{
	_isActive = _bottomPlayer->getUnoState() == Player::UNOState::NotSafe
		|| (_bottomPlayer->getUnoState() == Player::UNOState::Safe
			&& _gameState.players->at(*_gameState.currentPlayerID) == _bottomPlayer
			&& _bottomPlayer->getHand().size() == 2);
}

void UnoButton::showOverlay()
{
	setEnabled(true);
}

void UnoButton::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (_isActive && _bounds.contains(mousePosition)) {
		_bottomPlayer->setUnoState(Player::UNOState::Called);
		Game::getCurrentGame()->showGeneralOverlay("UNOCalled" + _bottomPlayer->getPlayerID());
	}
}