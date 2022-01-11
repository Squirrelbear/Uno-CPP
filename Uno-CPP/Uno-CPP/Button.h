#pragma once
#include "InteractableRect.h"
#include "DrawableObjectGroup.h"
#include "DrawableShape.h"
#include "DrawableText.h"

/**
 * Uno
 *
 * Button class:
 * Defines a simple button consisting of a rectangle region,
 * some text to centre in it, a hover state, and an
 * actionID that is available to give the button context.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class Button :
	public InteractableRect
{
public:
	/**
	* Sets up the Button ready to display and interact with.
	*
	* @param bounds Bounds of the button. 
	* @param actionID A number that can be used to give context for when the button is detected to have been clicked.
	* @param font Reference to the font.
	*/
	Button(const sf::IntRect& bounds, const std::string& text, const int actionID, const sf::Font& font);
	virtual ~Button() = default;

	void draw(sf::RenderWindow& renderWindow) const;

	int getActionID() const;

	void setHovering(const bool isHovering);

private:
	// A number that can be used to give context for when the button is detected to have been clicked.
	int _actionID;
	// True when the mouse is over the rectangle causing a colour change.
	bool _isHovered;
	// Text to centre in the button.
	std::string _text;
	// Preconfigured views of the default button appearance and changed appearance when hovered over.
	DrawableObjectGroup _defaultView, _hoveredView;

	void initialiseDefaultView(const sf::Font& font);
	void initialiseHoveredView(const sf::Font& font);
};

