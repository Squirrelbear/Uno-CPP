#include "Card.h"

Card::Card(const int faceValueID, const int colourID, const int cardID, const std::shared_ptr<CardBackGroupObject>& cardBack, const sf::Font& font)
	: InteractableRect(sf::IntRect(0,0,CARD_WIDTH,CARD_HEIGHT)), 
	_faceValueID(faceValueID), _colourID(colourID), _uniqueCardID(cardID), _cardBack(cardBack)
{
	_drawColour = getColourByID(_colourID);
	_cardLabel = getLabelByFaceValue(_faceValueID);
	_cornerLabel = getCornerLabelByFaceValue(_faceValueID);
	_cardFront = std::make_unique<CardFrontObjectGroup>(faceValueID, colourID, sf::Vector2f(_bounds.left, _bounds.top), font);
}

Card::~Card()
{
}

void Card::drawCardFront(sf::RenderWindow & renderWindow) const
{
	_cardFront->draw(renderWindow);
}

void Card::drawCardBack(sf::RenderWindow & renderWindow) const
{
	_cardBack->drawAt(sf::Vector2f(_bounds.left, _bounds.top), renderWindow);
}

void Card::setColour(const int colourID)
{
	_colourID = colourID;
	_drawColour = getColourByID(_colourID);
}

int Card::getColourID() const
{
	return _colourID;
}

int Card::getFaceValueID() const
{
	return _faceValueID;
}

int Card::getUniqueCardID() const
{
	return _uniqueCardID;
}

int Card::getScoreValue() const
{
	if (_faceValueID < 10) {
		return _faceValueID;
	}
	else if (_faceValueID == 13 || _faceValueID == 14) {
		return 50;
	}
	else {
		return 20;
	}
}

sf::Color Card::getDrawColour() const
{
	return _drawColour;
}

std::string Card::getCardLabel() const
{
	return _cardLabel;
}

std::string Card::getCornerLabel() const
{
	return _cornerLabel;
}

sf::Color Card::getColourByID(const int colourID)
{
	switch (colourID) {
		case 0: return sf::Color(191, 48, 48); // Red
		case 1: return sf::Color(36, 94, 160); // Blue
		case 2: return sf::Color(115, 187, 54); // Green
		case 3: return sf::Color(238, 188, 65); // Yellow
		default: return sf::Color(0,0,0); // Black
	};
}

std::string Card::getLabelByFaceValue(const int faceValue)
{
	if (faceValue >= 0 && faceValue <= 9) {
		return std::to_string(faceValue);
	}

	switch (faceValue) {
		case 10: return "Draw Two";
		case 11: return "Skip";
		case 12: return "Reverse";
		case 13: return "Draw Four";
		case 14: return "Wild";
		default: return "ERROR";
	}
}

std::string Card::getCornerLabelByFaceValue(const int faceValue)
{
	switch (faceValue) {
		case 10: return "+2";
		case 13: return "+4";
		case 14: return "";
		default: return getLabelByFaceValue(faceValue);
	}
}

void Card::setPosition(const sf::Vector2i& newPosition)
{
	_bounds = sf::IntRect(newPosition.x, newPosition.y, _bounds.width, _bounds.height);
	_cardFront->setPositionWithOffset(sf::Vector2f(newPosition.x, newPosition.y));
}
