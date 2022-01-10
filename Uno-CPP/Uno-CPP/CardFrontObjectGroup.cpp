#include "CardFrontObjectGroup.h"
#include "Card.h"
#include "DrawableShape.h"
#include "EllipseShape.h"

CardFrontObjectGroup::CardFrontObjectGroup(const Card & templateCard, const sf::Vector2f& initialPosition)
{
	// Black background with white border
	DrawableShape* border = new DrawableShape(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT)), sf::Color::White);
	addChild(border);
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH - 4, CARD_HEIGHT - 4)), 
													sf::Color::Black, sf::Vector2f(2, 2));
	addChild(background);

	if (templateCard.getColourID() != 4) {
		// White oval in centre of card
		DrawableShape* centralEllipse = new DrawableShape(new EllipseShape(sf::Vector2f((CARD_WIDTH - 8) / 2, (CARD_WIDTH - 8) / 4)), 
												sf::Color::White, sf::Vector2f(4, CARD_HEIGHT / 2 - ((CARD_WIDTH - 8) / 4)));
		addChild(centralEllipse);
	}
	else {
		// Red, blue, green, yellow segments for any wild card in the middle.
		for (int i = 0; i < 4; i++) {
			// TODO change to use ArcShape instead of EllipseShape
			EllipseShape* arcShape = new EllipseShape(sf::Vector2f((CARD_WIDTH - 8) / 2, (CARD_WIDTH - 8) / 4+10));
			// start end: 270 + 90 * i, 90
			DrawableShape* arc = new DrawableShape(arcShape, Card::getColourByID(i), sf::Vector2f(4, CARD_HEIGHT / 2 - ((CARD_WIDTH - 8) / 4)));
		}
	}

	size_t fontHeight = (templateCard.getCardLabel().length() > 4) ? 10 : 20;

	setPositionWithOffset(initialPosition);
}
