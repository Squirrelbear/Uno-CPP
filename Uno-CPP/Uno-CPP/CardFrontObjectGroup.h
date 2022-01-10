#pragma once
#include "DrawableObjectGroup.h"
#include "Card.h"

class CardFrontObjectGroup :
	public DrawableObjectGroup
{
public:
	explicit CardFrontObjectGroup(const Card & templateCard, const sf::Vector2f& initialPosition, const sf::Font& font);
	virtual ~CardFrontObjectGroup() = default;
};

