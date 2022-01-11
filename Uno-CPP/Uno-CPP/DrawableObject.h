#pragma once

#include <SFML/Graphics.hpp>

class DrawableObject
{
public:
	DrawableObject(const sf::Vector2f& offset = sf::Vector2f(0.0f, 0.0f)) : _offset(offset) {};
	virtual ~DrawableObject() = default;

	virtual void draw(sf::RenderWindow& renderWindow) const = 0;
	virtual void setPositionWithOffset(const sf::Vector2f& position) = 0;
	virtual void move(const sf::Vector2f& offset) = 0;

	void setOffset(const sf::Vector2f& offset) {
		_offset = offset;
	}

protected:
	sf::Vector2f _offset;
};

