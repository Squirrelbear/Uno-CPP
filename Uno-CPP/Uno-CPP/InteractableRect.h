#pragma once

#include <SFML/Graphics.hpp>

class InteractableRect
{
public:
	explicit InteractableRect(sf::IntRect bounds) : _bounds(bounds) {}
	virtual ~InteractableRect() = default;

	bool isPositionInside(const sf::Vector2i& mousePosition) const {
		return _bounds.contains(mousePosition);
	}

protected:
	sf::IntRect _bounds;
};

