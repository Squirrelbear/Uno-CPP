#include "TurnActionSequence.h"
#include "Game.h"
#include "TurnAction.h"
#include "TurnActionFactory.h"
#include <algorithm>
#include "TurnActionSequenceUtility.h"
#include <iostream>
#include <string>

template<class T>
TurnActionSequence<T>::TurnActionSequence()
{
	_currentAction = nullptr;
	_ruleSet = Game::getCurrentGame()->getRuleSet();
}

TurnActionSequence<TurnAction>::TurnActionSequence()
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

template<class T>
void TurnActionSequence<T>::iterateSequence()
{
	return nullptr;
}

void TurnActionSequence<TurnAction>::iterateSequence()
{
	TurnActionEffect effect = _currentAction->getTurnAction();
	resolveEffect(effect);
	_currentAction = _currentAction->getNext();
}

template<class T>
T * TurnActionSequence<T>::getCurrentAction()
{
	return _currentAction;
}

TurnAction * TurnActionSequence<TurnAction>::getCurrentAction()
{
	return _currentAction;
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
	// Do nothing for generic case.
	//debugRecursiveNodeOutput(dynamic_cast<TurnAction*>(_currentAction), 0);
}

void TurnActionSequence<TurnAction>::debugOutputTurnActionTree()
{
	debugRecursiveNodeOutput(dynamic_cast<TurnAction*>(_currentAction), 0);
}

template<class T>
void TurnActionSequence<T>::resolveEffect(TurnActionEffect effect)
{
	switch (effect) {
		case TurnActionEffect::BeginChoiceOverlay:
			beginChoiceOverlay();
			break;
		case TurnActionEffect::CheckCouldPlayCard:
			checkCouldPlayCard();
			break;
		case TurnActionEffect::CheckDrawTillCanPlayRule:
			checkDrawTillCanPlayRule();
			break;
		case TurnActionEffect::CheckForcedPlayRule:
			checkForcedPlayRule();
			break;
		case TurnActionEffect::CheckNoBluffingRule:
			checkNoBluffingRule();
			break;
		case TurnActionEffect::Draw4ChallengeSuccess:
			draw4ChallengeSuccess();
			break;
		case TurnActionEffect::DrawCard:
			drawCard();
			break;
		case TurnActionEffect::DrawCardAsActionFromData:
			drawCardAsActionFromData();
			break;
		case TurnActionEffect::DrawNCards:
			drawNCards();
			break;
		case TurnActionEffect::HasPlus2AndResponseAllowed:
			hasPlus2AndResponseAllowed();
			break;
		case TurnActionEffect::IncreaseDrawCountBy2:
			increaseDrawCountBy2();
			break;
		case TurnActionEffect::IncreaseDrawCountBy4:
			increaseDrawCountBy4();
			break;
		case TurnActionEffect::IsCardPlayable:
			isCardPlayable();
			break;
		case TurnActionEffect::MoveNextTurn:
			moveNextTurn();
			break;
		case TurnActionEffect::MovePrevious:
			movePrevious();
			break;
		case TurnActionEffect::PassAllHands:
			passAllHands();
			break;
		case TurnActionEffect::PlaceCard:
			placeCard();
			break;
		case TurnActionEffect::PlayCardAsActionFromData:
			playCardAsActionFromData();
			break;
		case TurnActionEffect::SetTopPileColour:
			setTopPileColour();
			break;
		case TurnActionEffect::ShowChallengeResult:
			showChallengeResult();
			break;
		case TurnActionEffect::ShowSkip:
			showSkip();
			break;
		case TurnActionEffect::SwapHandWithOther:
			swapHandWithOther();
			break;
		case TurnActionEffect::TogglePlayDirection:
			togglePlayDirection();
			break;
		case TurnActionEffect::Nothing:
			// Do nothing...
			break;
		default:
			std::cerr << "ERROR! Unknown TurnActionEffect missing function definition." << std::endl;
			break;
	}
}

template<class T>
void TurnActionSequence<T>::playCardAsActionFromData()
{
	TurnActionSequence<TurnAction>* playCard = TurnActionFactory::playCardAsAction(getPropertyValue("playerID"), getPropertyValue("cardID"),
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
	Deck* deck = Game::getCurrentGame()->getDeck();
	Card* drawnCard = deck->drawCard();
	// store ID into storedData
	injectProperty("cardID", drawnCard->getUniqueCardID());
	injectProperty("faceValueID", drawnCard->getFaceValueID());
	injectProperty("colourID", drawnCard->getColourID());
	// Add card to hand
	Game::getCurrentGame()->getCurrentPlayer()->addCardToHand(drawnCard);
}

template<class T>
void TurnActionSequence<T>::placeCard()
{
	// Get card from hand
	Player* currentPlayer = Game::getCurrentGame()->getCurrentPlayer();
	// Remove card from hand
	Card* removedCard = currentPlayer->removeCard(getPropertyValue("cardID"));
	// Add card to pile
	Game::getCurrentGame()->getRecentCardPile()->placeCard(removedCard);
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
		auto overlayConfig = "DrawN" + std::to_string(Game::getCurrentGame()->getCurrentPlayer()->getPlayerID())
							+ ";" + std::to_string(count);
		Game::getCurrentGame()->showGeneralOverlay(overlayConfig);
		injectProperty("drawCount", -1);
	}
}

template<class T>
void TurnActionSequence<T>::isCardPlayable()
{
	Card* latestCard = Game::getCurrentGame()->getRecentCardPile()->getTopCard();
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
	auto hand = Game::getCurrentGame()->getCurrentPlayer()->getHand();
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
	Game::getCurrentGame()->showGeneralOverlay("SkipVisual"	+ std::to_string(Game::getCurrentGame()->getCurrentPlayer()->getPlayerID()));
}

template<class T>
void TurnActionSequence<T>::togglePlayDirection()
{
	Game::getCurrentGame()->toggleTurnDirection();
}

template<class T>
void TurnActionSequence<T>::setTopPileColour()
{
	Game::getCurrentGame()->getRecentCardPile()->setTopCardColour(getPropertyValue("colourID"));
}

template<class T>
void TurnActionSequence<T>::checkCouldPlayCard()
{
	Card* cardBeforeLast = Game::getCurrentGame()->getRecentCardPile()->getCardBelowTop();
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
	Game::getCurrentGame()->showGeneralOverlay("DrawN" + std::to_string(Game::getCurrentGame()->getCurrentPlayer()->getPlayerID()) + ";4");
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
	int targetPlayerID = getPropertyValue("otherPlayer");
	Player* targetPlayer = Game::getCurrentGame()->getPlayerByID(targetPlayerID);
	auto targetPlayerHand = targetPlayer->takeAllHand();
	Player* currentPlayer = Game::getCurrentGame()->getCurrentPlayer();
	auto currentPlayerHand = currentPlayer->takeAllHand();

	for (auto card : targetPlayerHand) {
		currentPlayer->addCardToHand(card);
	}
	for (auto card : currentPlayerHand) {
		targetPlayer->addCardToHand(card);
	}
}

template<class T>
void TurnActionSequence<T>::passAllHands()
{
	std::vector<std::vector<Card*>> hands;
    std::vector<Player*> players = Game::getCurrentGame()->getAllPlayers();
    for(auto player : players) {
		auto hand = player->takeAllHand();
        hands.emplace_back(hand);
    }

    // Shuffle the hands
    if(Game::getCurrentGame()->isIncreasing()) {
        auto movedHand = hands.at(0);
        hands.erase(hands.begin());
        hands.emplace_back(movedHand);
    } else {
        auto movedHand = hands.at(hands.size()-1);
        hands.pop_back();
        hands.insert(hands.begin(), movedHand);
    }

    // put all the cards into the hands again
    for(int playerID = 0; playerID < players.size(); playerID++) {
        for(auto card : hands.at(playerID)) {
            players.at(playerID)->addCardToHand(card);
        }
    }
}

template<class T>
void TurnActionSequence<T>::showChallengeResult()
{
	if (getPropertyValue("couldPreviousPlayCard") == 0) {
		Game::getCurrentGame()->showGeneralOverlay("ChallengeFailed" + std::to_string(Game::getCurrentGame()->getCurrentPlayer()->getPlayerID()));
	}
	else {
		Game::getCurrentGame()->showGeneralOverlay("ChallengeSuccess" + std::to_string(Game::getCurrentGame()->getCurrentPlayer()->getPlayerID()));
	}
}

template<class T>
void TurnActionSequence<T>::checkNoBluffingRule()
{
	bool canStack = _ruleSet->canStackCards();
	auto hand = Game::getCurrentGame()->getCurrentPlayer()->getHand();
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

	TurnDecisionAction* decisionAction = dynamic_cast<TurnDecisionAction*>(actionToAdd);
	if (decisionAction != nullptr) {
		loadAllPointers(decisionAction->getOtherNextPointer());
	}
	loadAllPointers(actionToAdd->getNextPointer());
}

