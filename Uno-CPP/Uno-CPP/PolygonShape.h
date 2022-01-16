#pragma once

#include <SFML/Graphics.hpp>

/**
 * Uno
 *
 * PolygonShape class:
 * Defines a polygon shape using a point list. The polygon exists as a line sequence.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class PolygonShape : public sf::Shape
{
public:
	// Sets up the shape with a point list of all points used to draw the polygon.
	PolygonShape(const std::vector<sf::Vector2f>& pointList);
	virtual ~PolygonShape() = default;

	// Gets the number of points (required by sf::Shape)
	virtual std::size_t getPointCount() const;

	// Gets the point at a given index in the polygon (required by sf::Shape)
	virtual sf::Vector2f getPoint(std::size_t index) const;

private:
	// The list of points drawn for this polygon.
	std::vector<sf::Vector2f> _pointList;
};

