#include "CardFrontObjectGroup.h"
#include "Card.h"
#include "DrawableShape.h"
#include "DrawableText.h"
#include "EllipseShape.h"
#include "ArcShape.h"

CardFrontObjectGroup::CardFrontObjectGroup(const Card & templateCard, const sf::Vector2f& initialPosition, const sf::Font& font)
{
	// Black background with white border
	DrawableShape* border = new DrawableShape(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT)), sf::Color::White);
	addChild(border);
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH - 4, CARD_HEIGHT - 4)), 
													templateCard.getDrawColour(), sf::Vector2f(2, 2));
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
			ArcShape* arcShape = new ArcShape(90 * i, 90 * (i+1), sf::Vector2f((CARD_WIDTH - 8) / 2, ((CARD_WIDTH - 8) / 4)/2+10));
			// start end: 270 + 90 * i, 90
			DrawableShape* arc = new DrawableShape(arcShape, Card::getColourByID(i), sf::Vector2f(4, CARD_HEIGHT / 2 - ((CARD_WIDTH - 8) / 4)));
			addChild(arc);
		}
	}

	// Shadow behind central text (only shows for wild and short text)
	if (templateCard.getColourID() == 4 || templateCard.getCardLabel().length() <= 4) {
		size_t fontHeight = (templateCard.getCardLabel().length() > 4) ? 10 : 20;
		DrawableText* shadowText = new DrawableText(sf::Vector2f(0, 0), templateCard.getCardLabel(), font, fontHeight, sf::Color::Black, sf::Text::Bold);
		shadowText->setOffset(sf::Vector2f(CARD_WIDTH/2-shadowText->getTextWidth()/2-1, CARD_HEIGHT/2-fontHeight/2-1));
		addChild(shadowText);
	}

	// Foreground text that changes dependent on shadow and card colour.
	sf::Color textColour = sf::Color::White;
	if (templateCard.getColourID() != 4) {
		textColour = templateCard.getCardLabel().length() <= 4 ? templateCard.getDrawColour() : sf::Color::Black;
	}
	size_t fontHeight = (templateCard.getCardLabel().length() > 4) ? 10 : 20;
	DrawableText* foregroundText = new DrawableText(sf::Vector2f(0, 0), templateCard.getCardLabel(), font, fontHeight, textColour, sf::Text::Bold);
	foregroundText->setOffset(sf::Vector2f(CARD_WIDTH / 2 - foregroundText->getTextWidth() / 2, CARD_HEIGHT / 2 - fontHeight / 2));
	addChild(foregroundText);

	// Labels in each of the corners.	
	fontHeight = (templateCard.getCornerLabel().length() > 2) ? 10 : 20;
	DrawableText* topLeftCorner = new DrawableText(sf::Vector2f(0, 0), templateCard.getCornerLabel(), font, fontHeight, sf::Color::White, sf::Text::Bold);
	topLeftCorner->setOffset(sf::Vector2f(5, 5));
	addChild(topLeftCorner);
	DrawableText* bottomRightCorner = new DrawableText(sf::Vector2f(0, 0), templateCard.getCornerLabel(), font, fontHeight, sf::Color::White, sf::Text::Bold);
	bottomRightCorner->setOffset(sf::Vector2f(CARD_WIDTH-bottomRightCorner->getTextWidth()-5, CARD_HEIGHT-fontHeight-5));
	addChild(bottomRightCorner);

	// ESSENTIAL! This line updates all elements to apply the offset based on a relative position. 
	setPositionWithOffset(initialPosition);
}
