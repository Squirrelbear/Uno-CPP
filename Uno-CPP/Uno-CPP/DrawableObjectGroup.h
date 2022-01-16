#pragma once
#include "DrawableObject.h"

/*
UNO

DrawableObjectGroup class:
Defines a class that manages a collection of child objects for the purpose of easy rendering.
Objects are drawn in the order they are added as children.

@author Peter Mitchell
@version 2022.1
*/
class DrawableObjectGroup :
	public DrawableObject
{
public:
	// Creates an empty group of child objects.
	DrawableObjectGroup() {}
	virtual ~DrawableObjectGroup();

	// Draws all objects in the order they were added as children.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	// Sets the position of all objects by adding the object's offset to the specified position.
	virtual void setPositionWithOffset(const sf::Vector2f & position) override;

	// Moves all the objects by the specified offset without consideration for any other factors.
	virtual void move(const sf::Vector2f & offset) override;

	// Adds a child at the specified index. If the index is out of bounds it is forced to the back by default.
	void addChild(DrawableObject* child, const size_t index);

	// Adds a child to the back of the list (drawn on top of preceeding children).
	void addChild(DrawableObject* child);

	// Gets a reference to the child by the specified index.
	DrawableObject* getChild(const size_t index);

private:
	// Collection of child DrawableObjects that are drawn and managed as a group by this object.
	std::vector<DrawableObject*> _childObjects;
};

