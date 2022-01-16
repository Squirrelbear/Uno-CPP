#include "TurnActionSequenceManager.h"
#include <iostream>
#include "TurnDecisionAction.h"

TurnActionSequenceManager::TurnActionSequenceManager(const bool& debugModeEnabled)
	: _debugModeEnabled(debugModeEnabled)
{
	_currentSequence = nullptr;
	_queuedSequence = nullptr;
	_debugShowTaskActionNotes = false;
	_debugShowTreeOnNewAction = false;
}

TurnActionSequenceManager::~TurnActionSequenceManager()
{
	if (_currentSequence != nullptr) {
		delete _queuedSequence;
	}
	if (_queuedSequence != nullptr) {
		delete _queuedSequence;
	}
}

void TurnActionSequenceManager::update()
{
	if (hasActiveTurnAction()) {
		// Tree Debug Output
		if (_debugModeEnabled && _debugShowTaskActionNotes) {
			if (typeid(getCurrentTurnAction()) == typeid(TurnDecisionAction)) {
				if (!(dynamic_cast<TurnDecisionAction*>(getCurrentTurnAction())->getHasRunOnce())) {
					std::cout << getCurrentTurnAction()->getActionDebugText() << std::endl;
				}
			}
			else {
				std::cout << getCurrentTurnAction()->getActionDebugText() << std::endl;
			}
		}
		// TODO handle performing action
		/*currentTurnAction.performAction();
		currentTurnAction = currentTurnAction.getNext();*/
		if (_queuedSequence != nullptr) {
			delete _currentSequence;
			_currentSequence = _queuedSequence;
			_queuedSequence = nullptr;
		}
	}
}

void TurnActionSequenceManager::setSequence(TurnActionSequence<TurnAction>* newSequence)
{
	if (hasActiveTurnAction()) {
		queueSequence(newSequence);

		if (_debugModeEnabled && _debugShowTreeOnNewAction) {
			std::cout << "Queued action sequence:" << std::endl;
			// TODO debug output tree
			//TurnActionFactory.debugOutputTurnActionTree(turnAction);
		}
	}
	else {
		if (_currentSequence != nullptr) {
			delete _queuedSequence;
		}
		_currentSequence = newSequence;

		if (_debugModeEnabled && _debugShowTreeOnNewAction) {
			std::cout << "Set action sequence:" << std::endl;
			// TODO debug output tree
			//TurnActionFactory.debugOutputTurnActionTree(turnAction);
		}
	}
}

void TurnActionSequenceManager::queueSequence(TurnActionSequence<TurnAction>* newSequence)
{
	if (_queuedSequence != nullptr) {
		delete _queuedSequence;
	}
	_queuedSequence = newSequence;
}

TurnAction * TurnActionSequenceManager::getCurrentTurnAction() const
{
	if (_currentSequence == nullptr) {
		return nullptr;
	}
	// TODO need to link after adding TurnActionSequence<TurnAction>::getCurrentTurnAction()
	return nullptr; //_currentSequence->getCurrentTurnAction();
}

bool TurnActionSequenceManager::hasActiveTurnAction() const
{
	return getCurrentTurnAction() != nullptr;
}

void TurnActionSequenceManager::toggleDebugShowTaskActionNotes()
{
	_debugShowTaskActionNotes = !_debugShowTaskActionNotes;
}

void TurnActionSequenceManager::toggleDebugShowTreeOnNewAction()
{
	_debugShowTreeOnNewAction = !_debugShowTreeOnNewAction;
}
