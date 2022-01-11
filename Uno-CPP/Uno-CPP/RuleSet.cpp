#include "RuleSet.h"

RuleSet::RuleSet()
{
	for (int i = 0; i <= 9; i++) {
		_faceValueToActionMap[i] = CardAction::Nothing;
	}
	_faceValueToActionMap[10] = CardAction::Plus2;
	_faceValueToActionMap[11] = CardAction::Skip;
	_faceValueToActionMap[12] = CardAction::Reverse;
	_faceValueToActionMap[13] = CardAction::Plus4;
	_faceValueToActionMap[14] = CardAction::Wild;
	_defaultTimeOut = 25;
	setToDefaults();
}

void RuleSet::setToDefaults()
{
	setCanStackCards(true);
	setDrawnTillCanPlay(true);
	setTwoPlayers(false);
	setSevenZeroRule(false);
	setForcedPlayRule(false);
	setAllowJumpInRule(false);
	setNoBuffingRule(false);
	setScoreLimitType(ScoreLimitType::OneRound);
}

RuleSet::CardAction RuleSet::getActionForCard(const int faceValueID) const
{
	return _faceValueToActionMap.at(faceValueID);
}

bool RuleSet::canStackCards() const
{
	return _canStackCards;
}

void RuleSet::setCanStackCards(const bool canStackCards)
{
	_canStackCards = canStackCards;
}

bool RuleSet::shouldDrawnTillCanPlay() const
{
	return _drawTillCanPlay;
}

void RuleSet::setDrawnTillCanPlay(const bool drawnTillCanPlay)
{
	_drawTillCanPlay = drawnTillCanPlay;
}

int RuleSet::getDefaultTimeOut() const
{
	return _defaultTimeOut;
}

void RuleSet::setTwoPlayers(const bool onlyTwoPlayers)
{
	_onlyTwoPlayers = onlyTwoPlayers;
	_faceValueToActionMap[12] = onlyTwoPlayers ? CardAction::Skip : CardAction::Reverse;
}

bool RuleSet::getOnlyTwoPlayers() const
{
	return _onlyTwoPlayers;
}

void RuleSet::setSevenZeroRule(const bool sevenZeroRule)
{
	_sevenZeroRule = sevenZeroRule;
	_faceValueToActionMap[0] = sevenZeroRule ? CardAction::Swap : CardAction::Nothing;
	_faceValueToActionMap[7] = sevenZeroRule ? CardAction::PassAll : CardAction::Nothing;
}

bool RuleSet::getSevenZeroRule() const
{
	return _sevenZeroRule;
}

void RuleSet::setNoBuffingRule(const bool noBluffingRule)
{
	_noBluffingRule = noBluffingRule;
}

bool RuleSet::getNoBluffingRule() const
{
	return _noBluffingRule;
}

void RuleSet::setAllowJumpInRule(const bool allowJumpInRule)
{
	_allowJumpInRule = allowJumpInRule;
}

bool RuleSet::allowJumpInRule() const
{
	return _allowJumpInRule;
}

void RuleSet::setForcedPlayRule(const bool forcedPlayRule)
{
	_forcedPlayRule = forcedPlayRule;
}

bool RuleSet::getForcedPlayRule() const
{
	return _forcedPlayRule;
}

void RuleSet::setScoreLimitType(const ScoreLimitType scoreLimitType)
{
	_scoreLimitType = scoreLimitType;
}

RuleSet::ScoreLimitType RuleSet::getScoreLimitType() const
{
	return _scoreLimitType;
}
