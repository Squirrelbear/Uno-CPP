#pragma once
#include "WndInterface.h"
#include "DrawableObjectGroup.h"
#include "Button.h"

/**
 * Uno
 *
 * PauseInterface class:
 * A simple pause interface that allows a couple of buttons to control
 * game flow or otherwise just pause for a moment.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class PauseInterface :
	public WndInterface
{
public:
	/**
	 * Initialise the interface with bounds makes it ready to use.
	 *
	 * @param bounds The region to draw this interface in.
	 * @param gameBounds The bounds of the entire application to apply an transparent overlay.
	 * @param font Reference to the font for initialising text elements.
	 */
	PauseInterface(const sf::IntRect& bounds, const sf::IntRect& gameBounds, const sf::Font& font);
	virtual ~PauseInterface();

	// Do nothing during update.
	virtual void update(const float deltaTime) override {}

	// Draws the background elements and buttons on top.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Does nothing if not enabled. Checks if a button has been clicked and responds to it.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Does nothing if not enabled. Updates hover states of all buttons.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

	// By default this method returns WndResult::Nothing, Finished represents the menu should be hidden, Quit represents the game should be terminated, and Menu should return to the lobby.
	WndResultState getResultState() const override;

	// Resets the result state back to nothing after the result state has been handled.
	void resetResultState();

private:
	// The static background elements of the interface initialised by initialiseInterface().
	DrawableObjectGroup* _interfaceBackground;

	// A list of all the buttons in the interface.
	std::vector<Button> _buttonList;

	// The current state of the interface: WndResult::Nothing if no state change required, Finished represents the menu should be hidden, Quit represents the game should be terminated, and Menu should return to the lobby.
	WndResultState _resultState;

	// Initialises all the visual elements in the interface.
	void initialiseInterface(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font);

	/**
	 * Handles the actionID by mapping each ID to an action related to the button.
	 *
	 * @param actionID The actionID to map to an action from the pause menu.
	 */
	void handleButtonAction(const int actionID);
};

