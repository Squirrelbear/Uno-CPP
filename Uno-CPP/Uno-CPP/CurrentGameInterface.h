#pragma once
#include "WndInterface.h"
#include "Deck.h"
#include "RecentCardPile.h"
#include "LobbyPlayer.h"
#include "OverlayManager.h"
#include "PlayDirectionAnimation.h"
#include "RuleSet.h"
#include "TurnActionSequenceManager.h"
#include <vector>

/**
 * Uno
 *
 * CurrentGameInterface class:
 * Defines the main game view controlling a list of players and
 * managing the state of all game elements.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class CurrentGameInterface :
	public WndInterface
{
public:
	/**
	 * Initialise the interface with bounds and make it enabled. Use this version when coming from the Lobby for
	 * a new set of rounds.
	 */
	CurrentGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<LobbyPlayer*> playerList, RuleSet* ruleSet, std::default_random_engine& randomEngine);

	/**
	 * Initialise the interface with bounds and make it enabled. Use this version when coming from
	 * after a game has already been completed and the sequence of games is continuing.
	 */
	CurrentGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<Player*> playerList, RuleSet* ruleSet, std::default_random_engine& randomEngine);
	virtual ~CurrentGameInterface();

	// Updates all the game components that need to be updated on a timer.
	virtual void update(const float deltaTime) override;

	// Draws all the game elements that are available.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Does nothing if not enabled. Passes the interaction to the overlay manager,
	 * and allows the player to interact with the deck/their cards when it is their turn.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Does nothing if not enabled. Passes the mouse movement to the overlay manager and bottom player.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

	/**
	 * Handles the key events for this interface.
	 *
	 * @param keyCode The key that was pressed.
	 */
	virtual void handleKeyInput(const sf::Keyboard::Key key) override;

	/**
	 * Verifies the card can be played as a jump in and then swaps the current player,
	 * and initiates the action of the card being played.
	 *
	 * @param playerID The player trying to jump in.
	 * @param cardToPlay The card that is being jumped in with.
	 */
	void jumpIn(const int playerID, Card* cardToPlay);

	// Used to show an overlay based on a current decision.
	void showOverlayForTurnAction();

	/**
	 * Shows an overlay based on a String lookup in the overlay manager.
	 *
	 * @param overlayName Name that maps to a specific overlay.
	 */
	void showGeneralOverlay(const std::string& overlayName);

	// Forces all hands to reveal and stay revealed.
	void revealHands();

	// Sorts the player's hand.
	void sortHand();

	/**
	 * Returns the player who is currently playing the game.
	 *
	 * @return The player who is playing the game.
	 */
	Player* getBottomPlayer() const;

	/**
	 * Toggles the turn direction between clockwise and anti-clockwise.
	 * Including updating the animation direction.
	 */
	void toggleTurnDirection();

	// Moves to the next player depending on whether the direction is clockwise or anti-clockwise.
	void moveToNextPlayer();

	/**
	 * Applies the effect from being called out on not having said UNO.
	 * Flashes the message and draws two cards to that player.
	 */
	void applyAntiUno(const int playerID);

	/**
	 * Gets the current direction of play.
	 *
	 * @return When true the play direction is clockwise.
	 */
	bool isIncreasing() const;

	/**
	 * If there is a current action already active it will be queued to start asap.
	 * Otherwise the action is set up immediately.
	 *
	 * @param newSequence The TurnAction to begin.
	 */
	void setCurrentTurnAction(TurnActionSequence<TurnAction>* newSequence);

	/**
	 * Gets the current TurnAction if there is one.
	 *
	 * @return The current action or null.
	 */
	TurnAction* getCurrentTurnAction() const;

	/**
	 * Gets the ruleset to easily check and apply any rules.
	 *
	 * @return The ruleset definition.
	 */
	RuleSet* getRuleSet() const;

	/**
	 * Gets the currently active player for turn order.
	 *
	 * @return The player identified by currentPlayerID.
	 */
	Player* getCurrentPlayer() const;

	/**
	 * Gets a list of all players.
	 *
	 * @return A reference to all the players.
	 */
	std::vector<Player*> getAllPlayers() const;

	/**
	 * Looks up the player with the given ID.
	 *
	 * @param playerID ID to get from the players collection.
	 * @return The player matching the given playerID.
	 */
	Player* getPlayerByID(const int playerID) const;

	/**
	 * Gets the deck to provide access to drawing cards.
	 *
	 * @return A reference to the Deck.
	 */
	Deck* getDeck() const;

	// Returns Nothing when interface is still in use, Finished to move to PostGame.
	WndResultState getResultState() const override;

private:
	// The rules for what card actions are set and other specific changes to how the game is played.
	RuleSet* _ruleSet;

	// The deck of cards ready to have cards drawn from it.
	Deck* _deck;

	// Manages the active TurnActions and their sequences. 
	TurnActionSequenceManager* _turnActionSequenceManager;

	// A history of cards that have been played.
	RecentCardPile _recentCardPile;

	// A manager controlling the various overlays that are shown based on events during the game.
	OverlayManager* _overlayManager;

	// All the players that are currently playing including their hands and other details.
	std::vector<Player*> _players;

	// Reference to the player who is playing the game.
	Player* _bottomPlayer;

	// The current player who is in control of actions.
	int _currentPlayerID;

	// Turn order increasing (true) means clockwise, or false would be anti-clockwise.
	bool _isIncreasing;

	// Animation to show the direction of turn order.
	PlayDirectionAnimation* _playDirectionAnimation;

	// DEBUG MODE
	bool _debugModeEnabled;

	// Result state that changes to Finished when round ends.
	WndResultState _resultState;

	// Checks if there is currently a player who has won the game and initiates end game conditions once found.
	void checkForEndOfRound();

	// Updates the current turn action state by performing the action and then iterating to the next one if possible.
	void updateTurnAction();

	// Sets the current player to NotSafe if they have one card or Safe otherwise, all other players are set to Safe.
	void updateUNOState();

	/**
	 * Generates a list of players using the specified types. Requires a single ThisPlayer and 1 or 3 AIPlayer.
	 *
	 * @param playerList A list of player data to generate a collection.
	 * @param bounds The bounds to use for calculating offsets and regions.
	 */
	static std::vector<Player*> createPlayersFromLobby(const std::vector<LobbyPlayer*> playerList, sf::IntRect bounds, const sf::Font& font);
	/**
	 * Generates bounds for where a player's cards should be placed.
	 *
	 * @param direction 0=bottom, 1=left, 2=top, 3=right
	 * @param bounds The bounds to use for calculating offsets and regions.
	 * @return A Rectangle defining where the player should have their cards on the field.
	 */
	static sf::IntRect getPlayerRect(const int direction, const sf::IntRect& bounds);
};

