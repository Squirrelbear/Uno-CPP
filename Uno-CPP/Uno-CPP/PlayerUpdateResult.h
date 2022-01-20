#pragma once

#include "TurnAction.h"

enum PlayerUpdateResultState { PlayerDidNothing, PlayerStartedTurnAction, PlayerJumpedIn, PlayerCalledAntiUno, PlayerCalledUno };

/*
Uno

PlayerUpdateResult struct:
Defines details about the result of a player updating. 

@author: Peter Mitchell
@version 2022.1
*/
struct PlayerUpdateResult {
	// Defines the type of result to expect.
	PlayerUpdateResultState resultState;
	
	// Only set if resultState is PlayerStartedTurnAction 
	TurnActionSequence<TurnAction>* turnActionRequest;

	// Only set if PlayerJumpedIn, PlayerCalledUno and PlayerCalledAntiUno to designate the target.
	int playerIDForResult;

	// Only set if the state is set to PlayerJumpedIn
	Card* cardForJump;
};