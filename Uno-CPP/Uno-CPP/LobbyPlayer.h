#pragma once
#include "InteractableRect.h"
#include "AIPlayer.h"
#include "DrawableObjectGroup.h"
#include "DrawableShape.h"
#include "DrawableText.h"

/**
 * Uno
 *
 * LobbyPlayer class:
 * Defines a player in the Lobby menu with functions to modify their settings ready before a game starts.
 *
 * @author Peter Mitchell
 * @version 2022.1
 */
class LobbyPlayer :
	public InteractableRect
{
public:
	/**
	 * Initialises the object ready to show information about the player.
	 *
	 * @param playerName The name shown for the player.
	 * @param playerType The type of player (ThisPlayer, AIPlayer, or NetworkPlayer).
	 * @param bounds Region for interacting with this player object in the menu.
	 */
	LobbyPlayer(const std::string& playerName, const Player::PlayerType playerType, const sf::IntRect& bounds, const sf::Font& font);
	virtual ~LobbyPlayer();

	/**
	 * Changes the player's name.
	 *
	 * @param playerName Name to change the player to.
	 */
	void setPlayerName(const std::string& playerName);

	// Gets the player's name.
	std::string getPlayerName() const;

	// Gets the type of player. (ThisPlayer, AIPlayer).
	Player::PlayerType getPlayerType() const;

	/**
	 * Sets the enabled state of the player.
	 *
	 * @param isEnabled When true, the player is included in the list of players for the game.
	 */
	void setEnabled(const bool isEnabled);

	/**
	 * Gets the current enabled status.
	 *
	 * @return When true the player should be visible and included as a player.
	 */
	bool isEnabled() const;

	// Gets the strategy for this player. This is only relevant for AIPlayer types.
	AIPlayer::AIStrategy getAIStrategy() const;

	// Handles the click to either iterate the strategy for AI or choose a new name for the player.
	void handleClick();

	// Does nothing if not enabled. Draws the content showing this player's information.
	void draw(sf::RenderWindow& renderWindow) const;

	/**
	 * Updates the hovered state of the button object based on where the mouse is.
	 *
	 * @param mousePosition Position of the mouse.
	 */
	void updateHoverState(const sf::Vector2i& mousePosition);

private:
	// The name shown for the player.
	std::string _playerName;

	// The type of player(ThisPlayer, AIPlayer).
	Player::PlayerType _playerType;

	// The strategy to use for the AIPlayer type.
	AIPlayer::AIStrategy _aiStrategy;

	// A String showing the text version of the strategy.
	std::string _strategyStr;

	// Visible and included in the collection of players when true.
	bool _isEnabled;

	// True when the mouse is over the player.
	bool _isHovered;

	// String representing the type of player.
	std::string _playerTypeStr;

	// All drawable elements
	DrawableObjectGroup* _drawableObject;

	// Background Rect referenced for changing colour on hover.
	DrawableShape* _background;

	// Background colour when hovered
	sf::Color _bgHoverColour;

	// Background colour when not hovered
	sf::Color _bgNotHoveredColour;

	// Player Name Element
	DrawableText* _playerNameText;

	// AI Strategy Element
	DrawableText* _aiStrategyText;

	/**
	 * Provides ability to input a string up to 12 characters long.
	 * The String trims whitespace before evaluating and caps the maximum length
	 * at 12 characters long. It will do nothing if there is not at least 1 valid character.
	 */
	void chooseNewName();

	// Iterates through the list of AI Strategies to the next one.
	void iterateStrategy();
};

