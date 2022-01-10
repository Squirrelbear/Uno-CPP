#pragma once

#include "DrawableObjectGroup.h"

class CardBackGroupObject
	: public DrawableObjectGroup
{
public:
	CardBackGroupObject(const sf::Vector2f& initialPosition, const sf::Font& font);
	virtual ~CardBackGroupObject() = default;
};

