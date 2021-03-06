#pragma once

#include "Card.h"
#include <random>

/**
 * Uno
 *
 * RecentCardPile class:
 * Defines the place where cards are dumped after being played. Pile automatically clears itself after reaching a bounded size.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class RecentCardPile
{
public:
	// Initialises the empty pile at the specified position.
	RecentCardPile(const sf::Vector2f centredCardPos, std::default_random_engine& randomEngine);
	virtual ~RecentCardPile();

	// Draw the cards on the pile.
	void draw(sf::RenderWindow & renderWindow) const;

	/**
	 * Changes the top card colour. Used for changing the colour of the wild and +4 cards.
	 *
	 * @param colourID 0=Red, 1=Blue, 2=Green, 3=Yellow
	 */
	void setTopCardColour(const int colourID);

	/**
	 * Moves the card's position to the card pile with a random offset and adds it
	 * to the collection of recentCards. If the number of recent cards is more
	 * than the maximum allowed the oldest card is removed.
	 *
	 * @param card Card to place on top of the card pile.
	 */
	void placeCard(Card* card);

	/**
	 * Used to play the first card. This consists of simply placing the card
	 * with no action, and if the card is a wild the colour is randomised.
	 *
	 * @param card Card to place on top of the card pile with no action.
	 */
	void forcePlayCard(Card* card);

	/**
	 * Gets the most recently played recentCard.
	 *
	 * @return The card that appears on top of the played pile.
	 */
	Card* getTopCard() const;

	// Gets the card below the top card.
	Card* getCardBelowTop() const;

private:
	// A history of cards that have been played.
	std::vector<Card*> _recentCards;

	// Centre point used to randomly spread cards around.
	sf::Vector2f _centredCardPos;

	// Reference to the random engine for randomising card colours and positions.
	std::default_random_engine& _randomEngine;
};

