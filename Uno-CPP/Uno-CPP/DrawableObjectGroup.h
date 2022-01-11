#pragma once
#include "DrawableObject.h"
class DrawableObjectGroup :
	public DrawableObject
{
public:
	DrawableObjectGroup() {}
	virtual ~DrawableObjectGroup();

	virtual void draw(sf::RenderWindow & renderWindow) const override;
	virtual void setPositionWithOffset(const sf::Vector2f & position) override;
	virtual void move(const sf::Vector2f & offset) override;

	void addChild(DrawableObject* child, const size_t index);
	void addChild(DrawableObject* child);
	DrawableObject* getChild(const size_t index);

private:
	std::vector<DrawableObject*> _childObjects;
};

