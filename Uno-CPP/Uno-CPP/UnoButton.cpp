#include "UnoButton.h"
#include "Game.h"
#include "EllipseShape.h"

UnoButton::UnoButton(const sf::Vector2f position, const sf::Font& font)
	: WndInterface(sf::IntRect(position.x, position.y, 80, 60)), _bgColour(sf::Color(147, 44, 44))
{
	_isHovered = false;
	setEnabled(true);
	_bottomPlayer = Game::getCurrentGame()->getBottomPlayer();
	_isActive = true;

	DrawableText* _foreground = new DrawableText(sf::Vector2f(0, 0), "UNO", font, 30, sf::Color(226, 173, 67), sf::Text::Bold);
	int strWidth = _foreground->getTextWidth();
	_foreground->setPositionWithOffset(sf::Vector2f(position.x + _bounds.width / 2 - strWidth / 2, position.y + _bounds.height / 2 - 20));
	DrawableText* _shadow = new DrawableText(sf::Vector2f(0, 0), "UNO", font, 30, sf::Color::Black, sf::Text::Bold);
	_shadow->setPositionWithOffset(sf::Vector2f(position.x + _bounds.width / 2 - strWidth / 2 - 2, position.y + _bounds.height / 2 - 20 + 2));
	_text = new DrawableObjectGroup();
	_text->addChild(_shadow);
	_text->addChild(_foreground);

	_hoverShape = new DrawableShape(new EllipseShape(sf::Vector2f(_bounds.width/2, _bounds.height/2)), _bgColour);
	_hoverShape->setPositionWithOffset(position);
}

UnoButton::~UnoButton()
{
	delete _hoverShape;
	delete _text;
}

void UnoButton::update(const float deltaTime)
{
	_isActive = _bottomPlayer->getUnoState() == Player::UNOState::NotSafe
		|| (_bottomPlayer->getUnoState() == Player::UNOState::Safe
			&& Game::getCurrentGame()->getCurrentPlayer() == _bottomPlayer
			&& _bottomPlayer->getHand().size() == 2);
}

void UnoButton::draw(sf::RenderWindow & renderWindow) const
{
	if (!_isActive) return;

	_hoverShape->draw(renderWindow);
	_text->draw(renderWindow);
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

void UnoButton::handleMouseMove(const sf::Vector2i & mousePosition)
{
	bool currentValue = _isHovered;
	_isHovered = _bounds.contains(mousePosition);
	// If the state changed
	if (currentValue != _isHovered) {
		_hoverShape->setBorder(_isHovered ? 10.0f : 0.0f, _bgColour);
	}
}
