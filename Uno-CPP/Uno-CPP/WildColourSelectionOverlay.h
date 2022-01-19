#pragma once

#include "WndInterface.h"
#include "TurnDecisionOverlayInterface.h"
#include "InteractableRect.h"
#include "DrawableObjectGroup.h"
#include "DrawableShape.h"

/**
 * Uno
 *
 * WildColourSelectorOverlay class:
 * Defines the overlay for choosing colours for wild and +4 cards.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class WildColourSelectionOverlay : public WndInterface, public TurnDecisionOverlayInterface
{
public:
	/**
	 * Initialises the overlay using the specified region.
	 *
	 * @param bounds The bounds of the entire game area. The buttons are offset from the centre.
	 */
	WildColourSelectionOverlay(const sf::IntRect& bounds, const sf::Font& font);
	virtual ~WildColourSelectionOverlay();

	// Does nothing.
	virtual void update(const float deltaTime) override {}
	// Draws a selection interface to choose a colour.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Shows the overlay.
	 *
	 * @param currentAction The action used to trigger this interface.
	 */
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

	/**
	 * Checks if the region clicked is valid for a colour selection and
	 * applies the colour as an action if appropriate.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	/**
	 * Updates the currently hovered region in the colour wheel.
	 *
	 * @param mousePosition Position of the mouse during this movement.
	 */
	virtual void handleMouseMove(const sf::Vector2i& mousePosition) override;

private:
	// Tracking of the area hovered with the current grid position and region number.
	int _hoveredRegion, _hoverX, _hoverY;
	
	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction* _currentAction;
	
	// Region where hovering is managed.
	InteractableRect _interactionRect;
	
	// Collection of background graphics for the overlay.
	DrawableObjectGroup* _background;

	// Arcs to show only when hovering.
	std::vector<DrawableShape*> _hoverArcs;
};

