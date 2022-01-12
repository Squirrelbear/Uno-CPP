#include "Game.h"


WndInterface* Game::_activeInterface = nullptr;
CurrentGameInterface* Game::_currentGame = nullptr;

Game::Game()
{
	_activeInterface = nullptr;
	_currentGame = nullptr;
}


Game::~Game()
{
}

void Game::update(const float deltaTime)
{
	if (_activeInterface != nullptr) {
		_activeInterface->update(deltaTime);
	}
}

void Game::draw(sf::RenderWindow & renderWindow) const
{
	if (_activeInterface != nullptr) {
		_activeInterface->draw(renderWindow);
	}
	// TODO after pause defined
	/*if (_pauseWnd->isEnabled()) {
		_pauseWnd->draw(renderWindow);
	}*/
}

void Game::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	//_pauseWnd->handleMousePress(mousePosition, isLeft);
	if (_activeInterface != nullptr) {
		_activeInterface->handleMousePress(mousePosition, isLeft);
	}
}

void Game::handleMouseMove(const sf::Vector2i & mousePosition)
{
	//_pauseWnd->handleMouseMove(mousePosition);
	if (_activeInterface != nullptr) {
		_activeInterface->handleMouseMove(mousePosition);
	}
}

CurrentGameInterface * Game::getCurrentGame()
{
	return _currentGame;
}
