#include "AIPlayer.h"



AIPlayer::AIPlayer()
{
}


AIPlayer::~AIPlayer()
{
}

AIPlayer::AIPlayer(const int playerID, const std::string & playerName, const sf::IntRect bounds, const AIStrategy strategy, const bool showPlayerNameLeft)
	: Player(playerID, playerName, PlayerType::AIPlayer, bounds, showPlayerNameLeft)
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

void AIPlayer::update(const int deltaTime)
{
	updateAntiUnoCheck(deltaTime);
	updateJumpInCheck(deltaTime);

	// Do nothing more if this is not the current player.
	/*if (CurrentGameInterface.getCurrentGame().getCurrentPlayer().getPlayerID() != getPlayerID()) {
		return;
	}*/

	// Delay until
	delayTimer -= deltaTime;
	if (delayTimer <= 0) {
		resetDelayTimer();
	}
	else {
		return;
	}

	// TODO

	// If there is no turn action to deal with it means that the player is performing their regular turn
	/*if (CurrentGameInterface.getCurrentGame().getCurrentTurnAction() == nullptr) {
		performTurn();
	}
	else {
		// Handle the turn action if it is necessary
		TurnActionFactory.TurnAction currentAction = CurrentGameInterface.getCurrentGame().getCurrentTurnAction();
		if (currentAction instanceof TurnActionFactory.TurnDecisionAction) {
			TurnActionFactory.TurnDecisionAction decisionAction = (TurnActionFactory.TurnDecisionAction) currentAction;
			if (decisionAction.timeOut) {
				handleTurnDecision(decisionAction);
			}
		}
	}*/
}

void AIPlayer::updateAntiUnoCheck(const int deltaTime)
{
	/*
	        for(Player player : CurrentGameInterface.getCurrentGame().getAllPlayers()) {
            if(player != this && !player.isSafe() && player.getHand().size() == 1) {
                if(consideringPlayerID != player.getPlayerID()) {
                    consideringDelayTimer = Math.random() * 800 + 200;
                }
                consideringPlayerID = player.getPlayerID();
            }
        }
        if(consideringPlayerID == -1 || CurrentGameInterface.getCurrentGame().getPlayerByID(consideringPlayerID).isSafe()) {
            consideringPlayerID = -1;
        } else {
            consideringDelayTimer -= deltaTime;
            if(consideringDelayTimer <= 0) {
                consideringDelayTimer = Math.random() * 1200 + 300;
                if(Math.random() * 100 < 30) {
                    CurrentGameInterface.getCurrentGame().applyAntiUno(consideringPlayerID);
                }
            }
        }
	*/
}

void AIPlayer::updateJumpInCheck(const int deltaTime)
{
	/*
	if(CurrentGameInterface.getCurrentGame().getRuleSet().allowJumpInRule()
                && CurrentGameInterface.getCurrentGame().getCurrentTurnAction() == null
                && CurrentGameInterface.getCurrentGame().getCurrentPlayer() != this) {
            Card topCard = CurrentGameInterface.getCurrentGame().getTopCard();
            List<Card> validCards = getHand().stream()
                    .filter(card -> card.getFaceValueID() == topCard.getFaceValueID()
                                    && card.getColourID() == topCard.getColourID())
                    .collect(Collectors.toList());
            if(!validCards.isEmpty()) {
                if(!canJumpIn) {
                    consideringJumpIn = Math.random() * 100 < 80;
                    consideringJumpInTimer = Math.random() * 200 + 100;
                }
                canJumpIn = true;
            } else {
                canJumpIn = false;
                consideringJumpIn = false;
            }
        } else {
            canJumpIn = false;
            consideringJumpIn = false;
        }

        if(consideringJumpIn) {
            consideringJumpInTimer -= deltaTime;
            if(consideringJumpInTimer <= 0) {
                Card topCard = CurrentGameInterface.getCurrentGame().getTopCard();
                List<Card> validCards = getHand().stream()
                        .filter(card -> card.getFaceValueID() == topCard.getFaceValueID()
                                && card.getColourID() == topCard.getColourID())
                        .collect(Collectors.toList());
                if(!validCards.isEmpty()) {
                    CurrentGameInterface.getCurrentGame().jumpIn(getPlayerID(), validCards.get(0));
                }
            }
        }
	*/
}

void AIPlayer::performTurn()
{
	/*
	        Card topCard = CurrentGameInterface.getCurrentGame().getTopCard();
        List<Card> validMoves = getValidMoves(topCard.getFaceValueID(), topCard.getColourID());
        if(validMoves.isEmpty()) {
            CurrentGameInterface.getCurrentGame().setCurrentTurnAction(TurnActionFactory.drawCardAsAction(getPlayerID()));
        } else {
            Card cardToPlay = chooseCard(validMoves);
            checkCallUNO();
            CurrentGameInterface.getCurrentGame().setCurrentTurnAction(TurnActionFactory.playCardAsAction(
                    getPlayerID(), cardToPlay.getCardID(), cardToPlay.getFaceValueID(), cardToPlay.getColourID()));
        }
	*/
}

void AIPlayer::resetDelayTimer()
{
	_delayTimer = 1500;
}

Card * AIPlayer::chooseCard(const std::vector<Card*>& validCards)
{
	if (_strategy == AIStrategy::Chaotic) {
		return validCards.at(0); // TODO (int)(Math.random()*validCards.size()));
	}

	// TODO
	// validCards.sort(Comparator.comparingInt(Card::getScoreValue));

	if (_strategy == AIStrategy::Defensive) {
		return validCards.at(validCards.size() - 1);
	}
	else { // Offensive
		return validCards.at(0);
	}
}
