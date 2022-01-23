#include "AIPlayer.h"
#include <algorithm>
#include "TurnActionFactory.h"

AIPlayer::AIPlayer(const int playerID, const std::string & playerName, const sf::IntRect bounds, const AIStrategy strategy, const bool showPlayerNameLeft, std::default_random_engine& randomEngine, const sf::Font& font)
	: Player(playerID, playerName, PlayerType::AIPlayer, bounds, showPlayerNameLeft, font), _randomEngine(randomEngine)
{
	if (strategy == AIStrategy::Random) {
		selectRandomStrategy();
	}
	else {
		_strategy = strategy;
	}
	resetDelayTimer();
	_consideringDelayTimer = -1;
}

PlayerUpdateResult AIPlayer::update(const float deltaTime, const Player* currentPlayer, TurnAction* currentTurnAction, const RecentCardPile* recentCards, const std::vector<Player*>& players, const RuleSet* rules)
{
	PlayerUpdateResult result = updateAntiUnoCheck(deltaTime, players);
	if (result.resultState != PlayerUpdateResultState::PlayerDidNothing) {
		return result;
	}
	result = updateJumpInCheck(deltaTime, rules, currentTurnAction, currentPlayer, recentCards);
	if (result.resultState != PlayerUpdateResultState::PlayerDidNothing) {
		return result;
	}

	// Do nothing more if this is not the current player.
	if (currentPlayer != this) {
		// Result will already contain PlayerDidNothing due to the checks above.
		return result;
	}

	// Delay until
	_delayTimer -= deltaTime;
	if (_delayTimer <= 0) {
		resetDelayTimer();
	}
	else {
		return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
	}

	// If there is no turn action to deal with it means that the player is performing their regular turn
	if (currentTurnAction == nullptr) {
		Card* topCard = recentCards->getTopCard();
		auto action = performTurn(topCard);
		// Handle special case where the player needs to call UNO simultaneously with playing their card.
		PlayerUpdateResultState state = action.second.resultState == PlayerUpdateResultState::PlayerCalledUno ? PlayerUpdateResultState::PlayerStartedTurnActionWithUno : PlayerUpdateResultState::PlayerStartedTurnAction;
		return {state, action.first, action.second.playerIDForResult, nullptr };
	}
	else {
		// Handle the turn action if it is necessary
		TurnDecisionAction* decisionAction = dynamic_cast<TurnDecisionAction*>(currentTurnAction);
		if (decisionAction != nullptr) {
			if (decisionAction->getTimeOut()) {
				return handleTurnDecision(decisionAction, players, rules);
			}
		}
	}
	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

std::string AIPlayer::aiStrategyToString(const AIStrategy strategy)
{
	switch (strategy) {
		case AIStrategy::Chaotic: return "Chaotic";
		case AIStrategy::Defensive: return "Defensive";
		case AIStrategy::Offensive: return "Offensive";
		case AIStrategy::Random: return "Random";
	}
	return "ERROR";
}

void AIPlayer::selectRandomStrategy()
{
	switch (_randomEngine() % 3 == 0) {
		case 0: _strategy = AIPlayer::AIStrategy::Offensive; break;
		case 1: _strategy = AIPlayer::AIStrategy::Defensive; break;
		case 2: _strategy = AIPlayer::AIStrategy::Chaotic; break;
	}
}

PlayerUpdateResult AIPlayer::updateAntiUnoCheck(const int deltaTime, const std::vector<Player*>& players)
{
	for(auto player : players) {
        if(player != this && !player->isSafe() && player->getHand().size() == 1) {
            if(_consideringPlayerID != player->getPlayerID()) {
                _consideringDelayTimer = (_randomEngine() % 800 + 200) / 1000.0f;
            }
            _consideringPlayerID = player->getPlayerID();
        }
    }
    if(_consideringPlayerID == -1 || players.at(_consideringPlayerID)->isSafe()) {
        _consideringPlayerID = -1;
    } else {
        _consideringDelayTimer -= deltaTime;
        if(_consideringDelayTimer <= 0) {
            _consideringDelayTimer = (_randomEngine() % 1200 + 300) / 1000.0f;
            if(_randomEngine() % 100 < 30) {
				return { PlayerUpdateResultState::PlayerCalledAntiUno, nullptr, _consideringPlayerID, nullptr };
            }
        }
    }
	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

PlayerUpdateResult AIPlayer::updateJumpInCheck(const int deltaTime, const RuleSet* rules, const TurnAction* currentTurnAction, const Player* currentPlayer, const RecentCardPile* recentCards)
{
	if(rules->allowJumpInRule()
			&& currentTurnAction == nullptr
			&& currentPlayer != this) {
		Card* topCard = recentCards->getTopCard();
		std::vector<Card*> validCards;
		for (auto card : getHand()) {
			if (card->getFaceValueID() == topCard->getFaceValueID()
				&& card->getColourID() == topCard->getColourID()) {
				validCards.emplace_back(card);
			}
		}

		if(!validCards.empty()) {
			if(!_canJumpIn) {
				_consideringJumpIn = _randomEngine() % 100 < 80;
				_consideringJumpInTimer = (_randomEngine() % 200 + 100) / 1000.0f;
			}
			_canJumpIn = true;
		} else {
			_canJumpIn = false;
			_consideringJumpIn = false;
		}
	} else {
		_canJumpIn = false;
		_consideringJumpIn = false;
	}

	if(_consideringJumpIn) {
		_consideringJumpInTimer -= deltaTime;
		if(_consideringJumpInTimer <= 0) {
			Card* topCard = recentCards->getTopCard();
			for (auto card : getHand()) {
				if (card->getFaceValueID() == topCard->getFaceValueID()
					&& card->getColourID() == topCard->getColourID()) {
					return { PlayerUpdateResultState::PlayerJumpedIn, nullptr, getPlayerID(), card };
				}
			}
		}
	}
	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

std::pair<TurnActionSequence<TurnAction>*, PlayerUpdateResult> AIPlayer::performTurn(Card* topCard)
{
    std::vector<Card*> validMoves = getValidMoves(topCard->getFaceValueID(), topCard->getColourID());
    if(validMoves.empty()) {
		return std::make_pair(TurnActionFactory::drawCardAsAction(getPlayerID()), PlayerUpdateResult{ PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr });
    } else {
        Card* cardToPlay = chooseCard(validMoves);
		PlayerUpdateResult result = checkCallUNO();
        return std::make_pair(TurnActionFactory::playCardAsAction(getPlayerID(), cardToPlay->getUniqueCardID(), cardToPlay->getFaceValueID(), cardToPlay->getColourID()), result);
    }
}

void AIPlayer::resetDelayTimer()
{
	_delayTimer = 1.5f;
}

Card * AIPlayer::chooseCard(std::vector<Card*>& validCards)
{
	if (_strategy == AIStrategy::Chaotic) {
		return validCards.at(_randomEngine() % validCards.size());
	}

	if (_strategy == AIStrategy::Defensive) {
		return *std::max_element(validCards.begin(), validCards.end(), [](Card* a, Card* b) { return a->getScoreValue() > b->getScoreValue(); });
	}
	else { // Offensive
		return *std::max_element(validCards.begin(), validCards.end(), [](Card* a, Card* b) { return a->getScoreValue() < b->getScoreValue(); });
	}
}

PlayerUpdateResult AIPlayer::handleTurnDecision(TurnDecisionAction * decisionAction, const std::vector<Player*>& players, const RuleSet* rules)
{
	if (decisionAction->getFlagName() == "wildColour") {
		return chooseWildColour(decisionAction);
	}
	else if (decisionAction->getFlagName() == "keepOrPlay") {
		return chooseKeepOrPlay(decisionAction);
	}
	else if (decisionAction->getFlagName() == "otherPlayer") {
		return choosePlayerToSwapWith(decisionAction, players);
	}
	else if (decisionAction->getFlagName() == "isChallenging") {
		return chooseChallengeOrDecline(decisionAction, rules);
	}
	else if (decisionAction->getFlagName() == "isStacking") {
		return chooseStackPlus2(decisionAction, rules);
	}

	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

PlayerUpdateResult AIPlayer::chooseWildColour(TurnDecisionAction * decisionAction)
{
	std::vector<Card*> colouredHandCards;
	for (auto card : getHand()) {
		if (card->getColourID() != 4) {
			colouredHandCards.emplace_back(card);
		}
	}

	// No cards, or only wilds, or rare 10% chance: randomly choose colour
	if (colouredHandCards.empty() || _randomEngine() % 100 > 90) {
		decisionAction->injectProperty("colourID", _randomEngine() % 4);
	}
	else { // Use first coloured card
		decisionAction->injectProperty("colourID", colouredHandCards.at(0)->getColourID());
	}
	decisionAction->injectFlagProperty(1);

	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

PlayerUpdateResult AIPlayer::chooseKeepOrPlay(TurnDecisionAction * decisionAction)
{
	PlayerUpdateResult result = checkCallUNO();
	decisionAction->injectFlagProperty(1);
	return result;
}

PlayerUpdateResult AIPlayer::choosePlayerToSwapWith(TurnDecisionAction * decisionAction, const std::vector<Player*>& players)
{
	Player* chosenPlayer = this;
	int cardCount = 9999;
	for (auto player : players) {
		if (player->getHand().size() < cardCount && player != this) {
			chosenPlayer = player;
			cardCount = chosenPlayer->getHand().size();
		}
	}
	decisionAction->injectFlagProperty(chosenPlayer->getPlayerID());

	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

PlayerUpdateResult AIPlayer::chooseChallengeOrDecline(TurnDecisionAction * decisionAction, const RuleSet* rules)
{
	// Always stack a card if it is allowed and available.
	if (rules->canStackCards()) {
		auto hand = getHand();
		auto validCard = std::find_if(hand.begin(), hand.end(), [](Card* card) { return card->getFaceValueID() == 13; });
		if (validCard != hand.end()) {
			PlayerUpdateResult result = checkCallUNO();
			decisionAction->injectProperty("faceValueID", (*validCard)->getFaceValueID());
			decisionAction->injectProperty("colourID", (*validCard)->getColourID());
			decisionAction->injectProperty("cardID", (*validCard)->getUniqueCardID());
			decisionAction->injectProperty("isChaining", 1);
			decisionAction->injectFlagProperty(0);
			return result;
		}
	}
	decisionAction->injectProperty("isChaining", 0);
	// Randomly choose 50-50 whether to challenge or decline
	// Don't need to check the no bluffing rule because this method is only called if a valid choice is available
	// And the AI will ALWAYS choose to stack a card meaning this will never run the random chance of challenge in those cases.
	decisionAction->injectFlagProperty(_randomEngine() % 2);

	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}

PlayerUpdateResult AIPlayer::chooseStackPlus2(TurnDecisionAction * decisionAction, const RuleSet* rules)
{
	if (rules->canStackCards()) {
		auto hand = getHand();
		auto validCard = std::find_if(hand.begin(), hand.end(), [](Card* card) { return card->getFaceValueID() == 10; });
		if (validCard != hand.end()) {
			PlayerUpdateResult result = checkCallUNO();
			decisionAction->injectProperty("faceValueID", (*validCard)->getFaceValueID());
			decisionAction->injectProperty("colourID", (*validCard)->getColourID());
			decisionAction->injectProperty("cardID", (*validCard)->getUniqueCardID());
			decisionAction->injectFlagProperty(1);
			return result;
		}
	}
	decisionAction->injectFlagProperty(0);
}

PlayerUpdateResult AIPlayer::checkCallUNO()
{
	if (getHand().size() != 2) return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
	if (_randomEngine() % 100 < 70) {
		setUnoState(UNOState::Called);
		return { PlayerUpdateResultState::PlayerCalledUno, nullptr, getPlayerID(), nullptr };
	}
	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}
