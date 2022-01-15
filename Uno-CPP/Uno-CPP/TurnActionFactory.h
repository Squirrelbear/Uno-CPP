#pragma once

#include "TurnActionSequence.h"
#include "TurnDecisionAction.h"

static class TurnActionFactory
{
public:
	/**
	 * Queues placing the specified card followed by the sequence of actions that result from the
	 * type of card that was played from calling this method.
	 *
	 * @param playerID The player controlling the card.
	 * @param cardID The unique ID associated with the card to be played.
	 * @param faceValueID The reference to what is shown on the card to be played.
	 * @param colourID The colour of the card to be played.
	 * @return A sequence of actions based on the card that is being played.
	 */
	static TurnActionSequence<TurnAction>* playCardAsAction(const int playerID, const int cardID, const int faceValueID, const int colourID);

	/**
	 * This method should be used when the player is using their turn action to draw a card from the deck.
	 * The decision tree generated by this method follows the sequence shown below. It is constructed in reverse.
	 *     Draw Card -> cardPlayable? -> (true) -> keepOrPlay? -> Keep -> MoveToNextTurn
	 *                                                         -> Play -> Begin Action Play Card
	 *                                -> (false) -> drawTillCanPlay? -> (true) ->  Begin Action Draw Card
	 *                                                               -> (false) -> MoveToNextTurn
	 *
	 * @param playerID The player who is performing the drawing action.
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnActionSequence<TurnAction>* drawCardAsAction(const int playerID);

private:
	/**
	 * Generates a sequence of TurnActions to handle the events required when a +2 card is played.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 *   MoveToNextTurn -> Increase Draw Count +2 -> hasPlus2AndResponseAllowed?
	 *                     -> (true) -> isStacking? -> (true) -> Begin Action Play Card
	 *                                              -> (false) -> Draw Card * Draw Count + Reset Draw Count to 0 -> MoveToNextTurn
	 *                     -> (false) -> Draw Card * Draw Count + Reset Draw Count to 0 -> MoveToNextTurn
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playPlus2Action(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Generates a sequence of TurnActions to handle the events required when a +4 card is played.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 * couldPreviousPlayCard PreCheck -> WildColourSelection -> Set top pile colour -> MoveToNextTurn
	 * 				-> isChallenging? -> (true) -> couldPreviousPlayCard? -> (true) -> MoveToPreviousTurn -> Draw 6 cards -> MoveToNextPlayer -> Draw * Draw Count + reset
	 * 																	  -> (false) -> Increase drawCount by 4 -> Draw * Draw Count + reset draw count
	 * 								  -> (false) -> isChaining? -> (true) -> Begin Action Play Card
	 * 															-> (false) -> Increase drawCount by 4 -> Draw * Draw Count + reset draw count
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playPlus4Action(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Generates a sequence of TurnActions to handle the events required when a Wild card is played.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 * WildColourSelection -> Set top pile colour -> MoveToNextTurn
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playWildAction(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Generates a sequence of TurnActions to handle the events required when a Skip card is played.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 * MoveToNextTurn -> Show Skip -> MoveToNextTurn
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playSkipAction(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Generates a sequence of TurnActions to handle the events required when a Reverse card is played.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 * Toggle Turn Direction Order -> MoveToNextTurn
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playReverseAction(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Generates a sequence of TurnActions to handle the events required when a Swap card is played.
	 * This is for a different game mode with selecting a player to swap hands with.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 * otherPlayer? Selection -> Swap Hands (current, selected) -> MoveToNextTurn
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playSwapAction(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Generates a sequence of TurnActions to handle the events required when a Pass All card is played.
	 * This is for a different game mode with shifting all hands around based on turn order.
	 * The following shows the sequence that can occur. It is constructed in reverse.
	 *
	 * Pass All Cards -> MoveToNextTurn
	 *
	 * @return The decision tree sequence of TurnActions as described ready for iteration.
	 */
	static TurnAction* playPassAllAction(TurnActionSequence<TurnAction>* nextSequence);

	/**
	 * Looks up a relevant action to apply based on the faceValue of the card. If there is no matching
	 * associated action to generate a TurnAction sequence from then the default is to move to the next turn.
	 *
	 * @param faceValueID The face value of the card being played.
	 * @return A sequence of TurnActions based on the faceValue of the card being played.
	 */
	static TurnAction* cardIDToTurnAction(const int faceValueID, TurnActionSequence<TurnAction>* nextSequence);
};

