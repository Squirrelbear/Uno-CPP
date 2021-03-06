#include "AntiUnoButton.h"

AntiUnoButton::AntiUnoButton(const sf::Vector2f position, const sf::Font & font, const GameStateData& gameData)
	: OvalHoverButton(sf::IntRect(position.x, position.y, 80, 60), font, "!", sf::Color(147, 44, 44)), _gameState(gameData)
{
	setEnabled(true);
	_bottomPlayer = _gameState.bottomPlayer;
}

AntiUnoButton::~AntiUnoButton()
{
}

void AntiUnoButton::update(const float deltaTime)
{
	_isActive = false;
	for (const auto& player : *_gameState.players) {
		if (player != _bottomPlayer && !player->isSafe() && player->getHand().size() == 1) {
			_isActive = true;
		}
	}
}

void AntiUnoButton::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (_isActive && _bounds.contains(mousePosition)) {
		for (const auto& player : *_gameState.players) {
			if (player != _bottomPlayer && !player->isSafe() && player->getHand().size() == 1) {
				_isTriggered = true;
				break;
			}
		}
	}
}

void AntiUnoButton::showOverlay()
{
	setEnabled(true);
}
