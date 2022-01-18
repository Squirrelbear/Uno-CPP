#pragma once

#include "RuleSet.h"
#include "Player.h"
#include "Deck.h"

struct GameStateData {
	RuleSet* ruleSet;
	std::vector<Player*>* players;
	Player* bottomPlayer;
	bool* isIncreasing;
	Deck* deck;
	int* currentPlayerID;
};