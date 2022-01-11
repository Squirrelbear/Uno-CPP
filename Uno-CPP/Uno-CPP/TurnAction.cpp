#include "TurnAction.h"

TurnAction::TurnAction(TurnAction * next, const TurnActionSequence<TurnAction> & parentSequence, const int actionID, const std::string & actionDebugText)
	: _parent(parentSequence), _next(next), _actionID(actionID), _actionDebugText(actionDebugText)
{
}

void TurnAction::performAction()
{
	if (_actionID != -1) {
		// TODO
	}
}

TurnAction * TurnAction::getNext()
{
	return _next;
}

void TurnAction::injectProperty(const std::string & key, const int value)
{
	// TODO
}
