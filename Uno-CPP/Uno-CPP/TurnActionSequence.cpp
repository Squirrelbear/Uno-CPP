#include "TurnActionSequence.h"
#include "Game.h"
#include "TurnAction.h"
#include "TurnActionFactory.h"
#include <algorithm>
#include "TurnActionSequenceUtility.h"

template<class T>
TurnActionSequence<T>::TurnActionSequence()
{
	_currentAction = nullptr;
	_ruleSet = Game::getCurrentGame()->getRuleSet();
}

template<class T>
TurnActionSequence<T>::~TurnActionSequence()
{
	flushTurnActions();
}

template<class T>
void TurnActionSequence<T>::setStartOfSequence(T * startAction)
{
	// Do nothing for generic case
}

void TurnActionSequence<TurnAction>::setStartOfSequence(TurnAction * startAction)
{
	// This should not happen, but if the method has been previously called force the sequence to dump everything.
	if (!_unorderedSequence.empty()) {
		flushTurnActions();
	}

	// Set the currentAction to the start and parse the tree to find all pointers for ownership
	_currentAction = startAction;
	loadAllPointers(_currentAction);
}

template<class T>
void TurnActionSequence<T>::injectProperty(const std::string & key, const int value)
{
	_storedData.insert_or_assign(key, value);
}

void TurnActionSequence<TurnAction>::injectProperty(const std::string & key, const int value)
{
	_storedData.insert_or_assign(key, value);
}

template<class T>
int TurnActionSequence<T>::getPropertyValue(const std::string & key) const
{
	if (_storedData.find(key) == _storedData.end()) {
		return -1;
	}

	return _storedData.at(key);
}

int TurnActionSequence<TurnAction>::getPropertyValue(const std::string & key) const
{
	if (_storedData.find(key) == _storedData.end()) {
		return -1;
	}

	return _storedData.at(key);
}

template<class T>
RuleSet * TurnActionSequence<T>::getRuleSet()
{
	return _ruleSet;
}



template<class T>
void TurnActionSequence<T>::debugOutputTurnActionTree()
{
	debugRecursiveNodeOutput(dynamic_cast<TurnAction*>(_currentAction), 0);
}

template<class T>
void TurnActionSequence<T>::playCardAsActionFromData()
{
	TurnActionSequence<TurnAction>* playCard = playCardAsAction(getPropertyValue("playerID"), getPropertyValue("cardID"),
		getPropertyValue("faceValueID"), getPropertyValue("colourID"));
	playCard->injectProperty("drawCount", getPropertyValue("drawCount"));
	Game::getCurrentGame()->setCurrentTurnAction(playCard);
}

template<class T>
void TurnActionSequence<T>::drawCardAsActionFromData()
{
	TurnActionSequence<TurnAction>* drawCardSequence = TurnActionFactory::drawCardAsAction(getPropertyValue("playerID"));
	Game::getCurrentGame()->setCurrentTurnAction(drawCardSequence);
}

template<class T>
void TurnActionSequence<T>::drawCard()
{
	// Draw card from deck
	Deck deck = Game::getCurrentGame()->getDeck();
	Card* drawnCard = deck.drawCard();
	// store ID into storedData
	injectProperty("cardID", drawnCard.getCardID());
	injectProperty("faceValueID", drawnCard.getFaceValueID());
	injectProperty("colourID", drawnCard.getColourID());
	// Add card to hand
	Game::getCurrentGame()->getCurrentPlayer()->addCardToHand(drawnCard);
}

template<class T>
void TurnActionSequence<T>::placeCard()
{
	// Get card from hand
	Player* currentPlayer = Game::getCurrentGame()->getCurrentPlayer();
	Card* cardToPlace = currentPlayer.getCardByID(getPropertyValue("cardID"));
	// Remove card from hand
	currentPlayer->removeCard(cardToPlace);
	// Add card to pile
	Game::getCurrentGame()->placeCard(cardToPlace);
}

template<class T>
void TurnActionSequence<T>::moveNextTurn()
{
	Game::getCurrentGame()->moveToNextPlayer();
}

template<class T>
void TurnActionSequence<T>::increaseDrawCountBy2()
{
	increaseDrawCountByN(2);
}

template<class T>
void TurnActionSequence<T>::increaseDrawCountBy4()
{
	increaseDrawCountByN(4);
}

template<class T>
void TurnActionSequence<T>::increaseDrawCountByN(const int N)
{
	int result = N;
	if (getPropertyValue("drawCount") != -1) {
		result += getPropertyValue("drawCount");
	}
	injectProperty("drawCount", result);
}

template<class T>
void TurnActionSequence<T>::drawNCards()
{
	int count = getPropertyValue("drawCount");
	if (count > 0) {
		for (int i = 0; i < count; i++) {
			drawCard();
		}
		Game::getCurrentGame()->showGeneralOverlay(
			"DrawN" + Game::getCurrentGame()->getCurrentPlayer()->getPlayerID()
			+ ";" + count);
		injectProperty("drawCount", -1);
	}
}

template<class T>
void TurnActionSequence<T>::isCardPlayable()
{
	Card* latestCard = Game::getCurrentGame()->getTopCard();
	bool isPlayable = getPropertyValue("faceValueID") == latestCard->getFaceValueID()
		|| getPropertyValue("colourID") == latestCard->getColourID()
		|| getPropertyValue("faceValueID") >= 13;
	injectProperty("cardPlayable", isPlayable ? 1 : 0);
}

template<class T>
void TurnActionSequence<T>::beginChoiceOverlay()
{
	Game::getCurrentGame()->showOverlayForTurnAction();
}

template<class T>
void TurnActionSequence<T>::checkDrawTillCanPlayRule()
{
	injectProperty("drawTillCanPlay?", _ruleSet->shouldDrawnTillCanPlay() ? 1 : 0);
}

template<class T>
void TurnActionSequence<T>::hasPlus2AndResponseAllowed()
{
	auto& hand = Game::getCurrentGame()->getCurrentPlayer()->getHand();
	if (_ruleSet->canStackCards() &&		
		std::find_if(hand.begin(), hand.end(), [](Card* card) { return card->getFaceValueID() == 10; }) != hand.end()) {
		injectProperty("hasPlus2AndResponseAllowed", 1);
	}
	else {
		injectProperty("hasPlus2AndResponseAllowed", 0);
	}
}

template<class T>
void TurnActionSequence<T>::showSkip()
{
	Game::getCurrentGame()->showGeneralOverlay("SkipVisual"	+ Game::getCurrentGame()->getCurrentPlayer()->getPlayerID());
}

template<class T>
void TurnActionSequence<T>::togglePlayDirection()
{
	Game::getCurrentGame()->toggleTurnDirection();
}

template<class T>
void TurnActionSequence<T>::setTopPileColour()
{
	Game::getCurrentGame()->setTopCardColour(getPropertyValue("colourID"));
}

template<class T>
void TurnActionSequence<T>::checkCouldPlayCard()
{
	auto& recentCards = Game::getCurrentGame()->getRecentCards();
	Card* cardBeforeLast = recentCards.at(recentCards.size() - 2);
	std::vector<Card*> validMoves = Game::getCurrentGame()->getCurrentPlayer()->getValidMoves(
		cardBeforeLast->getFaceValueID(), cardBeforeLast->getColourID());
	for (auto card : validMoves) {
		if (card->getFaceValueID() < 13) {
			injectProperty("couldPreviousPlayCard", 1);
			return;
		}
	}
	injectProperty("couldPreviousPlayCard", 0);
}

template<class T>
void TurnActionSequence<T>::draw4ChallengeSuccess()
{
	for (int i = 0; i < 4; i++) {
		drawCard();
	}
	Game::getCurrentGame()->showGeneralOverlay("DrawN" + Game::getCurrentGame()->getCurrentPlayer()->getPlayerID() + ";4");
}

template<class T>
void TurnActionSequence<T>::movePrevious()
{
	togglePlayDirection();
	moveNextTurn();
	togglePlayDirection();
}

template<class T>
void TurnActionSequence<T>::swapHandWithOther()
{
	// TODO Need to change logic for swapping vectors.
	int targetPlayerID = getPropertyValue("otherPlayer");
	Player* targetPlayer = Game::getCurrentGame()->getPlayerByID(targetPlayerID);
	auto& targetPlayerHand = targetPlayer->getHand();
	//targetPlayer.emptyHand();
	Player currentPlayer = Game::getCurrentGame()->getCurrentPlayer();
	auto& currentPlayerHand = currentPlayer->getHand();
	//currentPlayer.emptyHand();
	/*for (Object card : targetPlayerHand) {
		currentPlayer.addCardToHand((Card)card);
	}
	for (Object card : currentPlayerHand) {
		targetPlayer.addCardToHand((Card)card);
	}*/
}

template<class T>
void TurnActionSequence<T>::passAllHands()
{
	// TODO Need to change logic for swapping hands.
	/*
	        List<Object[]> hands = new ArrayList<>();
        List<Player> players = CurrentGameInterface.getCurrentGame().getAllPlayers();
        for(Player player : players) {
            hands.add(player.getHand().toArray());
            player.emptyHand();
        }

        // Shuffle the hands
        if(CurrentGameInterface.getCurrentGame().isIncreasing()) {
            Object[] movedHand = hands.get(0);
            hands.remove(0);
            hands.add(movedHand);
        } else {
            Object[] movedHand = hands.get(hands.size()-1);
            hands.remove(hands.size()-1);
            hands.add(0, movedHand);
        }

        // put all the cards into the hands again
        for(int playerID = 0; playerID < players.size(); playerID++) {
            for(Object card : hands.get(playerID)) {
                players.get(playerID).addCardToHand((Card)card);
            }
        }
	*/
}

template<class T>
void TurnActionSequence<T>::showChallengeResult()
{
	if (getPropertyValue("couldPreviousPlayCard") == 0) {
		Game::getCurrentGame()->showGeneralOverlay("ChallengeFailed" + Game::getCurrentGame()->getCurrentPlayer()->getPlayerID());
	}
	else {
		Game::getCurrentGame()->showGeneralOverlay("ChallengeSuccess" + Game::getCurrentGame()->getCurrentPlayer()->getPlayerID());
	}
}

template<class T>
void TurnActionSequence<T>::checkNoBluffingRule()
{
	bool canStack = _ruleSet->canStackCards();
	auto& hand = Game::getCurrentGame()->getCurrentPlayer().getHand();
	bool hasAPlus4 = std::find_if(hand.begin(), hand.end(), [](Card* card) { return card->getFaceValueID() == 13; }) != hand.end();
	bool canBluff = !_ruleSet->getNoBluffingRule();

	bool canChallenge = canBluff || (canStack && hasAPlus4);

	injectProperty("canChallenge", canChallenge ? 1 : 0);
}

template<class T>
void TurnActionSequence<T>::checkForcedPlayRule()
{
	injectProperty("isForcedPlay", _ruleSet->getForcedPlayRule() ? 1 : 0);
}

template<class T>
void TurnActionSequence<T>::flushTurnActions()
{
	for (auto p : _unorderedSequence) {
		delete p;
	}
	_unorderedSequence.clear();
}

template<class T>
void TurnActionSequence<T>::loadAllPointers(T * actionToAdd)
{
	if (actionToAdd == nullptr) return;

	// If the pointer is not already in the list
	if (std::find(_unorderedSequence.begin(), _unorderedSequence.end(), actionToAdd) != _unorderedSequence.end()) {
		_unorderedSequence.emplace_back(actionToAdd);
	}

	if (typeid(actionToAdd) == typeid(TurnDecisionAction*)) {
		loadAllPointers(dynamic_cast<TurnDecisionAction*>(actionToAdd)->getOtherNextPointer());
	}
	loadAllPointers(actionToAdd->getNextPointer());
}

