#pragma once

#include "RuleSet.h"
#include <vector>
#include <map>

/*
UNO

TurnActionSequence class:
Manages the control of a sequence of TurnAction objects. Simulates a linked list with multiple linear pathways.

@author Peter Mitchell
@version 2022.1
*/
template <class T>
class TurnActionSequence
{
public:
	// Initialises an empty sequence of TurnAction objects.
	TurnActionSequence();
	virtual ~TurnActionSequence() = default;

	// Links in a TurnAction as the start of a sequence and parses the tree to find all unique pointers
	// TODO

	/**
	 * Stores the specified data into the storedData map to be used for future iterations.
	 *
	 * @param key Key to store at in the storedData map.
	 * @param value Value to associate with the key.
	 */
	void injectProperty(const std::string& key, const int value);

	// Gets the value at the specified position, returns -1 if the key could not be found.
	int getPropertyValue(const std::string& key) const;

	// returns the rules
	RuleSet* getRuleSet();

private:
	// Unique list referencing all objects in the sequence so they can be properly deleted 
	std::vector<T*> _unorderedSequence;
	// Reference to the current action.
	T* _currentAction;
	// The data used as a shared reference for the sequence. TurnAction objects have access to this.
	std::map<std::string, int> _storedData;
	// Reference to the RuleSet for determining rules to follow during sequences.
	RuleSet* _ruleSet;

	/**
	 * Requires storedData contains (playerID, cardID, faceValueID, colourID)
	 * If the drawCount was set it is carried over. All other properties are discarded.
	 * The resulting TurnAction sequence is not returned, it is queued up directly
	 * into the current game to start a new sequence of playing the card.
	 * This method should be used to sequence playing of a card as part
	 * of other actions from card effects.
	 */
	void playCardAsActionFromData();

	 /**
	  * Requires stored data contains a playerID. The resulting TurnAction sequence is
	  * not returned, it is queued directly into the current game to start a sequence of
	  * drawing the card. This should only be used for sequencing additional draws
	  * when drawTillCanPlay? is true and triggers a recursive draw via drawCardAsAction().
	  */
	void drawCardAsActionFromData();

	/**
	 * Draws a card from the deck, stores the (cardID, faceValueID, and colourID) in storedData,
	 * and then adds the card to the current player's hand.
	 */
	void drawCard();

	/**
	 * Requires a cardID is set in storedData. Gets the card referenced by cardID in currentPlayer's hand,
	 * then removes the card from their hand and adds the card to the pile of recently played cards.
	 */
	void placeCard();

	// Moves to the next turn by moving one player in the current direction of play.
	void moveNextTurn();

	// Uses increaseDrawCountByN to increase the drawCount by 2.
	void increaseDrawCountBy2();
	
	// Uses increaseDrawCountByN to increase the drawCount by 4.
	void increaseDrawCountBy4();

	/**
	 * Gets the current value stored in drawCount in storedData if it exists and adds the value to N
	 * before storing the result back into drawCount.
	 *
	 * @param N The number to add to drawCount.
	 */
	void increaseDrawCountByN(const int N);

	/**
	 * Requires drawCount is set in storedData. The value is taken and a loop is performed drawCount
	 * number of times to call drawCard. After the cards have all been drawn the drawCount is removed
	 * from storedData to clear ready for any future use.
	 */
	void drawNCards();

	/**
	 * Requires storedData contains faceValueID, and colourID.
	 * Gets the top card of the pile and checks if the card stored in storedData is playable.
	 * The card is considered playable if it is either matching the faceValueID of the top card,
	 * the colour of the top card, or the card is a wild or +4.
	 * The result is stored into cardPlayable in storedData as a 1 if it is playable, or 0 if it is not.
	 */
	void isCardPlayable();

	/**
	 * Used to display a contextual choice overlay automatically based on the current TurnDecisionAction.
	 * Calling this method assumes that the current TurnAction is a TurnDecisionAction and will
	 * initialise any interface elements to wait for a required input.
	 */
	void beginChoiceOverlay();

	/**
	 * Checks the RuleSet to verify if cards should continue to be drawn until one is playable.
	 * The result is stored into drawTillCanPlay? as 1 if cards should continue to be drawn, or
	 * 0 if cards should not be drawn until something can be played.
	 */
	void checkDrawTillCanPlayRule();

	/**
	 * Checks the ruleset to verify if cards can be stacked. If they can be stacked, and the current player
	 * has any +2 card in their hand. The result is stored into hasPlus2AndResponseAllowed in storedData.
	 * If a response is allowed in this situation a 1 is stored, otherwise a 0.
	 */
	void hasPlus2AndResponseAllowed();

	// Triggers a SkipVisual overlay over the current player.
	void showSkip();

	// Toggles the turn direction between clockwise to anti-clockwise and vice versa.
	void togglePlayDirection();

	/**
	 * Requires colourID is set in storedData. The colourID is used to set the top card colour.
	 * This method is assuming that the action is being applied as part of a Wild colour choice (not enforced).
	 */
	void setTopPileColour();

	/**
	 * Gets the card played prior to the current top card and checks if there were any valid moves at the
	 * time that could have been played instead as a colour card. If there was couldPreviousPlayCard is set to 1.
	 * Otherwise couldPreviousPlayCard is set to 0.
	 */
	void checkCouldPlayCard();

	// Draws 4 cards to the current player. Use for applying the penalty when a +4 challenge is succeeded.
	void draw4ChallengeSuccess();

	/**
	 * Moves to the previous player. This is accomplished by reversing the play direction,
	 * then moving to the next player, and then moving the direction back.=
	 */
	void movePrevious();

	/**
	 * Requires otherPlayer is set in storedData. Gets the cards from the hands of otherPlayer,
	 * and the current player. Removes the cards from both players, and then adds all the cards
	 * to the opposite player's hand to complete the swap.
	 */
	void swapHandWithOther();

	/**
	 * Empties the hands of all players into an array of hands. Then shifts the hands based on direction of play.
	 * The hands are then stored back into players relative to the moved order.
	 */
	void passAllHands();

	// Shows either a tick or cross overlay on the player who challenged.
	void showChallengeResult();

	// Checks the conditions for whether a challenge is allowed or if there is also an allowed +4 stack option too.
	void checkNoBluffingRule();

	// Checks the forced play rule.
	void checkForcedPlayRule();
};