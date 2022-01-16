#pragma once
#include "TurnActionSequence.h"
#include "TurnAction.h"

/*
UNO

TurnActionSequenceManager class:
Manages the swapping of states related to the TurnActionSequence and any that are queued.

@author Peter Mitchell
@version 2022.1
*/
class TurnActionSequenceManager
{
public:
	// Initialises the manager object ready to set a sequence.
	explicit TurnActionSequenceManager(const bool& debugModeEnabled);
	virtual ~TurnActionSequenceManager();

	// TODO
	void update();

	// Sets the sequence to the specified sequence.
	void setSequence(TurnActionSequence<TurnAction>* newSequence);

	// Queues the specified sequence to begin when the current sequence has safely ended.
	void queueSequence(TurnActionSequence<TurnAction>* newSequence);

	// Gets the current turn action for the current sequence.
	TurnAction* getCurrentTurnAction() const;
	
	// Returns true when there is a sequence actively being run.
	bool hasActiveTurnAction() const;

	// [DEBUG ONLY] Toggle the state of showing task action notes
	void toggleDebugShowTaskActionNotes();

	// [DEBUG ONLY] Toggle the showing of a state on a new action
	void toggleDebugShowTreeOnNewAction();

private:
	// The current sequence of TurnActions.
	TurnActionSequence<TurnAction>* _currentSequence;
	
	// The queued sequence of TurnActions ready for when the current one ends.
	TurnActionSequence<TurnAction>* _queuedSequence;

	// Reference to the debug state
	const bool& _debugModeEnabled;

	// When GamePanel.DEBUG_MODE and this are true, output is shown for each transition in the TaskAction sequence.
	bool _debugShowTaskActionNotes;

	// When GamePanel.DEBUG_MODE and this are true, output is shown with the whole tree TaskAction sequence when setCurrentTurnAction is used.
	bool _debugShowTreeOnNewAction;
};

