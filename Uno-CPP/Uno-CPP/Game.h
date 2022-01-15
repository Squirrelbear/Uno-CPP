#pragma once

#include "CurrentGameInterface.h"
#include "LobbyInterface.h"
#include "PostGameInterface.h"
#include "PauseInterface.h"

class Game
{
public:
	Game();
	virtual ~Game();

	void update(const float deltaTime);
	void draw(sf::RenderWindow & renderWindow) const;
	void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft);
	void handleMouseMove(const sf::Vector2i& mousePosition);
	void handleKeyInput(const sf::Keyboard::Key key);

	static CurrentGameInterface* getCurrentGame();
	   
private:
	sf::IntRect _bounds;
	sf::Font _font;
	std::default_random_engine randomEngine;
	WndInterface* _activeInterface;
	static CurrentGameInterface* _currentGame;
	LobbyInterface* _lobbyInterface;
	PostGameInterface* _postGameInterface;
	PauseInterface* _pauseInterface;

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
};
