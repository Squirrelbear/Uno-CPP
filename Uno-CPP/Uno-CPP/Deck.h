#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>       // std::default_random_engine
#include "Card.h"
#include "CardBackGroupObject.h"
#include "DrawableText.h"

/**
 * Uno
 *
 * Deck class:
 * Represents a Deck with a collection of cards.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class Deck
{
public:
	/**
	 * Initialises the deck with no cards initially.
	 *
	 * @param position Position for the deck to appear in the game.
	 * @param font reference to the font for creating card renders.
	 */
	Deck(const sf::Vector2f position, const sf::Font& font, std::default_random_engine& randomEngine);
	virtual ~Deck();

	/**
	 * Draws a the back of a card to represent the deck position.
	 * With the word "DECK" appearing above it.
	 *
	 * @param g Reference to the Graphics object for rendering.
	 */
	void draw(sf::RenderWindow& renderWindow) const;

	/**
	 * If the deck is empty it is populated with a new deck.
	 * Then a card is removed from the deck and returned.
	 *
	 * @return A single card drawn from the deck.
	 */
	Card* drawCard();

private:
	// Random engine used for shuffling.
	std::default_random_engine& _randomEngine;

	// Position where the deck is located. Used for drawing the card back.
	sf::Vector2f _position;

	// Bounds for selection.
	sf::IntRect _bounds;

	// The list of cards currently in the deck.
	std::vector<Card*> _deck;

	// Shared reference to the card back.
	std::shared_ptr<CardBackGroupObject> _cardBack;

	// Used to give every card a unique cardID.
	int _nextCardID;

	// Title shown above the deck.
	DrawableText _deckTitle;

	/**
	 * Fills the deck by populating the deck with all the different card variations,
	 * and then shuffles the cards to create a random order.
	 */
	void fillDeck();

	// Adds a card matching the faceValueID and colourID to the _deck.
	void addCard(const int faceValueID, const int colourID);
};

