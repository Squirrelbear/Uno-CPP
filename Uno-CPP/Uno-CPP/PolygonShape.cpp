#include "PolygonShape.h"

PolygonShape::PolygonShape(const std::vector<sf::Vector2f>& pointList)
	: _pointList(pointList)
{
	update();
}

std::size_t PolygonShape::getPointCount() const
{
	return _pointList.size();
}

sf::Vector2f PolygonShape::getPoint(std::size_t index) const
{
	return _pointList.at(index);
}
