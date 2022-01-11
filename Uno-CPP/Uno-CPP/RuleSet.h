#pragma once

#include <map>

/**
 * Uno
 *
 * RuleSet class:
 * Refines the class that stores the active rules used to determine how cards are mapped to actions
 * and what actions are allowed.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class RuleSet
{
public:
	// Defines the different unique CardActions that can be mapped to faceValueIDs.
	enum CardAction { Nothing, Plus2, Plus4, Wild, Skip, Reverse, Swap, PassAll };
	// Score limits that determine how long the game will run for.
	enum ScoreLimitType { OneRound, Score200, Score300, Score500, Unlimited };

	// Initialises a default RuleSet.
	RuleSet();
	virtual ~RuleSet() = default;

	void setToDefaults();

	/**
	 * Looks up the CardAction that should be activated in relation to a played card.
	 *
	 * @param faceValueID The faceValue to look up in the action map.
	 * @return The mapped CardAction associated with the specified faceValueID.
	 */
	CardAction getActionForCard(const int faceValueID) const;

	/**
	 * Checks whether +2 and +4 cards can be played in response to other +2 and +4 cards.
	 *
	 * @return True if +2 and +4 cards can be stacked in response.
	 */
	bool canStackCards() const;

	/**
	 * Changes the state of card stacking.
	 *
	 * @param canStackCards When true +2 and +4 cards can be stacked in response.
	 */
	void setCanStackCards(const bool canStackCards);

	/**
	 * Checks whether cards must be drawn till one can be played.
	 *
	 * @return True if when drawing from the deck for turn cards must continue to be drawn till a playable card is found.
	 */
	bool shouldDrawnTillCanPlay() const;

	/**
	 * Changes the state of drawing to a card can be played.
	 *
	 * @param drawnTillCanPlay When true cards must be drawn until one can be played.
	 */
	void setDrawnTillCanPlay(const bool drawnTillCanPlay);

	/**
	 * Gets the time in seconds that can be spent maximum for any individual action.
	 *
	 * @return The time player's have to make their choice during actions.
	 */
	int getDefaultTimeOut() const;

	/**
	 * Sets the state of whether there are only two players.
	 *
	 * @param onlyTwoPlayers When true Reverse becomes a skip.
	 */
	void setTwoPlayers(const bool onlyTwoPlayers);

	/**
	 * Gets the current state of the two players.
	 *
	 * @return True if the only two player rules are active with Reverse set to a skip.
	 */
	bool getOnlyTwoPlayers() const;

	/**
	 * Changes the state of whether the seven-zero rule is active.
	 *
	 * @param sevenZeroRule When true 7 is a swap action and 0 is a pass all action.
	 */
	void setSevenZeroRule(const bool sevenZeroRule);

	/**
	 * Checks whether the Seven-Zero rule is active.
	 *
	 * @return When true 7 is a swap action and 0 is a pass all action.
	 */
	bool getSevenZeroRule() const;

	/**
	 * Sets the No Buffing rule.
	 *
	 * @param noBluffingRule When no bluffing is true, the +4 can not be challenged.
	 */
	void setNoBuffingRule(const bool noBluffingRule);

	/**
	 * Gets the current state of the no bluffing rule.
	 *
	 * @return When no bluffing is true, the +4 can not be challenged.
	 */
	bool getNoBluffingRule() const;

	/**
	 * Sets the current state of the allowing jump in rule.
	 *
	 * @param allowJumpInRule When true players can jump in with cards of the same face value.
	 */
	void setAllowJumpInRule(const bool allowJumpInRule);

	/**
	 * Gets the current state of the jump in rule.
	 *
	 * @return When true players can jump in with cards of the same face value.
	 */
	bool allowJumpInRule() const;

	/**
	 * Sets the current state of the forced play rule.
	 *
	 * @param forcedPlayRule When forced play is true, there should be no keep or play choice.
	 */
	void setForcedPlayRule(const bool forcedPlayRule);

	/**
	 * Gets the current state of the forced play rule.
	 *
	 * @return When forced play is true, there should be no keep or play choice.
	 */
	bool getForcedPlayRule() const;

	/**
	 * Sets the score limit to wind the rounds.
	 *
	 * @param scoreLimitType The new score limit to apply.
	 */
	void setScoreLimitType(const ScoreLimitType scoreLimitType);

	/**
	 * Gets the current score limit setting.
	 *
	 * @return The current score limit to win all the rounds.
	 */
	ScoreLimitType getScoreLimitType() const;

private:
	// 0 to 14 mapped with CardActions to represent each of the different Uno cards.
	std::map<int, CardAction> _faceValueToActionMap;
	// True if +2 and +4 cards can be stacked in response.
	bool _canStackCards;
	// True if when drawing from the deck for turn cards must continue to be drawn till a playable card is found.
	bool _drawTillCanPlay;
	// The time player's have to make their choice during actions.
	int _defaultTimeOut;
	// Used to determine if Reverse becomes a skip when true.
	bool _onlyTwoPlayers;
	// When true the 7 becomes a swap action, and 0 becomes a pass all action.
	bool _sevenZeroRule;
	// When no bluffing is true, the +4 can not be challenged.
	bool _noBluffingRule;
	// When true, players can jump in out of turn order with cards showing the same face value.
	bool _allowJumpInRule;
	// When true, there is no keep or play choice, it is forced play.
	bool _forcedPlayRule;
	// Stores the type of score limit to use for managing the end of game.
	ScoreLimitType _scoreLimitType;
};

