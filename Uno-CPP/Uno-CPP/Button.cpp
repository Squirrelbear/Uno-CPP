#include "Button.h"

Button::Button(const sf::IntRect & bounds, const std::string & text, const int actionID, const sf::Font& font)
	: InteractableRect(bounds), _text(text), _actionID(actionID)
{
	_isHovered = false;

	initialiseDefaultView(font);
	initialiseHoveredView(font);
}

void Button::draw(sf::RenderWindow & renderWindow) const
{
	if (_isHovered) {
		_hoveredView.draw(renderWindow);
	}
	else {
		_defaultView.draw(renderWindow);
	}
}

int Button::getActionID() const
{
	return _actionID;
}

void Button::setHovering(const bool isHovering)
{
	_isHovered = isHovering;
}

void Button::initialiseDefaultView(const sf::Font& font)
{
	DrawableShape* border = new DrawableShape(new sf::RectangleShape(sf::Vector2f(_bounds.width, _bounds.height)), sf::Color::Black);
	_defaultView.addChild(border);
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(_bounds.width-2, _bounds.height-2)), sf::Color(123, 133, 163), sf::Vector2f(1,1));
	_defaultView.addChild(background);
	DrawableText* text = new DrawableText(sf::Vector2f(0, 0), _text, font, 20, sf::Color::Black, sf::Text::Bold);
	text->setOffset(sf::Vector2f(_bounds.width / 2 - text->getTextWidth() / 2, _bounds.height / 2 - 20 + 8));
	_defaultView.addChild(text);
	_defaultView.setPositionWithOffset(sf::Vector2f(_bounds.left, _bounds.top));
}

void Button::initialiseHoveredView(const sf::Font & font)
{
	DrawableShape* border = new DrawableShape(new sf::RectangleShape(sf::Vector2f(_bounds.width+6, _bounds.height+6)), sf::Color::Black, sf::Vector2f(-3,-3));
	_hoveredView.addChild(border);
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(_bounds.width - 4, _bounds.height - 4)), sf::Color(123, 133, 163), sf::Vector2f(2, 2));
	_hoveredView.addChild(background);
	DrawableText* text = new DrawableText(sf::Vector2f(0, 0), _text, font, 20, sf::Color::Black, sf::Text::Bold);
	text->setOffset(sf::Vector2f(_bounds.width / 2 - text->getTextWidth() / 2, _bounds.height / 2 - 20 + 8));
	_hoveredView.addChild(text);
	_hoveredView.setPositionWithOffset(sf::Vector2f(_bounds.left, _bounds.top));
}
