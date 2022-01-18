#pragma once

#include "RuleSet.h"
#include "Player.h"
#include "Deck.h"

/*
UNO

GameStateData struct:
Used as a shared reference for passing game state around to objects that need it.

@author: Peter Mitchell
@version 2022.1
*/
struct GameStateData {
	RuleSet* ruleSet;
	std::vector<Player*>* players;
	Player* bottomPlayer;
	bool* isIncreasing;
	Deck* deck;
	int* currentPlayerID;
};