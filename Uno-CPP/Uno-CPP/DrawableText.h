#pragma once
#include "DrawableObject.h"
class DrawableText :
	public DrawableObject
{
public:
	DrawableText(const sf::Vector2f& position, const std::string& str, const sf::Font& font, const size_t fontSize, const sf::Color textColour, const sf::Uint32 style);
	virtual ~DrawableText() = default;

	// Inherited via DrawableObject
	virtual void draw(sf::RenderWindow & renderWindow) override;
	virtual void setPositionWithOffset(const sf::Vector2f & position) override;
	virtual void move(const sf::Vector2f & offset) override;

	float getTextWidth() const;

private:
	sf::Text _textObject;
};

