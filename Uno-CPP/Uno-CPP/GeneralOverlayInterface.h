#pragma once

/**
 * Uno
 *
 * GeneralOverlayInterface interface:
 * Defines an interface to be used for defining overlays that can be
 * made to appear in many situations.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class GeneralOverlayInterface {
public:
	// Show the overlay.
	virtual void showOverlay() = 0;
};
