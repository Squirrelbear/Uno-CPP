#pragma once

#include "WndInterface.h"
#include "LobbyPlayer.h"
#include "Button.h"
#include "RuleSet.h"
#include <map>
#include <string>

/**
 * Uno
 *
 * LobbyInterface class:
 * Shows a lobby to setup the players and ruleset ready to start a game.
 *
 * @author Peter Mitchell
 * @version 2021.1
 */
class LobbyInterface : public WndInterface
{
public:
	/**
	 * Initialise the interface with bounds and make it enabled.
	 *
	 * @param bounds Area to display the lobby in.
	 */
	LobbyInterface(const sf::IntRect& bounds, const sf::Font& font);
	virtual ~LobbyInterface();

	// Does nothing
	void update(const float deltaTime) override {}

	// Draws all the elements required for the LobbyInterface.
	void draw(sf::RenderWindow& renderWindow) const override;

	/**
	 * Does nothing if not enabled. Checks for presses on the buttons and players
	 * with methods to handle the interactions as necessary.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * updates the hover status of elements. Does nothing if not enabled.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	void handleMouseMove(const sf::Vector2i& mousePosition) override;

private:
	// List of the players. Always contains 4. Those that are enabled are considered active.
	std::vector<LobbyPlayer*> _playerList;
	// List of buttons visible on the lobby interface.
	std::vector<Button> _buttonList;
	// The RuleSet being configured.
	RuleSet* _ruleSet;
	// Variable rule text
	std::map<std::string, sf::Text> _ruleStrings;
	// Static background elements
	DrawableObjectGroup* _background;

	/**
	 * Handles the button actions by mapping the IDs to actions.
	 *
	 * @param actionID ID mapped to an action relevant to the button.
	 */
	void handleButtonPress(const int actionID);

	// Toggles player 2 and 3 between enabled and disabled states.
	void toggleNumberOfPlayers();

	// Updates the visual status based on the number of players.
	void updatePlayerNumberStatus();

	// Toggles the stacking rule and updates the message.
	void toggleStackRule();

	// Updates the text for the stack rule.
	void updateStackRuleLabel();

	// Toggles the draw rule and updates the message.
	void toggleDrawTillCanPlayRule();

	// Updates the text for the draw till can play rule.
	void updateDrawTillCanPlayRuleLabel();

	// Toggles the seven-zero rule and updates the message.
	void toggleSevenZeroRule();

	// Updates the text for the seven-zero rule.
	void updateSevenZeroRuleLabel();

	// Toggles the forced play rule and updates the message.
	void toggleForcedPlayRule();

	// Updates the text for the forced play rule.
	void updateForcedPlayRuleLabel();

	// Toggles the Jump-In rule and updates the message.
	void toggleJumpInRule();

	// Updates the text for the jump in rule.
	void updateJumpInRuleLabel();

	// Toggles the No Bluffing rule and updates the message.
	void toggleNoBluffingRule();

	// Updates the text for the no bluffing rule.
	void updateNoBluffingRuleLabel();

	// Cycles between the options for score limit
	void cycleScoreLimit();

	// Updates the label to show a message related to the score.
	void updateScoreLimitLabel();

	// Resets all rules to defaults.
	void resetRulesToDefault();

	// Updates the state of all labels.
	void updateAllRuleLabels();

	/**
	 * Loads a list of names from AINameList.txt and randomly sorts it ready for use.
	 *
	 * @return A list of names read in from the file.
	 */
	std::vector<std::string> getRandomAINameList();

	// Initialises all the labels and buttons stored in _buttonList and _ruleStrings
	void initialiseRuleOptions(const sf::IntRect& bounds, const sf::Font& font);

	// Initialises all the labels and buttons stored in _buttonList and _ruleStrings
	void initialiseBackground(const sf::IntRect& bounds, const sf::Font& font);
};

