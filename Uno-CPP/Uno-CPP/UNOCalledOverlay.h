#pragma once

#include "PlayerFlashOverlay.h"

/**
 * Uno
 *
 * UNOCalledOverlay class:
 * Shows UNO! for a player when they called UNO!
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class UNOCalledOverlay : public PlayerFlashOverlay
{
public:
	/**
	 * Sets up the overlay ready to show.
	 *
	 * @param position Position where to place this overlay.
	 */
	UNOCalledOverlay(sf::Vector2f position, const sf::Font& font);
	virtual ~UNOCalledOverlay();

	// Draws the UNO! text flashing with showing 75% of the time.
	void draw(sf::RenderWindow & renderWindow) const override;

private:
	// The letters to be draw when this overlay is active.
	std::vector<sf::Text> _UNOLetters;
};

