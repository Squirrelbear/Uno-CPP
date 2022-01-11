#pragma once

#include "TurnDecisionAction.h"

class TurnDecisionOverlayInterface {
public:
	virtual void showOverlay(TurnDecisionAction* currentAction) = 0;
};