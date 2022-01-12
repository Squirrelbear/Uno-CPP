#pragma once
#include "DrawableObjectGroup.h"

class CardFrontObjectGroup :
	public DrawableObjectGroup
{
public:
	CardFrontObjectGroup(const int faceValue, const int colourID, const sf::Vector2f& initialPosition, const sf::Font& font);
	virtual ~CardFrontObjectGroup() = default;
};

