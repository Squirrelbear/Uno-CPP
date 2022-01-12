#pragma once

#include "CurrentGameInterface.h"

class Game
{
public:
	Game();
	virtual ~Game();

	void update(const float deltaTime);
	void draw(sf::RenderWindow & renderWindow) const;
	void handleMousePress(const sf::Vector2i& mousePosition, bool isLeft);
	void handleMouseMove(const sf::Vector2i& mousePosition);

	static CurrentGameInterface* getCurrentGame();
	   
private:
	static WndInterface* _activeInterface;
	static CurrentGameInterface* _currentGame;
};
