#pragma once

#include "CurrentGameInterface.h"
#include "LobbyInterface.h"
#include "PostGameInterface.h"
#include "PauseInterface.h"

/*
UNO

Game class:
Manages all the windows and transitions between game states for the lobby, current game, and postgame.

@author: Peter Mitchell
@version 2022.1
*/
class Game
{
public:
	// Initialises the game with the Lobby.
	Game(const sf::IntRect& gameBounds, const sf::Font& font);
	virtual ~Game();

	// Updates the current state of the active window.
	void update(const float deltaTime);

	// Draws the current window.
	void draw(sf::RenderWindow & renderWindow) const;

	// Handles passing events on to the active interface.
	void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft);

	// Handles passing the mouse moved event to the active interface.
	void handleMouseMove(const sf::Vector2i& mousePosition);

	// Handles passing key pressed event to the active interface.
	void handleKeyInput(const sf::Keyboard::Key key);

	// Gets a reference to the single active interface used to access all the callbacks.
	static CurrentGameInterface* getCurrentGame();
	   
	//  When true, the game should end.
	bool getGameCloseRequested();

private:
	// Bounds of the game area.
	const sf::IntRect _bounds;

	// Reference to the font
	const sf::Font& _font;

	// Shared randomEngine used for all the randomisation.
	std::default_random_engine randomEngine;

	// Reference to the current active interface.
	WndInterface* _activeInterface;

	// Reference to the active current game interface or nullptr.
	static CurrentGameInterface* _currentGame;

	// Reference to the active LobbyInterface if one exists. Used for passing the LobbyPlayer objects into the CurrentGame.
	LobbyInterface* _lobbyInterface;

	// Reference to the active PostGameInterface if one exists. Used for the Post Game results.
	PostGameInterface* _postGameInterface;

	// Reference to the PauseInterface shown when the game enters a pause state.
	PauseInterface* _pauseInterface;

	// When true, the game should end.
	bool _terminateGame;

	/**
	 * Pauses or unpauses the game.
	 *
	 * @param isPaused When true the game is paused and pause window is shown.
	 */
	void setPauseState(const bool isPaused);

	/**
	 * Sets the currently active interface to the lobby removing any existing interface.
	 * If this is triggered from the pause interface it just resumes the current interface.
	 */
	void showLobby();

	// Creates a new game with the specified list of players. Use this for coming from the lobby.
	void startGame();

	// Creates a new game with the specified list of players. Use this for coming from post-game.
	void startNextRound();

	// Sets the currently active interface to the post-game interface after a game has ended.
	void showPostGame();

	// Terminates the game.
	void quitGame();
};
