#pragma once

#include "TurnActionSequence.h"
#include <string>
#include "TurnActionEffect.h"

/**
 * Uno
 *
 * TurnAction class:
 * Defines a TurnAction that acts as a linked list of actions.
 * Takes in an action that is expected to be performed once before iterating to a next state.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class TurnAction
{
public:
	/**
	* Stores the properties specified ready to use.
	*
	* @param next Reference to the next TurnAction in the linked list sequence. This can be null to indicate the end.
	* @param parentSequence Reference to the parent for accessing shared data and executing actions.
	* @param actionID The action to be performed via performAction().
	* @param actionDebugText Text to be used to describe the current state in debug output.
	*/
	TurnAction(TurnAction* next, TurnActionSequence<TurnAction>* parentSequence, const TurnActionEffect actionID, const std::string& actionDebugText);
	virtual ~TurnAction() = default;

	// Calls the action associated with this TurnAction.
	virtual void performAction();

	// Gets the reference to the next turn action after this one, can be null if at the end.
	virtual TurnAction* getNext();

	/**
	 * Stores the specified data into the storedData map to be used for future iterations.
	 *
	 * @param key Key to store at in the storedData map.
	 * @param value Value to associate with the key.
	 */
	void injectProperty(const std::string& key, const int value);

	// Gets the value at the specified position, returns -1 if the key could not be found.
	int getPropertyValue(const std::string& key) const;

	// Gets the debug text describing this TurnAction
	std::string getActionDebugText() const;

protected:
	// Reference to the parent for accessing shared data for the sequence and executing the actions.
	TurnActionSequence<TurnAction>* _parent;

	// Reference to the action to be executed by this TurnAction during performAction().
	const TurnActionEffect _actionID;

	// Reference to the next TurnAction in the linked list sequence. This can be null to indicate the end.
	TurnAction* _next;

	// Text to be used to describe the current state in debug output.
	std::string _actionDebugText;
};

