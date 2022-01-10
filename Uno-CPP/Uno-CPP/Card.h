#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Constant definition of the width of a card to be used for calculations.
#define CARD_WIDTH 60
// Constant definition of the height of a card to be used for calculations.
#define CARD_HEIGHT 90

/**
 * Uno
 *
 * Card class:
 * Defines a Card including its properties and methods for showing appearance of the front and back.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class Card
{
public:
	Card(const int faceValueID, const int colourID, const int cardID);
	~Card();

	//void drawCardFront() const;
	//void drawCardBack() const;

	void setColour(const int colourID);
	int getColourID() const;
	int getFaceValueID() const;
	int getUniqueCardID() const;
	int getScoreValue() const;
	std::string getCardLabel() const;
	std::string getCornerLabel() const;

	static sf::Color getColourByID(const int colourID);
	static std::string getLabelByFaceValue(const int faceValue);
	static std::string getCornerLabelByFaceValue(const int faceValue);

private:
	// Location of the card in the game. Width and Height are set to CARD_WIDTH and CARD_HEIGHT.
	sf::IntRect _bounds;
	// The label in the centre of the card.
	std::string _cardLabel;
	// The label in both corners of the card.
	std::string _cornerLabel;
	/* The faceValue to represent what type of number or other visual appearance the card has.
		0 to 9 are numbers, 10 = Draw Two, 11 = Skip, 12 = Reverse, 13 = Draw Four, 14 = Wild 
		Face value is used when the object is created to initialise the labels.	*/
	const int _faceValueID;
	// The unique ID based on order drawn from the deck.
	const int _uniqueCardID;
	// The ID used to determine which of the four colours it is (or 4 if the card is a wild without colour set yet).
	int _colourID;
	// The colour used for drawing based on the colourID.
	sf::Color _drawColour;
	sf::CircleShape shape;
};

