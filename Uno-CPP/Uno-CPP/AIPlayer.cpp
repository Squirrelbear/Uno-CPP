#include "AIPlayer.h"
#include "Game.h"
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

void AIPlayer::update(const float deltaTime)
{
	updateAntiUnoCheck(deltaTime);
	updateJumpInCheck(deltaTime);

	// Do nothing more if this is not the current player.
	if (Game::getCurrentGame()->getCurrentPlayer()->getPlayerID() != getPlayerID()) {
		return;
	}

	// Delay until
	_delayTimer -= deltaTime;
	if (_delayTimer <= 0) {
		resetDelayTimer();
	}
	else {
		return;
	}

	// If there is no turn action to deal with it means that the player is performing their regular turn
	TurnAction* currentTurnAction = Game::getCurrentGame()->getCurrentTurnAction();
	if (currentTurnAction == nullptr) {
		Card* topCard = Game::getCurrentGame()->getRecentCardPile()->getTopCard();
		Game::getCurrentGame()->setCurrentTurnAction(performTurn(topCard));
	}
	else {
		// Handle the turn action if it is necessary
		TurnDecisionAction* decisionAction = dynamic_cast<TurnDecisionAction*>(currentTurnAction);
		if (decisionAction != nullptr) {
			if (decisionAction->getTimeOut()) {
				handleTurnDecision(decisionAction);
			}
		}
	}
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

void AIPlayer::updateAntiUnoCheck(const int deltaTime)
{
	for(auto player : Game::getCurrentGame()->getAllPlayers()) {
        if(player != this && !player->isSafe() && player->getHand().size() == 1) {
            if(_consideringPlayerID != player->getPlayerID()) {
                _consideringDelayTimer = _randomEngine() % 800 + 200;
            }
            _consideringPlayerID = player->getPlayerID();
        }
    }
    if(_consideringPlayerID == -1 || Game::getCurrentGame()->getPlayerByID(_consideringPlayerID)->isSafe()) {
        _consideringPlayerID = -1;
    } else {
        _consideringDelayTimer -= deltaTime;
        if(_consideringDelayTimer <= 0) {
            _consideringDelayTimer = _randomEngine() % 1200 + 300;
            if(_randomEngine() % 100 < 30) {
                Game::getCurrentGame()->applyAntiUno(_consideringPlayerID);
            }
        }
    }
}

void AIPlayer::updateJumpInCheck(const int deltaTime)
{
	if(Game::getCurrentGame()->getRuleSet()->allowJumpInRule()
			&& Game::getCurrentGame()->getCurrentTurnAction() == nullptr
			&& Game::getCurrentGame()->getCurrentPlayer() != this) {
		Card* topCard = Game::getCurrentGame()->getRecentCardPile()->getTopCard();
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
				_consideringJumpInTimer = _randomEngine() % 200 + 100;
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
			Card* topCard = Game::getCurrentGame()->getRecentCardPile()->getTopCard();
			for (auto card : getHand()) {
				if (card->getFaceValueID() == topCard->getFaceValueID()
					&& card->getColourID() == topCard->getColourID()) {
					Game::getCurrentGame()->jumpIn(getPlayerID(), card);
					return;
				}
			}
		}
	}
}

TurnActionSequence<TurnAction>* AIPlayer::performTurn(Card* topCard)
{
    std::vector<Card*> validMoves = getValidMoves(topCard->getFaceValueID(), topCard->getColourID());
    if(validMoves.empty()) {
        return TurnActionFactory::drawCardAsAction(getPlayerID());
    } else {
        Card* cardToPlay = chooseCard(validMoves);
        checkCallUNO();
        return TurnActionFactory::playCardAsAction(getPlayerID(), cardToPlay->getUniqueCardID(), cardToPlay->getFaceValueID(), cardToPlay->getColourID());
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

void AIPlayer::handleTurnDecision(TurnDecisionAction * decisionAction)
{
	if (decisionAction->getFlagName() == "wildColour") {
		chooseWildColour(decisionAction);
	}
	else if (decisionAction->getFlagName() == "keepOrPlay") {
		chooseKeepOrPlay(decisionAction);
	}
	else if (decisionAction->getFlagName() == "otherPlayer") {
		choosePlayerToSwapWith(decisionAction);
	}
	else if (decisionAction->getFlagName() == "isChallenging") {
		chooseChallengeOrDecline(decisionAction);
	}
	else if (decisionAction->getFlagName() == "isStacking") {
		chooseStackPlus2(decisionAction);
	}
}

void AIPlayer::chooseWildColour(TurnDecisionAction * decisionAction)
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
}

void AIPlayer::chooseKeepOrPlay(TurnDecisionAction * decisionAction)
{
	checkCallUNO();
	decisionAction->injectFlagProperty(1);
}

void AIPlayer::choosePlayerToSwapWith(TurnDecisionAction * decisionAction)
{
	Player* chosenPlayer = this;
	int cardCount = 9999;
	for (auto player : Game::getCurrentGame()->getAllPlayers()) {
		if (player->getHand().size() < cardCount && player != this) {
			chosenPlayer = player;
			cardCount = chosenPlayer->getHand().size();
		}
	}
	decisionAction->injectFlagProperty(chosenPlayer->getPlayerID());
}

void AIPlayer::chooseChallengeOrDecline(TurnDecisionAction * decisionAction)
{
	// Always stack a card if it is allowed and available.
	if (Game::getCurrentGame()->getRuleSet()->canStackCards()) {
		auto hand = getHand();
		auto validCard = std::find_if(hand.begin(), hand.end(), [](Card* card) { return card->getFaceValueID() == 13; });
		if (validCard != hand.end()) {
			checkCallUNO();
			decisionAction->injectProperty("faceValueID", (*validCard)->getFaceValueID());
			decisionAction->injectProperty("colourID", (*validCard)->getColourID());
			decisionAction->injectProperty("cardID", (*validCard)->getUniqueCardID());
			decisionAction->injectProperty("isChaining", 1);
			decisionAction->injectFlagProperty(0);
			return;
		}
	}
	decisionAction->injectProperty("isChaining", 0);
	// Randomly choose 50-50 whether to challenge or decline
	// Don't need to check the no bluffing rule because this method is only called if a valid choice is available
	// And the AI will ALWAYS choose to stack a card meaning this will never run the random chance of challenge in those cases.
	decisionAction->injectFlagProperty(_randomEngine() % 2);
}

void AIPlayer::chooseStackPlus2(TurnDecisionAction * decisionAction)
{
	if (Game::getCurrentGame()->getRuleSet()->canStackCards()) {
		auto hand = getHand();
		auto validCard = std::find_if(hand.begin(), hand.end(), [](Card* card) { return card->getFaceValueID() == 10; });
		if (validCard != hand.end()) {
			checkCallUNO();
			decisionAction->injectProperty("faceValueID", (*validCard)->getFaceValueID());
			decisionAction->injectProperty("colourID", (*validCard)->getColourID());
			decisionAction->injectProperty("cardID", (*validCard)->getUniqueCardID());
			decisionAction->injectFlagProperty(1);
			return;
		}
	}
	decisionAction->injectFlagProperty(0);
}

void AIPlayer::checkCallUNO()
{
	if (getHand().size() != 2) return;
	if (_randomEngine() % 100 < 70) {
		setUnoState(UNOState::Called);
		Game::getCurrentGame()->showGeneralOverlay("UNOCalled" + std::to_string(getPlayerID()));
	}
}
