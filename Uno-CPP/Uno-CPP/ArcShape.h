#pragma once

#include <SFML/Graphics.hpp>

class ArcShape : public sf::Shape
{
public:
	explicit ArcShape(const float startAngle, const float endAngle, const sf::Vector2f& radius);
	virtual ~ArcShape() = default;

	void setRadius(const sf::Vector2f& radius);
	const sf::Vector2f& getRadius() const;

	virtual std::size_t getPointCount() const;
	virtual sf::Vector2f getPoint(std::size_t index) const;

private:
	sf::Vector2f _radius;
	float _startAngle;
	float _endAngle;
	std::size_t _pointCount;
};

