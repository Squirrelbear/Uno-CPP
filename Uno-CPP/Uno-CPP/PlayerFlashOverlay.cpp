#include "PlayerFlashOverlay.h"

PlayerFlashOverlay::PlayerFlashOverlay(sf::Vector2f position, const std::string & message, const sf::Color & colour, const int fontSize, const sf::Font& font)
	: WndInterface(sf::IntRect(position.x, position.y, 40, 40)), _position(position),
	_flashingText(message, font, fontSize), _shadowText(message, font, fontSize)
{
	setEnabled(false);
	_flashingText.setFillColor(colour);
	_shadowText.setFillColor(sf::Color::Black);
	setMessage(message);
}

PlayerFlashOverlay::~PlayerFlashOverlay()
{
}

void PlayerFlashOverlay::update(const float deltaTime)
{
	_displayTimer -= deltaTime;
	if (_displayTimer <= 0) {
		setEnabled(false);
	}
}

void PlayerFlashOverlay::draw(sf::RenderWindow & renderWindow) const
{
	if (fmod(_displayTimer, 0.2f) < 0.15f) {
		renderWindow.draw(_shadowText);
		renderWindow.draw(_flashingText);
	}
}

void PlayerFlashOverlay::showOverlay()
{
	setEnabled(true);
	_displayTimer = 2.0f;
}

void PlayerFlashOverlay::setMessage(const std::string & message)
{
	_flashingText.setString(message);
	_shadowText.setString(message);
	int widthOffset = _flashingText.getGlobalBounds().width / 2;
	_flashingText.setPosition(sf::Vector2f(_position.x - widthOffset, _position.y));
	_shadowText.setPosition(sf::Vector2f(_position.x - widthOffset-2, _position.y-2));
}
