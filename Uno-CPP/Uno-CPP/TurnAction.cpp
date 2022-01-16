#include "TurnAction.h"

TurnAction::TurnAction(TurnAction * next, TurnActionSequence<TurnAction>* parentSequence, const TurnActionEffect actionID, const std::string & actionDebugText)
	: _next(next), _actionID(actionID), _actionDebugText(actionDebugText)
{
	_parent = parentSequence;
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
	_parent->injectProperty(key, value);
}

int TurnAction::getPropertyValue(const std::string & key) const
{
	return _parent->getPropertyValue(key);
}

std::string TurnAction::getActionDebugText() const
{
	return _actionDebugText;
}

TurnAction * TurnAction::getNextPointer() const
{
	return _next;
}
