#include "AntiUnoButton.h"
#include "Game.h"

AntiUnoButton::AntiUnoButton(const sf::Vector2f position, const sf::Font & font)
	: OvalHoverButton(sf::IntRect(position.x, position.y, 80, 60), font, "!", sf::Color(147, 44, 44))
{
	setEnabled(true);
	_bottomPlayer = Game::getCurrentGame()->getBottomPlayer();
}

AntiUnoButton::~AntiUnoButton()
{
}

void AntiUnoButton::update(const float deltaTime)
{
	_isActive = false;
	for (const auto& player : Game::getCurrentGame()->getAllPlayers()) {
		if (player != _bottomPlayer && !player->isSafe() && player->getHand().size() == 1) {
			_isActive = true;
		}
	}
}

void AntiUnoButton::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (_isActive && _bounds.contains(mousePosition)) {
		for (const auto& player : Game::getCurrentGame()->getAllPlayers()) {
			if (player != _bottomPlayer && !player->isSafe() && player->getHand().size() == 1) {
				Game::getCurrentGame()->applyAntiUno(player->getPlayerID());
				break;
			}
		}
	}
}

void AntiUnoButton::showOverlay()
{
	setEnabled(true);
}
