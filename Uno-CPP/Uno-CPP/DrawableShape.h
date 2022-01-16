#pragma once
#include "DrawableObject.h"

/*
UNO

DrawableShape class:
Defines a wrapper for an sf::Shape object that automatically manages the object in a way that can be joined into a DrawableObjectGroup.

@author Peter Mitchell
@version 2022.1
*/
class DrawableShape :
	public DrawableObject
{
public:
	// Initialises the wrapper by passing in a shape with defined colour and optional offset.
	DrawableShape(sf::Shape* shape, const sf::Color& colour, const sf::Vector2f& offset = sf::Vector2f(0.0f, 0.0f))
		: DrawableObject(offset), _shape(shape) {
		_shape->setFillColor(colour);
	};
	virtual ~DrawableShape();

	// Draws the shape to the renderWindow.
	void draw(sf::RenderWindow& renderWindow) const override;

	// Sets the position by adding the object's offset to the specified position.
	void setPositionWithOffset(const sf::Vector2f & position) override;

	// Moves the object by the specified offset without consideration for any other factors.
	void move(const sf::Vector2f & moveOffset) override;

	// Sets the text colour to the specified value.
	void setColour(const sf::Color& colour);

	// Sets the border thickness (0 = no border) and colour.
	void setBorder(const float thickness, const sf::Color colour);

	// Changes the size of the shape to the specified size.
	void setSize(const sf::Vector2f size);

private:
	// Reference the managed shape that this class wraps.
	sf::Shape* _shape;
};

