#pragma once

#include "TurnDecisionAction.h"

/**
 * Uno
 *
 * TurnDecisionOverlayInterface interface:
 * Defines an interface to be used for defining overlays that can be
 * made to appear using a TurnDecisionAction.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class TurnDecisionOverlayInterface {
public:
	/**
	 * Show the overlay.
	 *
	 * @param currentAction The action used to trigger this interface.
	 */
	virtual void showOverlay(TurnDecisionAction* currentAction) = 0;
};