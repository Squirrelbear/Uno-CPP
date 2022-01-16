#pragma once
#include "DrawableObjectGroup.h"

/*
UNO

CardFrontObjectGroup class:
Creates a visual representing the front of a card that is generated based on faceValue and colourID.

@author Peter Mitchell
@version 2022.1
*/
class CardFrontObjectGroup :
	public DrawableObjectGroup
{
public:
	// Generates the visual representation based on specified faceValue and colourID.
	CardFrontObjectGroup(const int faceValue, const int colourID, const sf::Vector2f& initialPosition, const sf::Font& font);
	virtual ~CardFrontObjectGroup() = default;
};

