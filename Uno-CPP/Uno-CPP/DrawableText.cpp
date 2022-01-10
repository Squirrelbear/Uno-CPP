#include "DrawableText.h"

DrawableText::DrawableText(const sf::Vector2f& position, const std::string & str, const sf::Font & font, const size_t fontSize, const sf::Color textColour, const sf::Uint32 style = sf::Text::Regular)
{
	_textObject.setFont(font);
	_textObject.setString(str);
	_textObject.setCharacterSize(fontSize);
	_textObject.setFillColor(textColour);
	_textObject.setStyle(style);
	_textObject.setPosition(position);
}

void DrawableText::draw(sf::RenderWindow & renderWindow)
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
