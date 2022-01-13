#pragma once

#include "WndInterface.h"
#include "TurnDecisionOverlayInterface.h"
#include "Button.h"
#include "Player.h"

/**
 * Uno
 *
 * StackChoiceOverlay class:
 * Defines the overlay used for choosing to Decline/Stack against a +2.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class StackChoiceOverlay : public WndInterface, public TurnDecisionOverlayInterface
{
public:
	/**
	 * Initialise the decline button and reference to the player for tracking their cards.
	 *
	 * @param bounds The bounds of the entire game area. The buttons are offset from the centre.
	 */
	StackChoiceOverlay(const sf::IntRect & bounds, const sf::Font & font);
	virtual ~StackChoiceOverlay();

	// Does nothing.
	virtual void update(const float deltaTime) override {};

	// Draws the Decline button.
	virtual void draw(sf::RenderWindow& renderWindow) const override;

	/**
	 * Does nothing if not enabled. Checks for a click on the decline button to handle it.
	 * And checks for the player clicking on their cards to allow stacking.
	 * If this overlay is visible it is implied that stacking is allowed.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Does nothing if not enabled. Updates the hover status of the decline button.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

	/**
	 * Shows the overlay.
	 *
	 * @param currentAction The action used to trigger this interface.
	 */
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

private:
	// The decline button that simply accepts taking the card drawing.
	Button* _declineButton;
	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction* _currentAction;
	// Reference to the bottom player who is making the choice.
	Player* _playerReference;
};

