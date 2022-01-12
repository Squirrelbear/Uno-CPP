#pragma once

#include "WndInterface.h"
#include "TurnDecisionOverlayInterface.h"
#include "Button.h"
#include "CardFrontObjectGroup.h"

class KeepOrPlayOverlay
	: public WndInterface, public TurnDecisionOverlayInterface
{
public:
	/**
	 * Initialise the Keep and Play buttons and the location where the dummy card has to be placed.
	 *
	 * @param bounds The bounds of the entire game area. The buttons are offset from the centre.
	 */
	KeepOrPlayOverlay(const sf::IntRect& bounds, const sf::Font& font);
	virtual ~KeepOrPlayOverlay();

	// Not used.
	virtual void update(const float deltaTime) override { }

	// Draws all the buttons and the card related to the choice.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Creates a dummy version of the card to be shown as part of the overlay
	 * and makes the overlay show.
	 *
	 * @param currentAction The action used to trigger this interface.
	 */
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

	/**
	 * Does nothing when not enabled. Checks for clicks in the buttons and
	 * triggers the correct event when a button is interacted with.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;
	
	/**
	 * Does nothing when not enabled. Updates the hover state of all buttons.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

private:
	// Reference to the font.
	const sf::Font& _font;
	// List of buttons consisting of the Keep and Play buttons.
	std::vector<Button> _buttonList;
	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction* _currentAction;
	// Reference to the dummy card that the overlay is making a decision about.
	CardFrontObjectGroup* _cardForChoice;
	// Position where the card in question is to be placed.
	sf::Vector2f _cardPosition;
};

