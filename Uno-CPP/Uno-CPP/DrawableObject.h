#pragma once

#include <SFML/Graphics.hpp>

/*
UNO

DrawableObject class:
Defines a pure virtual object that can be used to represent any DrawableObject for collecting into a DrawableObjectGroup.

@author Peter Mitchell
@version 2022.1
*/
class DrawableObject
{
public:
	// Defines the object with an offset that should be used when applying setPositionWithOffset().
	explicit DrawableObject(const sf::Vector2f& offset = sf::Vector2f(0.0f, 0.0f)) : _offset(offset) {};
	virtual ~DrawableObject() = default;

	// Pure virtual function for drawing the object.
	virtual void draw(sf::RenderWindow& renderWindow) const = 0;

	// Pure virtual function for moving the object by setting a position and then adding the _offset.
	virtual void setPositionWithOffset(const sf::Vector2f& position) = 0;

	// Pure virtual function for moving the object by an amount defined by offset.
	virtual void move(const sf::Vector2f& offset) = 0;

	// Sets the offset used for setPositionWithOffset.
	void setOffset(const sf::Vector2f& offset) {
		_offset = offset;
	}

protected:
	// An offset to apply when setPositionWithOffset() is called.
	sf::Vector2f _offset;
};

