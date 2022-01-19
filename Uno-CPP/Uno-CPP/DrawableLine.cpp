#include "DrawableLine.h"

DrawableLine::DrawableLine(std::initializer_list<sf::Vector2f> vertexList)
{
	_elementCount = vertexList.size();
	_line = getVertexList(vertexList);
}

DrawableLine::~DrawableLine()
{
	delete[] _line;
}

void DrawableLine::draw(sf::RenderWindow & renderWindow) const
{
	renderWindow.draw(_line, _elementCount, sf::Lines);
}

sf::Vertex * DrawableLine::getVertexList(std::initializer_list<sf::Vector2f> vertexList)
{
	sf::Vertex* result = new sf::Vertex[vertexList.size()];
	size_t insertPos = 0;
	for (const auto& pos : vertexList) {
		result[insertPos] = sf::Vertex(pos);
		result[insertPos].color = sf::Color::Black;
		++insertPos;
	}
	return result;
}
