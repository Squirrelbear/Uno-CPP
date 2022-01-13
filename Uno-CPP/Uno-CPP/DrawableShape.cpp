#include "DrawableShape.h"

DrawableShape::~DrawableShape()
{
	delete _shape;
}

void DrawableShape::draw(sf::RenderWindow& renderWindow) const
{
	renderWindow.draw(*_shape);
}

void DrawableShape::setPositionWithOffset(const sf::Vector2f & position)
{
	_shape->setPosition(sf::Vector2f(position.x + _offset.x, position.y + _offset.y));
}

void DrawableShape::move(const sf::Vector2f & moveOffset)
{
	_shape->move(moveOffset);
}

void DrawableShape::setColour(const sf::Color & colour)
{
	_shape->setFillColor(colour);
}

void DrawableShape::setBorder(const float thickness, const sf::Color colour)
{
	_shape->setOutlineThickness(thickness);
	_shape->setOutlineColor(colour);
}

void DrawableShape::setSize(const sf::Vector2f size)
{
	auto rect = _shape->getTextureRect();
	rect.height += size.y;
	rect.width += size.x;
	_shape->setTextureRect(rect);
}
