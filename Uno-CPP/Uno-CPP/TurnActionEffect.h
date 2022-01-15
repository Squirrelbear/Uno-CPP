#pragma once

enum TurnActionEffect { Nothing, DrawCard, PlaceCard, MoveNextTurn, IncreaseDrawCountBy2, IncreaseDrawCountBy4, DrawNCards,
						IsCardPlayable, BeginChoiceOverlay, CheckDrawTillCanPlayRule, HasPlus2AndResponseAllowed,
						ShowSkip, TogglePlayDirection, SetTopPileColour, CheckCouldPlayCard, Draw4ChallengeSuccess,
						MovePrevious, SwapHandWithOther, PassAllHands, ShowChallengeResult, CheckNoBluffingRule,
						CheckForcedPlayRule, PlayCardAsActionFromData, DrawCardAsActionFromData};