#include "RecentCardPile.h"

RecentCardPile::RecentCardPile(const sf::Vector2f centredCardPos)
	: _centredCardPos(centredCardPos)
{
}

RecentCardPile::~RecentCardPile()
{
	for (auto p : _recentCards) {
		delete p;
	}
}

void RecentCardPile::setTopCardColour(const int colourID)
{
	_recentCards.at(_recentCards.size() - 1)->setColour(colourID);
}

void RecentCardPile::placeCard(Card * card)
{
	// TODO add random
	//card->setPosition(_centredCardPos.x + (int)(Math.random() * 24 - 12), centredCardPos.y + (int)(Math.random() * 24 - 12));
	_recentCards.emplace_back(card);
	int MAX_CARD_HISTORY = 10;
	if (_recentCards.size() > MAX_CARD_HISTORY) {
		Card* oldCard = _recentCards.at(0);
		_recentCards.erase(_recentCards.begin());
		delete oldCard;
	}
}

void RecentCardPile::forcePlayCard(Card * card)
{
	placeCard(card);

	if (card->getFaceValueID() >= 13) {
		// TODO add random
		//setTopCardColour((int)(Math.random() * 4));
	}
}

Card * RecentCardPile::getTopCard() const
{
	return _recentCards.at(_recentCards.size()-1);
}
