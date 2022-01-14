#pragma once

#include "WndInterface.h"
#include "DrawableShape.h"
#include "DrawableObjectGroup.h"

/*
 * UNO
 *
 * OvalHoverButton class:
 * A generic class that is used for both UnoButton and AntiUnoButton.
 *
 * @author Peter Mitchell
 * @version 2022.1
*/
class OvalHoverButton : public WndInterface
{
public:
	/*
		Initialises the button with bounds, font, message, and colour.
	*/
	OvalHoverButton(const sf::IntRect& bounds, const sf::Font& font, const std::string& message, const sf::Color& bgColour);
	virtual ~OvalHoverButton();

	/**
	 * Updates the hover state of the button.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;


	// Draws the button with an expanding oval on hover with the text in the middle.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

protected:
	// Current hover status of the button.
	bool _isHovered;
	// When isActive is active the button can be interacted with and is visible.
	bool _isActive;

private:
	// Background colour
	sf::Color _bgColour;
	// Shape that increases in size when hovered.
	DrawableShape* _hoverShape;
	// Text shown on top of the oval.
	DrawableObjectGroup* _text;
};

