#pragma once

#include <SFML/Graphics.hpp>

class PolygonShape : public sf::Shape
{
public:
	PolygonShape(const std::vector<sf::Vector2f>& pointList);
	virtual ~PolygonShape() = default;

	virtual std::size_t getPointCount() const;
	virtual sf::Vector2f getPoint(std::size_t index) const;

private:
	std::vector<sf::Vector2f> _pointList;
};

