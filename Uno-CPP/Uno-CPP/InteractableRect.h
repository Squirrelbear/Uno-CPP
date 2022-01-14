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
	explicit InteractableRect(const sf::IntRect& bounds) : _bounds(bounds) {}
	virtual ~InteractableRect() = default;

	bool isPositionInside(const sf::Vector2i& mousePosition) const {
		return _bounds.contains(mousePosition);
	}

protected:
	sf::IntRect _bounds;
};

