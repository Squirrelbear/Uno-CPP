#pragma once

#include <SFML/Graphics.hpp>

/*
UNO

EllipseShape class:
	Based on the code from: https://www.sfml-dev.org/tutorials/2.5/graphics-shape.php
	Defines an oval shape that can be deformed with independent radius for width and height.

@author Peter Mitchell (Based on the code from: https://www.sfml-dev.org/tutorials/2.5/graphics-shape.php )
@version 2022.1
*/
class EllipseShape : public sf::Shape
{
public:
	// Initialises the EllipseShape with either a specified radius or (0.0,0.0) radius.
	explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0.f, 0.f));
	~EllipseShape();

	// Sets the radius to the specified values and updates the shape.
	void setRadius(const sf::Vector2f& radius);
	
	// Gets the current radius for the shape.
	const sf::Vector2f& getRadius() const;

	// Gets the number of points (required by sf::Shape)
	virtual std::size_t getPointCount() const;

	// Calculates the position of a point equally spaces around the ellipse based on index (required by sf::Shape)
	virtual sf::Vector2f getPoint(std::size_t index) const;

private:
	// Current radius of the shape.
	sf::Vector2f _radius;
};

