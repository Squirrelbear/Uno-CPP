#pragma once

#include "WndInterface.h"
#include "TurnDecisionOverlayInterface.h"
#include "Button.h"
#include "Player.h"

/**
 * Uno
 *
 * ChallengeOverlay class:
 * Defines the overlay used for choosing to Challenge/Decline/Stack against a +4.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class ChallengeOverlay : public WndInterface, public TurnDecisionOverlayInterface
{
public:
	/**
	 * Initialises the overlay with a Challenge and Decline button, and checks
	 * whether the RuleSet allows for stacking to cache processing for later.
	 *
	 * @param bounds The bounds of the entire game area. The buttons are offset from the centre.
	 * @param font reference to the font for adding buttons.
	 */
	ChallengeOverlay(const sf::IntRect& bounds, const sf::Font& font);
	virtual ~ChallengeOverlay() = default;


	// Does nothing.
	void update(const float deltaTime) override {};

	// Draws the buttons.
	void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Does nothing if not enabled. Updates the hover state for all buttons.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	void handleMouseMove(const sf::Vector2i& mousePosition) override;

	/**
	 * Does nothing if not enabled. Checks for a click in the buttons.
	 * If a button has been clicked the action is registered to close the overlay.
	 * Otherwise if a card has been clicked that is a +4 when stacking is enabled
	 * the card is chained.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Makes the overlay visible.
	 *
	 * @param currentAction The TurnAction used to make this overlay appear.
	 */
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

private:
	// List of buttons that can be used in the overlay. Includes a Challenge and Decline button.
	std::vector<Button> _buttonList;
	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction* _currentAction;
	// When true via the RuleSet the player's cards can be stacked if possible.
	bool _allowStacking;
	// Reference to the player to be used for card selection when stacking is allowed.
	Player* _playerReference;
};

