#pragma once

#include "WndInterface.h"
#include "TurnDecisionOverlayInterface.h"
#include "DrawableShape.h"
#include "DrawableText.h"
#include "GameStateData.h"

/**
 * Uno
 *
 * StatusOverlay class:
 * Defines the overlay used for showing information status about a TurnDecisionAction.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class StatusOverlay : public WndInterface, public TurnDecisionOverlayInterface
{
public:
	/**
	 * Initialise the interface ready to show a status.
	 *
	 * @param bounds The bounds of the entire game area.
	 */
	StatusOverlay(const sf::IntRect& bounds, const sf::Font& font, const GameStateData& gameData);
	virtual ~StatusOverlay();

	// Updates the timeOut remaining.
	virtual void update(const float deltaTime) override;
	// Draws the text for the status and timer.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	/**
	 * Shows the overlay by generating a status depending on the currentAction.
	 *
	 * @param currentAction The action used to trigger this interface.
	 */
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

private:
	// Centre of the bounds to draw the text at.
	sf::Vector2f _centre;
	
	// Timeout representing time remaining to complete the action.
	float _timeOut;
	
	// Text to display showing the current status.
	DrawableText* _statusText;
	
	// String showing the number representing the time remaining.
	DrawableText* _timeOutText;
	
	// Shadow for the string showing the number representing the time remaining.
	DrawableText* _timeOutShadowText;
	
	// Background for status text
	DrawableShape* _background;

	// Reference to the game state for checking the current player.
	const GameStateData& _gameData;

	/**
	 * Checks whether the action is one the player has to do or if it is someone else,
	 * and constructs a message relevant to the current situation.
	 *
	 * @param currentAction The action to use for context.
	 * @return A String representing the status message to be displayed.
	 */
	std::string createContextString(TurnDecisionAction* currentAction);

	// Updates the label and background based on the status.
	void updateStatusLabel(const std::string& status);

	// Updates the label based on the current timeout value.
	void updateTimeOutLabel();
};

