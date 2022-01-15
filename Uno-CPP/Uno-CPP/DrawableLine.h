#pragma once

#include "DrawableObject.h"

class DrawableLine : public DrawableObject
{
public:
	// Creates a line made up of the vextexList points. This line can not be moved.
	DrawableLine(std::initializer_list<sf::Vector2f> vertexList);
	virtual ~DrawableLine();

	// Draw the line.
	virtual void draw(sf::RenderWindow & renderWindow) const override;
	// Do nothing
	virtual void setPositionWithOffset(const sf::Vector2f & position) override {}
	// Do nothing
	virtual void move(const sf::Vector2f & offset) override {}

private:
	// Number of points
	std::size_t _elementCount;
	// Points to draw
	sf::Vertex* _line;

	// Initialises the line by packaging vertexList into sf::Vertex objects.
	sf::Vertex* getVertexList(std::initializer_list<sf::Vector2f> vertexList);
};

