#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Card.h"
#include "DrawableText.h"

/**
 * Uno
 *
 * Player class:
 * Defines a player with all the information about a single player.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class Player
{
public:
	/**
	 * Types of players include:
	 * ThisPlayer: Only one allowed, this is the player who is playing this game.
	 * AIPlayer: Controlled by an AI (should be using an AIPlayer class).
	 */
	enum PlayerType { ThisPlayer, AIPlayer };

	/**
	 * Safe indicates the player is not vulnerable to counter calls.
	 * Called indicates the Player called before ending their turn with a card.
	 * NotSafe indicates that players can counter call the player's uno and make them draw cards.
	 */
	enum UNOState { Safe, Called, NotSafe };

	/**
	 * Initialises the player with an empty hand and defaults to showing cards if
	 * the player is defined with the type ThisPlayer.
	 *
	 * @param playerID The unique ID for this player.
	 * @param playerName The name for this player.
	 * @param playerType The type of player. (ThisPlayer, AIPlayer, or NetworkPlayer).
	 * @param bounds The region for drawing the player's cards.
	 * @param showPlayerNameLeft When true, the player's name is centred to the left side of the bounds, otherwise it is centred on the top.
	 */
	Player(const int playerID, const std::string& playerName, const PlayerType playerType, const sf::IntRect bounds, const bool showPlayerNameLeft, const sf::Font& font);
	virtual ~Player();

	/**
	 * Does nothing by default.
	 *
	 * @param deltaTime Time since last update.
	 */
	virtual void update(const float deltaTime) {}

	// Draws the player's cards with either card backs or fronts. Then draws the player's name nearby.
	void draw(sf::RenderWindow& renderWindow) const;

	/**
	 * Adds the card to the hand and recalculates the positions where all cards should be positioned.
	 *
	 * @param card The card to add to the hand.
	 */
	void addCardToHand(Card* card);

	// Empties the hand. Warning! This deletes all the cards in the hand. 
	void emptyHand();

	/**
	 * Changes the visibility of the Player's cards.
	 *
	 * @param reveal True makes the card fronts show, false makes card backs show.
	 */
	void revealHand(const bool reveal);

	// Gets the type of Player.
	PlayerType getPlayerType() const;

	// Gets the unique player ID.
	int getPlayerID() const;

	/**
	 * Takes in a possible faceValue and colourValue that would normally be the
	 * top of pile colours. And checks every card in the hand to find a list
	 * of all cards that can be played and returns it.
	 *
	 * @param curFaceValue The faceValue to check against.
	 * @param curColourValue The colourID to check against.
	 * @return A list of cards that are valid to be played in this context.
	 */
	std::vector<Card*> getValidMoves(const int curFaceValue, const int curColourValue) const;

	/* Sorts the hand and recalculates the positions of all cards.
     * Cards are sorted first by colour and then by face values. */
	void sortHand();

	/**
	 * Updates the hover to check which card is hovered and then updates the
	 * positions of all cards to offset based on hovering.
	 *
	 * @param mousePosition Position of the mouse cursor.
	 */
	void updateHover(const sf::Vector2i& mousePosition);

	/**
	 * Removes the card from the hand and recalculates position of all cards.
	 *
	 * @param card Card to be removed.
	 */
	void removeCard(Card* card);

	/**
	 * Searches to find the cardID.
	 *
	 * @param cardID cardID to search for.
	 * @return The Card with cardID or null.
	 */
	Card* getCardByID(const int cardID);

	/**
	 * Updates the hovering position. Then returns any currently hovered card.
	 *
	 * @param mousePosition Position of the mouse.
	 * @return The currently hovered card (can be null if none).
	 */
	Card* chooseCardFromClick(const sf::Vector2i& mousePosition);

	// Gets all the cards in the player's hand.
	std::vector<Card*> getHand() const;

	// Gets the player name.
	std::string getPlayerName() const;

	/**
	 * Adds up the score of all cards in the current hand.
	 *
	 * @return A total score for all the cards in the hand.
	 */
	int getHandTotalScore() const;

	// Gets the centre of the player's region.
	sf::Vector2f getCentreOfBounds() const;

	/**
	 * Sets the currentRoundScore and increases the totalScore by this amount.
	 *
	 * @param newCurrentRoundScore New score for this player.
	 */
	void setCurrentRoundScore(const int newCurrentRoundScore);

	// Sets the won state to true.
	void setWon();

	/**
	 * This returns true when this player has won.
	 *
	 * @return The current won state.
	 */
	bool getWon() const;

	/**
	 * The total score between multiple rounds.
	 *
	 * @return The current total score for this player.
	 */
	int getTotalScore() const;

	/**
	 * Gets the current round score for this player.
	 *
	 * @return The current score for this player for the current round.
	 */
	int getCurrentRoundScore() const;

	// Reset the current round and total scores, the won state, and UNO State to Safe for the player.
	void resetScore();

	/**
	 * Can transition from Safe->Called (NotSafe->Called should not occur).
	 * Can transition from Safe->NotSafe, NotSafe->Safe, and Called->Safe.
	 * Will ignore attempt to transition from Called->NotSafe. This behaviour
	 * handles the ignoring of a transition to NotSafe when the turn ends.
	 *
	 * @param unoState The new state to set.
	 */
	void setUnoState(const UNOState unoState);

	/**
	 * Checks the current unoState for this player to verify if they are safe from being called.
	 *
	 * @return True if the UNOState is Safe or Called, and false if UNOState is
	 */
	bool isSafe() const;

	/**
	 * Gets the current UNOState that can be Safe, Called, or NotSafe.
	 *
	 * @return The current UNOState.
	 */
	UNOState getUnoState() const;

private:
	// The unique ID for this player.
	const int _playerID;

	// The name for this player.
	std::string _playerName;

	// The type of player. (ThisPlayer, or AIPlayer).
	const PlayerType _playerType;

	// The region for drawing the player's cards.
	const sf::IntRect _bounds;

	// The collection of cards contained in the player's hand.
	std::vector<Card*> _hand;

	// The card that the player is currently hovering their mouse over.
	Card* _hoveredCard;

	// When true the cards for this player are revealed face-up.
	bool _showCards;

	// The total score between multiple rounds for this player.
	int _totalScore;

	// The score for a single round for this player.
	int _currentRoundScore;

	// When true this player won the current round. ecessary to store this because a score could be 0 is all other players only have 0s in their hands.
	bool _wonRound;

	// When true, the player's name is centred to the left side of the bounds, otherwise it is centred on the top.
	const bool _showPlayerNameLeft;

	// The current UNOState that can be Safe, Called, or NotSafe.
	UNOState _unoState;

	// Name tag
	DrawableObjectGroup* _nameTag;

	// Reference to text to change its colour based on current player ID (handled via _nameTag for rendering).
	DrawableText* _playerNameText;

	/**
	 * Recalculates positions for all cards by calculating numbers of
	 * rows and columns then centring inside the region and applying
	 * positions to all cards in the hand.
	 */
	void recalculateCardPositions();
};

