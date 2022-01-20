#pragma once

#include <map>
#include "TurnDecisionAction.h"
#include "WndInterface.h"
#include "Player.h"
#include "GameStateData.h"
#include "UnoButton.h"
#include "AntiUnoButton.h"

/**
 * Uno
 *
 * OverlayManager class:
 * Defines a manager to control overlays for the CurrentGameInterface.
 * These overlays include those that wait for the player to interface with them
 * and some that are just informational.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class OverlayManager : public WndInterface
{
public:
	/**
	 * Initialise the interfaces all ready for any that needs to be made visible.
	 *
	 * @param bounds The bounds of the entire game area.
	 * @param playerList Has the list of playerrs as required for the playerSelection overlay.
	 */
	OverlayManager(const sf::IntRect& bounds, std::vector<Player*> playerList, const sf::Font& font, GameStateData gameData);
	virtual ~OverlayManager();

	/**
	 * Updates all the active overlays and hides all the decision overlays if the TurnAction changed.
	 *
	 * @param deltaTime Time since last update.
	 * @param currentTurnAction The reference to the current action.
	 */
	virtual void update(const float deltaTime, const TurnAction* currentTurnAction);

	// Do not call the default WndInterface update method for this.
	virtual void update(const float deltaTime) override {};

	// Draws all enabled overlays.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Passes the mousePress event on to all enabled overlays.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Passes the mouseMove event on to all enabled overlays.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

	/**
	 * Finds the matching overlay for a decision if necessary, and then shows it.
	 * Then shows the statusOverlay in all situations even if it is not the current player's decision.
	 *
	 * @param currentAction Action to use for determining which overlay to show.
	 */
	void showDecisionOverlay(TurnDecisionAction* currentAction);

	/**
	 * Finds the matching interface and makes it visible if possible.
	 *
	 * @param overlayName Name that maps to an interface.
	 */
	void showGeneralOverlay(const std::string& overlayName);

	// Hides all the decision overlays automatically called when the TurnAction changes in update().
	void hideAllDecisionOverlays();

	// Gets any trigger state of the Uno or AntiUno buttons handled during handleMousePress
	PlayerUpdateResult getUnoButtonsState();

private:
	// Interfaces mapped to unique strings.
	std::map<std::string, WndInterface*> _overlays;

	// Current action for an active TurnDecisionAction.
	TurnDecisionAction* _overlayAction;

	// Reference to core game variables.
	GameStateData _gameState;

	// Reference to the Uno button for getUnoButtonsState()
	UnoButton* _unoButton;

	// Reference to the AntiUno button for getUnoButtonState()
	AntiUnoButton* _antiUnoButton;
};

