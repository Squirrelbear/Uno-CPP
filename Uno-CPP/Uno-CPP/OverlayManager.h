#pragma once

#include <map>
#include "TurnDecisionAction.h"
#include "WndInterface.h"
#include "Player.h"

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
	OverlayManager(const sf::IntRect& bounds, std::vector<Player*> playerList);
	virtual ~OverlayManager();

	// Inherited via WndInterface
	virtual void update(const float deltaTime) override;
	virtual void draw(sf::RenderWindow & renderWindow) const override;
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;
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

private:
	// Interfaces mapped to unique strings.
	std::map<std::string, WndInterface*> _overlays;
	// Current action for an active TurnDecisionAction.
	TurnDecisionAction* _overlayAction;
};

