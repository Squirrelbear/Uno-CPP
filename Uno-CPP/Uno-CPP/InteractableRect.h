#pragma once

#include <SFML/Graphics.hpp>

/*
UNO 

InteractableRect class:
Simple wrapper for the sf::IntRect class to let classes require a rect be set.

@author: Peter Mitchell
@version: 2022.1
*/
class InteractableRect
{
public:
	// Passes the bounds for the object for use with isPositionInside().
	explicit InteractableRect(const sf::IntRect& bounds) : _bounds(bounds) {}
	virtual ~InteractableRect() = default;

	// returns true if the position is inside this object's bounds.
	bool isPositionInside(const sf::Vector2i& mousePosition) const {
		return _bounds.contains(mousePosition);
	}

protected:
	sf::IntRect _bounds;
};

