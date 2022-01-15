#include "TurnActionFactory.h"
#include "TurnActionEffect.h"
#include "Game.h"

TurnActionSequence<TurnAction>* TurnActionFactory::playCardAsAction(const int playerID, const int cardID, const int faceValueID, const int colourID)
{
	//TurnActionSequence<TurnAction>* nextSequence = new TurnActionSequence<TurnAction>();
	/*nextSequence->injectProperty("playerID", playerID);
	nextSequence->injectProperty("cardID", cardID);
	nextSequence->injectProperty("faceValueID", faceValueID);
	nextSequence->injectProperty("colourID", colourID);
	TurnAction* cardAction = cardIDToTurnAction(faceValueID, *nextSequence);
	TurnAction* placeCardAction = new TurnAction(cardAction, *nextSequence, TurnActionEffect::PlaceCard, "Place Card");*/
	// TODO link into sequence.
	return nullptr;// nextSequence;
}

TurnActionSequence<TurnAction>* TurnActionFactory::drawCardAsAction(const int playerID)
{
	//TurnActionSequence<TurnAction>* nextSequence = new TurnActionSequence<TurnAction>();
	/*nextSequence->injectProperty("playerID", playerID);
	TurnAction* moveToNextTurn = new TurnAction(nullptr, *nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* playCard = new TurnAction(nullptr, *nextSequence, TurnActionEffect::PlayCardAsActionFromData, "Play the DrawnCard");
	TurnDecisionAction* keepOrPlay = new TurnDecisionAction(moveToNextTurn, playCard, true, "keepOrPlay", *nextSequence, TurnActionEffect::BeginChoiceOverlay, "Keep Or Play Choice");
	TurnDecisionAction* isForcedPlay = new TurnDecisionAction(keepOrPlay, playCard, false, "isForcedPlay", *nextSequence, TurnActionEffect::CheckForcedPlayRule, "Check if the Forced Play is enabled and force the play if so.");
	TurnAction* keepDrawing = new TurnAction(nullptr, *nextSequence, TurnActionEffect::DrawCardAsActionFromData, "Draw Another Card (Recursive Tree)");
	TurnDecisionAction* drawTillCanPlay = new TurnDecisionAction(moveToNextTurn, keepDrawing, false, "drawTillCanPlay?", *nextSequence, TurnActionEffect::CheckDrawTillCanPlayRule, "Check Draw Till Can Play Rule");
	TurnDecisionAction* canPlayCard = new TurnDecisionAction(drawTillCanPlay, isForcedPlay, false, "cardPlayable", *nextSequence, TurnActionEffect::IsCardPlayable, "Check is the Card Playable");
	TurnAction* drawCard = new TurnAction(canPlayCard, *nextSequence, TurnActionEffect::DrawCard, "Draw a Card");*/
	// TODO link into squence.
	return nullptr;// nextSequence;
}

TurnAction * TurnActionFactory::playPlus2Action(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextTurn = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* dealPenalty = new TurnAction(moveToNextTurn, nextSequence, TurnActionEffect::DrawNCards, "Draw N Number Cards");
	TurnAction* playCard = new TurnAction(nullptr, nextSequence, TurnActionEffect::PlayCardAsActionFromData, "Play another +2 (Recursive)");
	TurnDecisionAction* waitForPlay2OrCancel = new TurnDecisionAction(dealPenalty, playCard, true,
		"isStacking", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Check for +2 or Cancel Choice");
	TurnDecisionAction* checkCanRespond = new TurnDecisionAction(dealPenalty, waitForPlay2OrCancel, false,
		"hasPlus2AndResponseAllowed", nextSequence, TurnActionEffect::HasPlus2AndResponseAllowed, "Can Stack and has a +2");
	TurnAction* increaseDrawCount = new TurnAction(checkCanRespond, nextSequence, TurnActionEffect::IncreaseDrawCountBy2, "Increase N (drawCount) by 2");
	return new TurnAction(increaseDrawCount, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
}

TurnAction * TurnActionFactory::playPlus4Action(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextSkipDamagedPlayer = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* drawNCards = new TurnAction(moveToNextSkipDamagedPlayer, nextSequence, TurnActionEffect::DrawNCards, "Draw N Number Cards");
	TurnAction* increaseDrawBy4 = new TurnAction(drawNCards, nextSequence, TurnActionEffect::IncreaseDrawCountBy4, "Increase N (drawCount) by 4");
	TurnAction* playCardAsResponse = new TurnAction(nullptr, nextSequence, TurnActionEffect::PlayCardAsActionFromData, "Stack +4 on Previous (Recursive)");
	TurnAction* increaseDrawBy4ThenStack = new TurnAction(playCardAsResponse, nextSequence, TurnActionEffect::IncreaseDrawCountBy4, "Increase N (drawCount) by 4");
	TurnDecisionAction* isChainingCard = new TurnDecisionAction(increaseDrawBy4, increaseDrawBy4ThenStack,
		false, "isChaining", nextSequence, TurnActionEffect::Nothing, "No Action");
	TurnAction* drawNCardsAndDoNothing = new TurnAction(nullptr, nextSequence, TurnActionEffect::DrawNCards, "Draw N Number Cards");
	TurnAction* moveBackToNext = new TurnAction(drawNCardsAndDoNothing, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* applyPenalty = new TurnAction(moveBackToNext, nextSequence, TurnActionEffect::Draw4ChallengeSuccess, "Apply penalty (+4) to Player");
	TurnAction* moveToPreviousPlayer = new TurnAction(applyPenalty, nextSequence, TurnActionEffect::MovePrevious, "Move to Previous Player");
	TurnAction* increaseDrawBy2 = new TurnAction(increaseDrawBy4, nextSequence, TurnActionEffect::IncreaseDrawCountBy2, "Increase N (drawCount) by 2");
	TurnDecisionAction* couldPreviousPlayCard = new TurnDecisionAction(increaseDrawBy2, moveToPreviousPlayer,
		false, "couldPreviousPlayCard", nextSequence, TurnActionEffect::ShowChallengeResult, "Could the Previous Player Have played a Card? (No Action)");
	TurnDecisionAction* isChallenging = new TurnDecisionAction(isChainingCard, couldPreviousPlayCard, true,
		"isChallenging", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Ask if the player wants to Challenge, Stack, or Do Nothing");
	TurnDecisionAction* canChallengeOrStack = new TurnDecisionAction(increaseDrawBy4, isChallenging, false,
		"canChallenge", nextSequence, TurnActionEffect::CheckNoBluffingRule, "Check if a Challenge is allowed or if there is a card to Stack");
	TurnAction* moveToNextTurn = new TurnAction(canChallengeOrStack, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* setTopOfPileColour = new TurnAction(moveToNextTurn, nextSequence, TurnActionEffect::SetTopPileColour, "Change the Colour on Top of Pile");
	TurnDecisionAction* chooseWildColour = new TurnDecisionAction(setTopOfPileColour, setTopOfPileColour,
		true, "wildColour", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Ask player for a Colour Choice");
	return new TurnAction(chooseWildColour, nextSequence, TurnActionEffect::CheckCouldPlayCard, "Check if a Card Could have been Played");
}

TurnAction * TurnActionFactory::playWildAction(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextTurn = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* setTopOfPileColour = new TurnAction(moveToNextTurn, nextSequence, TurnActionEffect::SetTopPileColour, "Change the Colour on Top of Pile");
	return new TurnDecisionAction(setTopOfPileColour, setTopOfPileColour,
		true, "wildColour", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Ask player for a Colour Choice");
}

TurnAction * TurnActionFactory::playSkipAction(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextTurnAtEnd = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* showSkip = new TurnAction(moveToNextTurnAtEnd, nextSequence, TurnActionEffect::ShowSkip, "Show a Skip Icon Over Player");
	return new TurnAction(showSkip, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
}

TurnAction * TurnActionFactory::playReverseAction(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextTurn = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	return new TurnAction(moveToNextTurn, nextSequence, TurnActionEffect::TogglePlayDirection, "Toggle Direction of Play");
}

TurnAction * TurnActionFactory::playSwapAction(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextTurn = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	TurnAction* swapHands = new TurnAction(moveToNextTurn, nextSequence, TurnActionEffect::SwapHandWithOther, "Swap Hands with Selected Player");
	return new TurnDecisionAction(swapHands, swapHands, true,
		"otherPlayer", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Choose Other Player to Swap With");
}

TurnAction * TurnActionFactory::playPassAllAction(TurnActionSequence<TurnAction>* nextSequence)
{
	TurnAction* moveToNextTurn = new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	return new TurnAction(moveToNextTurn, nextSequence, TurnActionEffect::PassAllHands, "Pass All Hands");
}

TurnAction * TurnActionFactory::cardIDToTurnAction(const int faceValueID, TurnActionSequence<TurnAction>* nextSequence)
{
	switch (Game::getCurrentGame()->getRuleSet()->getActionForCard(faceValueID)) {
		case RuleSet::CardAction::Plus2: return playPlus2Action(nextSequence);
		case RuleSet::CardAction::Plus4: return playPlus4Action(nextSequence);
		case RuleSet::CardAction::Wild: return playWildAction(nextSequence);
		case RuleSet::CardAction::Skip: return playSkipAction(nextSequence);
		case RuleSet::CardAction::Reverse: return playReverseAction(nextSequence);
		case RuleSet::CardAction::Swap: return playSwapAction(nextSequence);
		case RuleSet::CardAction::PassAll: return playPassAllAction(nextSequence);
		default: return new TurnAction(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	};
}
