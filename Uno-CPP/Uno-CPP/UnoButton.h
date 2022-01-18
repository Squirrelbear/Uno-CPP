#pragma once


#include "GeneralOverlayInterface.h"
#include "Player.h"
#include "OvalHoverButton.h"
#include "GameStateData.h"

/**
 * Uno
 *
 * UnoButton class:
 * A special variation of button that appears differently for the Uno calling.
 * Pressing the button is intended when a player reaches 2 or less cards.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class UnoButton : public OvalHoverButton, public GeneralOverlayInterface
{
public:
	/**
	 * Initialises the UnoButton.
	 *
	 * @param position Position to place the Uno button.
	 */
	UnoButton(const sf::Vector2f position, const sf::Font& font, const GameStateData& gameData);
	virtual ~UnoButton() = default;

	/**
	 * Enables the button when it should be available.
	 *
	 * @param deltaTime Time since last update.
	 */
	virtual void update(const float deltaTime) override;

	// Shows the overlay.
	virtual void showOverlay() override;
	/**
	 * When the button is available and is clicked the player is flagged as having called and the called signal is flashed.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

private:
	// Reference to the BottomPlayer.
	Player* _bottomPlayer;

	// Reference to the game state for checking current player
	const GameStateData& _gameState;
};

