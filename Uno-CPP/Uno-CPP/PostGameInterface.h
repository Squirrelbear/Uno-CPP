#pragma once

#include "WndInterface.h"
#include "Player.h"
#include "RuleSet.h"
#include "Button.h"
#include "DrawableObjectGroup.h"

/**
 * Uno
 *
 * PostGameInterface class:
 * Defines a simple interface that shows the scores from the just completed round.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class PostGameInterface : public WndInterface
{
public:
	/**
	 * Initialise the interface with bounds and make it enabled.
	 *
	 * @param bounds Bounds of the interface.
	 */
	PostGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<Player*>& playerList, RuleSet* ruleSet);
	virtual ~PostGameInterface();

	// Does nothing.
	virtual void update(const float deltaTime) override {}

	// Draws all the elements of the interface.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Does nothing if not enabled. Checks all the buttons for presses and handles actions if necessary.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Does nothing if not enabled. Updates the hover state of all buttons.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

	// Returns Nothing when interface is still in use, Finished for Continue, Menu for Lobby
	WndResultState getResultState() const override;

	// Gets references to all the players.
	std::vector<Player*> getPlayers() const;

	// Gets a reference to the rules.
	RuleSet* getRuleSet() const;

private:
	// List of buttons visible on the interface.
	std::vector<Button> _buttonList;
	
	// Reference to the players that are being shown for scores.
	std::vector<Player*> _playerList;
	
	// Static interface elements
	DrawableObjectGroup* _staticElements;
	
	// Result state that changes to Finished for Continue, Menu for Lobby
	WndResultState _resultState;

	// Reference to the rules.
	RuleSet* _rules;

	/**
	 * Looks up the action to apply based on an actionID for a button.
	 *
	 * @param actionID The actionID for a button that was pressed.
	 */
	void handleButtonPress(const int actionID);

	// Initialises all the visual elements of the interface.
	void initialiseInterface(const sf::Font& font, RuleSet* ruleSet);
};

