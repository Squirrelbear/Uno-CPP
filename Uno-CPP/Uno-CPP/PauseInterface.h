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

private:
	// The static background elements of the interface initialised by initialiseInterface().
	DrawableObjectGroup* _interfaceBackground;
	// A list of all the buttons in the interface.
	std::vector<Button> _buttonList;

	void initialiseInterface(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font);
};

