#pragma once

/*
UNO

TurnActionEffect enum:
All the possible effects that can be handled via the TurnActionSequence in individual TurnAction objects.

@author Peter Mitchell
@version 2022.1
*/
enum TurnActionEffect { Nothing, DrawCard, PlaceCard, MoveNextTurn, IncreaseDrawCountBy2, IncreaseDrawCountBy4, DrawNCards,
						IsCardPlayable, BeginChoiceOverlay, CheckDrawTillCanPlayRule, HasPlus2AndResponseAllowed,
						ShowSkip, TogglePlayDirection, SetTopPileColour, CheckCouldPlayCard, Draw4ChallengeSuccess,
						MovePrevious, SwapHandWithOther, PassAllHands, ShowChallengeResult, CheckNoBluffingRule,
						CheckForcedPlayRule, PlayCardAsActionFromData, DrawCardAsActionFromData};