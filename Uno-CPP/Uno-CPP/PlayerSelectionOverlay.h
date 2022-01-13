#pragma once

#include "WndInterface.h"
#include "TurnDecisionOverlayInterface.h"
#include "Button.h"
#include "Player.h"

/**
 * Uno
 *
 * PlayerSelectionOverlay class:
 * Defines the overlay used for choosing to choosing a player to swap with.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class PlayerSelectionOverlay : public WndInterface, public TurnDecisionOverlayInterface
{
public:
	/**
	 * Initialises the overlay with a button for each of the other players.
	 * The buttons are centred inside the player's regions.
	 *
	 * @param bounds The bounds of the entire game area. The buttons are offset from the centre.
	 */
	PlayerSelectionOverlay(const sf::IntRect & bounds, const sf::Font & font, std::vector<Player*> playerList);
	virtual ~PlayerSelectionOverlay();

	// Does nothing
	virtual void update(const float deltaTime) override {}
	// Draws all the buttons.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Does nothing if not enabled. Checks for a click in the buttons.
	 * If a button has been clicked the action is registered to close the overlay.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Does nothing if not enabled. Updates the hover state for all buttons.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

	/**
	 * Makes the overlay visible.
	 *
	 * @param currentAction The action used to trigger this interface.
	 */
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

private:
	// List of buttons that can be used in the overlay. Includes a Challenge and Decline button.
	std::vector<Button> _buttonList;
	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction* _currentAction;
};

