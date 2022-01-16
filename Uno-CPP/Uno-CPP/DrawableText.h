#pragma once
#include "DrawableObject.h"

/*
UNO

DrawableText class:
Defines a wrapper for an sf::Text object that automatically manages the object in a way that can be joined into a DrawableObjectGroup.

@author Peter Mitchell
@version 2022.1
*/
class DrawableText :
	public DrawableObject
{
public:
	// Sets the initial properties applied to the sf::Text object.
	DrawableText(const sf::Vector2f& position, const std::string& str, const sf::Font& font, const size_t fontSize, const sf::Color textColour, const sf::Uint32 style);
	virtual ~DrawableText() = default;

	// Draws the text to the renderWindow.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	// Sets the position by adding the object's offset to the specified position.
	virtual void setPositionWithOffset(const sf::Vector2f & position) override;

	// Moves the object by the specified offset without consideration for any other factors.
	virtual void move(const sf::Vector2f & offset) override;

	// Gets the width of the text based on the font and other settings.
	float getTextWidth() const;

	// Sets the text to the specified value.
	void setText(const std::string& text);

	// Sets the text colour to the specified value.
	void setColour(const sf::Color& colour);

private:
	// Reference to the object wrapped by this class.
	sf::Text _textObject;
};

