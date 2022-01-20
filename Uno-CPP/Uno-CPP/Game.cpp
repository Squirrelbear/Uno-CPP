#include "Game.h"
#include <chrono>       // std::chrono::system_clock

CurrentGameInterface* Game::_currentGame = nullptr;

Game::Game(const sf::IntRect& gameBounds, const sf::Font& font)
	:	_bounds(gameBounds), _font(font)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	_randomEngine = std::default_random_engine(seed);

	_terminateGame = false;

	_currentGame = nullptr;
	_lobbyInterface = nullptr;
	_postGameInterface = nullptr;
	_pauseInterface = new PauseInterface(sf::IntRect(gameBounds.width / 2 - 100, gameBounds.height / 2 - 100, 200, 200), gameBounds, font);
	_pauseInterface->setEnabled(false);

	showLobby();
}

Game::~Game()
{
	// Current game handles deleting of Player objects if they exist because that created them.
	if (_currentGame != nullptr) {
		auto players = _currentGame->getAllPlayers();
		for (auto p : players) {
			delete p;
		}

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

		// Move to next state if necessary
		if (_activeInterface->getResultState() != WndResultState::NothingState) {
			if (_activeInterface == _lobbyInterface) {
				startGame();
			}
			else if (_activeInterface == _currentGame) {
				showPostGame();
			}
			else if (_activeInterface == _postGameInterface) {
				if (_activeInterface->getResultState() == WndResultState::Menu) {
					showLobby();
				}
				else {
					startNextRound();
				}
			}
		}
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

		// Handle a result from the pauseInterface if one was triggered.
		if (_pauseInterface->getResultState() != WndResultState::NothingState) {
			if (_pauseInterface->getResultState() == WndResultState::Finished) {
				setPauseState(false);
			} else if (_pauseInterface->getResultState() == WndResultState::Menu) {
				showLobby();
			} else if (_pauseInterface->getResultState() == WndResultState::Quit) {
				quitGame();
			}
			else
			_pauseInterface->resetResultState();
		}
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

bool Game::getGameCloseRequested()
{
	return _terminateGame;
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
	// Allows for transition from anywhere including itself while using the pause menu
	// Only wipe the interface and recreate it if the interface is not already active.
	if (_activeInterface != _lobbyInterface || _activeInterface == nullptr) {
		if (_lobbyInterface != nullptr) {
			delete _lobbyInterface;
			_lobbyInterface = nullptr;
		}
		
		// Create the interface and make it active.
		_lobbyInterface = new LobbyInterface(_bounds, _font, _randomEngine);
		_activeInterface = _lobbyInterface;
	}

	// Hide the pause menu in case that is what brought the user here.
	setPauseState(false);
}

void Game::startGame()
{
	// Expects transition from lobby as the start of a new game
	std::vector<LobbyPlayer*> lobbyPlayers = _lobbyInterface->getLobbyPlayerList();
	RuleSet* ruleSet = _lobbyInterface->getRuleSet();

	// Delete any existing current game including all the references to players.
	if (_currentGame != nullptr) {
		auto players = _currentGame->getAllPlayers();
		for (auto p : players) {
			delete p;
		}

		delete _currentGame;
		_currentGame = nullptr;
	}

	// Create a new game using LobbyPlayers to let CurrentGameInterface create the Player objects.
	_currentGame = new CurrentGameInterface(_bounds, _font, lobbyPlayers, ruleSet, _randomEngine);
	_activeInterface = _currentGame;
}

void Game::startNextRound()
{
	// Expects transition from post game so get the info from there
	std::vector<Player*> players = _postGameInterface->getPlayers();
	RuleSet* ruleSet = _postGameInterface->getRuleSet();

	// Delete any existing current game but NOT the players
	if (_currentGame != nullptr) {
		delete _currentGame;
		_currentGame = nullptr;
	}

	// Create the new current game and make it active
	_currentGame = new CurrentGameInterface(_bounds, _font, players, ruleSet, _randomEngine);
	_activeInterface = _currentGame;
}

void Game::showPostGame()
{
	// Expects transition from current game
	std::vector<Player*> players = _currentGame->getAllPlayers();
	RuleSet* ruleSet = _currentGame->getRuleSet();

	// Delete any existing post game interface
	if (_postGameInterface != nullptr) {
		delete _postGameInterface;
		_postGameInterface = nullptr;
	}

	// Create the interface and make it active
	_postGameInterface = new PostGameInterface(_bounds, _font, players, ruleSet);
	_activeInterface = _postGameInterface;
}

void Game::quitGame()
{
	_terminateGame = true;
}
