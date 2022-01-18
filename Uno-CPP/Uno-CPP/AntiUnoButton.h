#pragma once
#include "OvalHoverButton.h"
#include "GeneralOverlayInterface.h"
#include "Player.h"
#include "GameStateData.h"

/**
 * Uno
 *
 * AntiUnoButton class:
 * A special button used to pair with the Uno button for calling out
 * players who have not called their Uno.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class AntiUnoButton :
	public OvalHoverButton, public GeneralOverlayInterface
{
public:
	/**
	 * Initialises the AntiUnoButton.
	 *
	 * @param position Position to place the Uno button.
	 */
	AntiUnoButton(const sf::Vector2f position, const sf::Font& font, const GameStateData& gameData);
	virtual ~AntiUnoButton();

	/**
	 * Updates to determine if there is a player vulnerable to being called out on not saying "UNO".
	 * They are vulnerable if they only have one card, are not the bottom player (because that is the one controlling it),
	 * and the player did not call UNO yet.
	 *
	 * @param deltaTime Time since last update.
	 */
	virtual void update(const float deltaTime) override;

	/**
	 * When the button is active it means there is at least one player that can be called out.
	 * This method checks for the button being pressed and determines which player needs to be called out.
	 *
	 * @param mousePosition Position of the mouse cursor during the press.
	 * @param isLeft        If true, the mouse button is left, otherwise is right.
	 */
	virtual void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft) override;

	// Show the overlay.
	virtual void showOverlay() override;

private:
	// Reference to the BottomPlayer.
	Player* _bottomPlayer;

	// Reference to the game state.
	const GameStateData& _gameState;
};

