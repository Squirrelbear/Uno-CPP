#include "CardBackGroupObject.h"

#include "Card.h"
#include "DrawableShape.h"
#include "EllipseShape.h"
#include "DrawableText.h"

CardBackGroupObject::CardBackGroupObject(const sf::Vector2f& initialPosition, const sf::Font& font)
{
	// Black background with white border
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH - 4, CARD_HEIGHT - 4)),
														sf::Color::Black, sf::Vector2f(2, 2));
	background->setBorder(2, sf::Color::White);
	addChild(background);

	DrawableShape* centreOval = new DrawableShape(new EllipseShape(sf::Vector2f((CARD_WIDTH-8)/2,(CARD_WIDTH-8)/4+1)), 
												sf::Color(147, 44, 44), sf::Vector2f(4, CARD_HEIGHT / 2 - (CARD_WIDTH - 8) / 4));
	addChild(centreOval);

	DrawableText* shadowText = new DrawableText(sf::Vector2f(0, 0), "UNO", font, 20, sf::Color::Black, sf::Text::Bold);
	shadowText->setOffset(sf::Vector2f(CARD_WIDTH / 2 - shadowText->getTextWidth() / 2 - 2, CARD_HEIGHT / 2 - 20 / 2 - 2));
	addChild(shadowText);

	DrawableText* mainText = new DrawableText(sf::Vector2f(0, 0), "UNO", font, 20, sf::Color(226, 173, 67), sf::Text::Bold);
	mainText->setOffset(sf::Vector2f(CARD_WIDTH / 2 - mainText->getTextWidth() / 2, CARD_HEIGHT / 2 - 20 / 2));
	addChild(mainText);

	// ESSENTIAL! This line updates all elements to apply the offset based on a relative position. 
	setPositionWithOffset(initialPosition);
}

void CardBackGroupObject::drawAt(const sf::Vector2f & position, sf::RenderWindow& renderWindow)
{
	setPositionWithOffset(position);
	draw(renderWindow);
}

