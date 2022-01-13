#pragma once

#include "WndInterface.h"
#include "GeneralOverlayInterface.h"

/**
 * Uno
 *
 * PlayerFlashOverlay class:
 * Defines a generic flashing text message class to display many types of messages.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class PlayerFlashOverlay : public WndInterface, public GeneralOverlayInterface
{
public:
	/**
	 * Sets up the overlay ready to show.
	 *
	 * @param position Position where to place this overlay.
	 * @param message Message to display.
	 * @param colour Colour to show message with.
	 */
	PlayerFlashOverlay(sf::Vector2f position, const std::string& message, const sf::Color& colour, const int fontSize, const sf::Font& font);
	virtual ~PlayerFlashOverlay();

	/**
	 * Updates the timer to hide the overlay and hides it when it hits 0.
	 *
	 * @param deltaTime Time since last update.
	 */
	void update(const float deltaTime) override;
	
	// Draws the message.
	void draw(sf::RenderWindow & renderWindow) const override;

	// Shows the overlay and sets a timer for how long it will appear.
	void showOverlay() override;

	/**
	 * Sets the message to the new value.
	 *
	 * @param message The message to display.
	 */
	void setMessage(const std::string& message);

private:
	// The text that is drawn by this overlay.
	sf::Text _flashingText;
	// Same text, but black for a shadow.
	sf::Text _shadowText;
	// Centre of where the text should be.
	sf::Vector2f _position;
	// Timer till the overlay is hidden again.
	float _displayTimer;
};

