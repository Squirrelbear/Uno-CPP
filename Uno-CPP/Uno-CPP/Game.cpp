#include "Game.h"
#include <chrono>       // std::chrono::system_clock

CurrentGameInterface* Game::_currentGame = nullptr;

Game::Game()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	randomEngine = std::default_random_engine(seed);

	_currentGame = nullptr;
	_lobbyInterface = nullptr;
	_postGameInterface = nullptr;
	_pauseInterface = nullptr;
}

Game::~Game()
{
	if (_currentGame != nullptr) {
		delete _currentGame;
	} 
	if (_lobbyInterface != nullptr) {
		delete _lobbyInterface;
	}
	if (_postGameInterface != nullptr) {
		delete _postGameInterface;
	}
	if (_pauseInterface != nullptr) {
		delete _pauseInterface;
	}
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
	if (_pauseInterface->isEnabled()) {
		_pauseInterface->draw(renderWindow);
	}
}

void Game::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (_pauseInterface->isEnabled()) {
		_pauseInterface->handleMousePress(mousePosition, isLeft);
	}
	if (_activeInterface != nullptr) {
		_activeInterface->handleMousePress(mousePosition, isLeft);
	}
}

void Game::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (_pauseInterface->isEnabled()) {
		_pauseInterface->handleMouseMove(mousePosition);
	}
	if (_activeInterface != nullptr) {
		_activeInterface->handleMouseMove(mousePosition);
	}
}

void Game::handleKeyInput(const sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Escape) {
		setPauseState(!_pauseInterface->isEnabled());
	}
	else {
		_activeInterface->handleKeyInput(key);
	}
}

CurrentGameInterface * Game::getCurrentGame()
{
	return _currentGame;
}

void Game::setPauseState(const bool isPaused)
{
	if (_activeInterface != nullptr) {
		_activeInterface->setEnabled(!isPaused);
	}
	_pauseInterface->setEnabled(isPaused);
}

void Game::showLobby()
{
	if (_activeInterface != _lobbyInterface) {
		if (_lobbyInterface != nullptr) {
			delete _lobbyInterface;
		}
		
		_lobbyInterface = new LobbyInterface(_bounds, _font);
		_activeInterface = _lobbyInterface;
	}
	setPauseState(false);
}

void Game::startGame()
{
	std::vector<LobbyPlayer*> lobbyPlayers = _lobbyInterface->getLobbyPlayerList();
	RuleSet* ruleSet = _lobbyInterface->getRuleSet();
	if (_currentGame != nullptr) {
		delete _currentGame;
	}

	_currentGame = new CurrentGameInterface(_bounds, _font, lobbyPlayers, ruleSet, randomEngine);
	_activeInterface = _currentGame;
}

void Game::startNextRound()
{
	std::vector<Player*> players = _currentGame->getAllPlayers();
	RuleSet* ruleSet = _currentGame->getRuleSet();
	if (_currentGame != nullptr) {
		delete _currentGame;
	}

	_currentGame = new CurrentGameInterface(_bounds, _font, players, ruleSet, randomEngine);
	_activeInterface = _currentGame;
}

void Game::showPostGame()
{
	std::vector<Player*> players = _currentGame->getAllPlayers();
	RuleSet* ruleSet = _currentGame->getRuleSet();
	if (_currentGame != nullptr) {
		delete _currentGame;
	}

	_postGameInterface = new PostGameInterface(_bounds, _font, players, ruleSet);
	_activeInterface = _currentGame;
}
