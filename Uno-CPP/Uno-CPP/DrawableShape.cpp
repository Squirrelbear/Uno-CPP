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
