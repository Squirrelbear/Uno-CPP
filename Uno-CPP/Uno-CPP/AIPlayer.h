#pragma once
#include "Player.h"
#include "TurnDecisionAction.h"
#include <random>

/**
 * Uno
 *
 * AIPlayer class:
 * Defines a specific variation of the Player that is handled
 * automatically by AI choosing actions to take during updates
 * with randomised delays to give players time to watch actions as they occur.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class AIPlayer :
	public Player
{
public:
	/**
	 * Defines the different types of Strategies that an AI can have.
	 * Random: Selects one of the other three randomly.
	 * Offensive: Uses low value cards first.
	 * Defensive: Uses high value cards first.
	 * Chaotic: Uses random cards from any that can be played.
	 */
	enum AIStrategy { Offensive, Defensive, Chaotic, Random };

	/**
	 * Defines an AI on top of a basic player ready to perform actions
	 * during update().
	 *
	 * @param playerID The playerID associated with this player.
	 * @param playerName The name shown for this player.
	 * @param bounds The region for placing cards within.
	 * @param strategy The strategy the AI will use to play.
	 * @param showPlayerNameLeft When true, the player's name is centred to the left side of the bounds, otherwise it is centred on the top.
	 */
	AIPlayer(const int playerID, const std::string& playerName, const sf::IntRect bounds, const AIStrategy strategy, const bool showPlayerNameLeft, std::default_random_engine& randomEngine, const sf::Font& font);
	virtual ~AIPlayer() = default;

	/**
	 * Checks for valid actions that can be taken by this player and
	 * performs them if there is the ability to.
	 *
	 * @param deltaTime Time since last update.
	 * @param currentPlayer A reference to the player whose current turn it is.
	 * @param currentTurnAction A reference to any turn action currently active.
	 * @param recentCards A reference to the pile of cards for accessing the top card.
	 * @param players A reference to the list of players who are playing.
	 * @param rules A reference to the rules that are in use.
	 */
	PlayerUpdateResult update(const float deltaTime, const Player* currentPlayer, TurnAction* currentTurnAction, const RecentCardPile* recentCards, const std::vector<Player*>& players, const RuleSet* rules) override;

	// Converts the AIStrategyfrom an enum into its std::string equivalent.
	static std::string aiStrategyToString(const AIStrategy strategy);

private:
	// Random engine used for making choices.
	std::default_random_engine& _randomEngine;

	// The Strategy to be used for selecting how cards are played.
	AIStrategy _strategy;

	// Timer used for delaying between actions.
	float _delayTimer;

	// ID of the player being considered for calling out.
	int _consideringPlayerID;

	// Delay till a decision is made about calling out.
	float _consideringDelayTimer;

	/** When true the current situation allows for a jump in.
	 * The transition from false to true is used to evaluate considerJumpIn. */
	bool _canJumpIn;

	// When true the AIPlayer has chosen to jump in after the period consideringJumpInTimer.
	bool _consideringJumpIn;

	// Timer till a jump in is executed if still allowed.
	float _consideringJumpInTimer;

	// Chooses a random Strategy.
	void selectRandomStrategy();

	/**
	 * Checks the current status of any available anti-uno calls and makes a decision whether to call them out.
	 *
	 * @param deltaTime Time since last update.
	 * @param players A reference to the list of players who are playing.
	 */
	PlayerUpdateResult updateAntiUnoCheck(const int deltaTime, const std::vector<Player*>& players);

	/**
	 * Updates the state of jumping in if it is allowed and possible for this player.
	 *
	 * @param deltaTime Time since last update.
	 * @param currentPlayer A reference to the player whose current turn it is.
	 * @param currentTurnAction A reference to any turn action currently active.
	 * @param recentCards A reference to the pile of cards for accessing the top card.
	 * @param rules A reference to the rules that are in use.
	 */
	PlayerUpdateResult updateJumpInCheck(const int deltaTime, const RuleSet* rules, const TurnAction* currentTurnAction, const Player* currentPlayer, const RecentCardPile* recentCards);

	/**
	 * Performs the turn by checking if there are any valid moves to be played.
	 * If there is no valid move, a card is drawn from the deck.
	 * Otherwise a card is chosen from the valid moves and played by initialising a TurnAction.
	 * Returns a pair containing any selection action sequence and optionally a PlayerUpdateResult if the player called UNO as part of the action.
	 */
	std::pair<TurnActionSequence<TurnAction>*, PlayerUpdateResult> performTurn(Card* topCard);

	// Resets the delay timer back to default.
	void resetDelayTimer();

	/**
	 * Takes a list of cards that can be played and chooses the card
	 * based on the selected strategy for the AI.
	 *
	 * @param validCards A collection of cards that are all valid to be played.
	 * @return A single valid card selected to be played.
	 */
	Card* chooseCard(std::vector<Card*>& validCards);

	/**
	 * Checks the flagName of the decisionAction to determine an
	 * appropriate response based on other methods in this class.
	 *
	 * @param decisionAction Reference to the current action requiring a decision.
	 * @param players A reference to the list of players who are playing.
	 * @param rules A reference to the rules that are in use.
	 */
	PlayerUpdateResult handleTurnDecision(TurnDecisionAction* decisionAction, const std::vector<Player*>& players, const RuleSet* rules);

	/**
	 * Gets a list of coloured cards in the AIPlayer's hand. If there are none, or on a random
	 * chance the colour is chosen randomly. Otherwise the first card in the list is selected
	 * as the colour to be applied.
	 *
	 * @param decisionAction Reference to the current action requiring a decision.
	 */
	PlayerUpdateResult chooseWildColour(TurnDecisionAction* decisionAction);

	/**
	* Always chooses to play cards that have been drawn.
	*
	* @param decisionAction Reference to the current action requiring a decision.
	*/
	PlayerUpdateResult chooseKeepOrPlay(TurnDecisionAction* decisionAction);

	/**
	* Finds the hand with the smallest number of cards other than their own and
	* swaps indicates a preference to swap with that target.
	*
	* @param decisionAction Reference to the current action requiring a decision.
	* @param players A reference to the list of players who are playing.
	*/
	PlayerUpdateResult choosePlayerToSwapWith(TurnDecisionAction* decisionAction, const std::vector<Player*>& players);

	/**
	* Checks if cards can be stacked and always chains if they can be with a valid card.
	* Otherwise will randomly decide whether to challenge or decline.
	*
	* @param decisionAction Reference to the current action requiring a decision.
	 * @param rules A reference to the rules that are in use.
	*/
	PlayerUpdateResult chooseChallengeOrDecline(TurnDecisionAction* decisionAction, const RuleSet* rules);

	/**
	 * Checks if cards can be stacked and then plays a valid +2 if it is available and allowed.
	 * Otherwise indicates that it is not being done.
	 *
	 * @param decisionAction Reference to the current action requiring a decision.
	 * @param rules A reference to the rules that are in use.
	 */
	PlayerUpdateResult chooseStackPlus2(TurnDecisionAction* decisionAction, const RuleSet* rules);

	// Evaluates whether to call UNO to make the AI safe.
	PlayerUpdateResult checkCallUNO();
};

