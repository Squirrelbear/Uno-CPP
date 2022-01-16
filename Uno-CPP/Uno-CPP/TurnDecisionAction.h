#pragma once
#include "TurnAction.h"

/**
 * Uno
 *
 * TurnDecisionAction class:
 * Defines a TurnDecisionAction that acts as a linked list of actions with a split to one
 * of two different options based on the value stored into a flagged variable.
 * Takes in an action that is expected to be performed once before iterating to a next state.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class TurnDecisionAction :
	public TurnAction
{
public:
	/**
	* Defines a TurnDecisionAction that chooses to use either the next or otherNext TurnAction
	* based on the value stored in flagName's mapped value stored in storedData. 0 will trigger next,
	* and 1 will trigger otherNext. getNext() will continue to return this current object until
	* the flagName has been set to a value.
	*
	* @param next Used when flagName's value is 0. Reference to the next TurnAction in the linked list sequence. This can be null to indicate the end.
	* @param otherNext Used when flagName's value is not 0. Reference to the alternative next TurnAction in the linked list sequence. This can be null to indicate the end.
	* @param timeOut When true, the TurnDecisionAction has a time limit to complete it.
	* @param flagName The flag variable used to determine when the decision has been met.
	* @param parentSequence The action to be performed via performAction().
	* @param actionID The action to be performed via performAction().
	* @param actionDebugText Text to be used to describe the current state in debug output.
	*/
	TurnDecisionAction(TurnAction* next, TurnAction* otherNext, const bool timeOut, const std::string& flagName, TurnActionSequence<TurnAction>* parentSequence, const TurnActionEffect actionID, const std::string& actionDebugText);
	virtual ~TurnDecisionAction() = default;

	/**
	 * Checks if the flagName has been set in storedData. If it has been set the
	 * value is evaluated such that 0 returns next, or any other value returns otherNext.
	 * When it has not yet been set the method will continue to return a reference
	 * to the current class.
	 *
	 * @return The current object or the next TurnAction to use.
	 */
	TurnAction* getNext() override;

	/**
	 * Checks if the action has already been performed. Then performs
	 * the action if it is not null based on the definition in TurnAction.
	 */
	void performAction() override;

	/**
	 * A shortcut method to storing a value directly into the flagName associated
	 * with this TurnDecisionAction.
	 *
	 * @param value The value to set into the storedData using flagName.
	 */
	void injectFlagProperty(const int value);

	// Returns true when the decision action requires a timeout for limiting interaction.
	bool requiresTimeout() const;

	// Gets the name used as a state variable for the decision.
	std::string getFlagName() const;

	// When true this decision has run its action once already meaning it should not rerun the action.
	bool getHasRunOnce() const;

	// When true, the TurnDecisionAction has a time limit to complete it.
	bool getTimeOut() const;

	// Method to expose the next action with no variable state.
	TurnAction* getOtherNextPointer() const;

private:
	// When true, the TurnDecisionAction has a time limit to complete it.
	bool _timeOut;
	// The alternative TurnAction to move to if the flag variable is non-zero.
	TurnAction* _otherNext;
	// The flag variable used to determine when the decision has been met.
	std::string _flagName;
	// A boolean to track whether the action has already been run.
	bool _hasRunOnce;
};

