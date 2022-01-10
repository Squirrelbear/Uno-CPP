#include "EllipseShape.h"

EllipseShape::EllipseShape(const sf::Vector2f & radius)
	: _radius(radius)
{
	update();
}

EllipseShape::~EllipseShape()
{
}

void EllipseShape::setRadius(const sf::Vector2f & radius)
{
	_radius = radius;
	update();
}

const sf::Vector2f & EllipseShape::getRadius() const
{
	return _radius;
}

std::size_t EllipseShape::getPointCount() const
{
	return 30;
}

sf::Vector2f EllipseShape::getPoint(std::size_t index) const
{
	static const float pi = 3.141592654f;

	float angle = index * 2 * pi / getPointCount() - pi / 2;
	float x = std::cos(angle) * _radius.x;
	float y = std::sin(angle) * _radius.y;

	return sf::Vector2f(_radius.x + x, _radius.y + y);
}
