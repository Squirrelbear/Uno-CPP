#pragma once

#include "WndInterface.h"
#include "GeneralOverlayInterface.h"
#include "PolygonShape.h"

/**
 * Uno
 *
 * ChallengeSuccessOverlay class:
 * Displays a short time flashing tick to show the challenge was successful.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class ChallengeSuccessOverlay : public WndInterface, public GeneralOverlayInterface
{
public:
	/**
	 * Initialise the interface with bounds and makes it ready to be enabled.
	 *
	 * @param bounds Region where the object is shown.
	 */
	ChallengeSuccessOverlay(const sf::IntRect& bounds);
	virtual ~ChallengeSuccessOverlay();

	/**
	 * Updates the timer to hide the overlay and hides it when it hits 0.
	 *
	 * @param deltaTime Time since last update.
	 */
	virtual void update(const float deltaTime) override;

	// Draws the tick flashing with showing 75% of the time.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	// Shows the overlay and sets a timer for how long it will appear.
	virtual void showOverlay() override;

private:
	// Timer till the overlay is hidden again.
	float _displayTimer;
	// A tick that will be flashed using the display timer when overlay is enabled.
	PolygonShape* _tickShape;
};

