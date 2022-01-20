#include "DrawableText.h"

DrawableText::DrawableText(const std::string & str, const sf::Font & font, const size_t fontSize, const sf::Color textColour, const sf::Uint32 style = sf::Text::Regular)
{
	_textObject.setFont(font);
	_textObject.setString(str);
	_textObject.setCharacterSize(fontSize);
	_textObject.setFillColor(textColour);
	_textObject.setStyle(style);
}

void DrawableText::draw(sf::RenderWindow & renderWindow) const
{
	renderWindow.draw(_textObject);
}

void DrawableText::setPositionWithOffset(const sf::Vector2f & position)
{
	_textObject.setPosition(sf::Vector2f(position.x + _offset.x, position.y + _offset.y));
}

void DrawableText::move(const sf::Vector2f & offset)
{
	_textObject.move(offset);
}

float DrawableText::getTextWidth() const
{
	return _textObject.getGlobalBounds().width;
}

void DrawableText::setText(const std::string & text)
{
	_textObject.setString(text);
}

void DrawableText::setColour(const sf::Color & colour)
{
	_textObject.setFillColor(colour);
}
