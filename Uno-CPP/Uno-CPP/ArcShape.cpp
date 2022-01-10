#include "ArcShape.h"

ArcShape::ArcShape(const float startAngle, const float endAngle, const sf::Vector2f& radius = sf::Vector2f(0.f, 0.f))
	: _radius(radius), _startAngle(startAngle * 3.141592654f / 180), _endAngle(endAngle * 3.141592654f / 180)
{
	_pointCount = 30;

	update();
}

void ArcShape::setRadius(const sf::Vector2f & radius)
{
	_radius = radius;
	update();
}

const sf::Vector2f & ArcShape::getRadius() const
{
	return _radius;
}

std::size_t ArcShape::getPointCount() const
{
	return _pointCount;
}

sf::Vector2f ArcShape::getPoint(std::size_t index) const
{
	static const float pi = 3.141592654f;

	if (index == _pointCount-1) {
		return sf::Vector2f(_radius.x, _radius.y);
	}

	float angle = index * 2 * pi / (getPointCount()-1) + _startAngle;
	if (angle > _endAngle) {
		angle = _endAngle;
	}
	float x = std::cos(angle) * _radius.x;
	float y = std::sin(angle) * _radius.y;
	return sf::Vector2f(_radius.x + x, _radius.y + y);
}
