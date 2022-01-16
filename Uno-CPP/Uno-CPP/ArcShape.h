#pragma once

#include <SFML/Graphics.hpp>

/*
UNO

ArcShape class:
Defines a modified version of the EllipseShape that has an angle of the ellipse visible with lines going into the centre.

@author Peter Mitchell
@version 2022.1
*/
class ArcShape : public sf::Shape
{
public:
	// Defines the shape with angles in degrees and radius of the partial ellipse.
	explicit ArcShape(const float startAngle, const float endAngle, const sf::Vector2f& radius);
	virtual ~ArcShape() = default;
	
	// Sets the radius of the ellipse shape that has a partial chunk visible.
	void setRadius(const sf::Vector2f& radius);

	// Gets the radius of the full ellipse.
	const sf::Vector2f& getRadius() const;

	// Gets the number of points used to represent the shape.
	virtual std::size_t getPointCount() const;

	// Generates the point at a given index.
	virtual sf::Vector2f getPoint(std::size_t index) const;

private:
	// Radius of the ellipse that is shown in part.
	sf::Vector2f _radius;

	// Start angle in radians.
	float _startAngle;

	// End angle in radians.
	float _endAngle;

	// Number of points used to represent the shape.
	std::size_t _pointCount;
};

