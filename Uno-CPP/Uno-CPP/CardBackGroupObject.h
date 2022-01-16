#pragma once

#include "DrawableObjectGroup.h"

/*
UNO

CardBackObjectGroup class:
Creates a visual representing the back of a card.

@author Peter Mitchell
@version 2022.1
*/
class CardBackGroupObject
	: public DrawableObjectGroup
{
public:
	// Initialises the generic Card back visual.
	CardBackGroupObject(const sf::Vector2f& initialPosition, const sf::Font& font);
	virtual ~CardBackGroupObject() = default;

	// Moves the object to the specified position and then draws it. Use this for drawing instead because the CardBackGroupObject is generic and shared between instances.
	void drawAt(const sf::Vector2f& position, sf::RenderWindow& renderWindow);
};

