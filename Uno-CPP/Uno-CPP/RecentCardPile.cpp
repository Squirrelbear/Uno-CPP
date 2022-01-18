#include "RecentCardPile.h"

RecentCardPile::RecentCardPile(const sf::Vector2f centredCardPos, std::default_random_engine& randomEngine)
	: _centredCardPos(centredCardPos), _randomEngine(randomEngine)
{
}

RecentCardPile::~RecentCardPile()
{
	for (auto p : _recentCards) {
		delete p;
	}
}

void RecentCardPile::draw(sf::RenderWindow & renderWindow) const
{
	for (const auto& card : _recentCards) {
		card->drawCardFront(renderWindow);
	}
}

void RecentCardPile::setTopCardColour(const int colourID)
{
	_recentCards.at(_recentCards.size() - 1)->setColour(colourID);
}

void RecentCardPile::placeCard(Card * card)
{
	card->setPosition(sf::Vector2i(_centredCardPos.x + _randomEngine() % 24 - 12, _centredCardPos.y + _randomEngine() % 24 - 12));
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
		setTopCardColour(_randomEngine() % 4);
	}
}

Card * RecentCardPile::getTopCard() const
{
	return _recentCards.at(_recentCards.size()-1);
}

Card * RecentCardPile::getCardBelowTop() const
{
	return _recentCards.at(_recentCards.size() - 2);
}
