#include "DrawableObjectGroup.h"

DrawableObjectGroup::~DrawableObjectGroup()
{
	//for (auto childPointer : _childObjects)
	//	delete childPointer;
}

void DrawableObjectGroup::draw(sf::RenderWindow & renderWindow) const
{
	for (auto obj : _childObjects) {
		obj->draw(renderWindow);
	}
}

void DrawableObjectGroup::setPositionWithOffset(const sf::Vector2f & position)
{
	for (auto obj : _childObjects) {
		obj->setPositionWithOffset(position);
	}
}

void DrawableObjectGroup::move(const sf::Vector2f & offset)
{
	for (auto obj : _childObjects) {
		obj->move(offset);
	}
}

void DrawableObjectGroup::addChild(DrawableObject* child, const size_t index)
{
	if (index >= _childObjects.size()) {
		_childObjects.emplace_back(child);
	}
	else {
		_childObjects.insert(_childObjects.begin() + index, child);
	}
}

void DrawableObjectGroup::addChild(DrawableObject * child)
{
	addChild(child, _childObjects.size());
}

DrawableObject* DrawableObjectGroup::getChild(const size_t index)
{
	return _childObjects.at(index);
}
