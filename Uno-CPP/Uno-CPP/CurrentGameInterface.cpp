#include "CurrentGameInterface.h"
#include "TurnActionFactory.h"

CurrentGameInterface::CurrentGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<LobbyPlayer*> playerList, RuleSet* ruleSet, std::default_random_engine& randomEngine) 
	: CurrentGameInterface(bounds, font, createPlayersFromLobby(playerList, bounds, font), ruleSet, randomEngine)
{
}

CurrentGameInterface::CurrentGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<Player*> playerList, RuleSet* ruleSet, std::default_random_engine& randomEngine)
	: WndInterface(bounds), _recentCardPile(sf::Vector2f(bounds.width / 2 - 30, bounds.height / 2 - 45))
{
	_ruleSet = ruleSet;
	_deck = new Deck(sf::Vector2f(bounds.width / 2 - 30 - 160, bounds.height / 2 - 45), font, randomEngine);
	for (auto player : playerList) {
		_players.emplace_back(player);
		if (player->getPlayerType() == Player::PlayerType::ThisPlayer) {
			_bottomPlayer = player;
		}
		player->emptyHand();
		for (int i = 0; i < 7; i++) {
			player->addCardToHand(_deck->drawCard());
		}
	}
	_currentPlayerID = randomEngine() % playerList.size();
	_isIncreasing = randomEngine() % 100 < 50;
	_playDirectionAnimation = new PlayDirectionAnimation(sf::Vector2f(bounds.width / 2, bounds.height / 2), 120, 5);
	_playDirectionAnimation->setIsIncreasing(_isIncreasing);

	_overlayManager = new OverlayManager(bounds, playerList, font);
	_recentCardPile.forcePlayCard(_deck->drawCard());
	_debugModeEnabled = false;

	_turnActionSequenceManager = new TurnActionSequenceManager(_debugModeEnabled);

	_resultState = WndResultState::NothingState;
}


CurrentGameInterface::~CurrentGameInterface()
{
	delete _overlayManager;
	delete _playDirectionAnimation;
	delete _turnActionSequenceManager;
}

void CurrentGameInterface::update(const float deltaTime)
{
	if (!isEnabled()) return;

	_playDirectionAnimation->update(deltaTime);
	_overlayManager->update(deltaTime);
	updateTurnAction();
	for (auto player : _players) {
		player->update(deltaTime);
	}
	checkForEndOfRound();
}

void CurrentGameInterface::draw(sf::RenderWindow & renderWindow) const
{
	_deck->draw(renderWindow);
	_recentCardPile.draw(renderWindow);
	for (const auto player : _players) {
		player->draw(renderWindow);
	}
	_overlayManager->draw(renderWindow);
	_playDirectionAnimation->draw(renderWindow);
}

void CurrentGameInterface::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	_overlayManager->handleMousePress(mousePosition, isLeft);
	
	// Handle player turn actions 
	if (!_turnActionSequenceManager->hasActiveTurnAction()) {
		if (_currentPlayerID == _bottomPlayer->getPlayerID()) {
			if (_deck->isPositionInside(mousePosition)) {
				_turnActionSequenceManager->setSequence(TurnActionFactory::drawCardAsAction(_currentPlayerID));
			}
			else {
				Card* cardToPlay = _bottomPlayer->chooseCardFromClick(mousePosition);
				Card* topCard = _recentCardPile.getTopCard();
				// Play if a valid move
				std::vector<Card*> validMoves = _bottomPlayer->getValidMoves(topCard->getFaceValueID(), topCard->getColourID());
				if (std::find(validMoves.begin(), validMoves.end(), cardToPlay) != validMoves.end()) {
					_turnActionSequenceManager->setSequence(TurnActionFactory::playCardAsAction(
							_currentPlayerID, cardToPlay->getUniqueCardID(), 
							cardToPlay->getFaceValueID(), cardToPlay->getColourID()));
				}
			}
		} else if (_ruleSet->allowJumpInRule()) {
			Card* cardToPlay = _bottomPlayer->chooseCardFromClick(mousePosition);
			if (cardToPlay != nullptr) {
				jumpIn(_bottomPlayer->getPlayerID(), cardToPlay);
			}
		}
	}
}

void CurrentGameInterface::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	_overlayManager->handleMouseMove(mousePosition);
	_bottomPlayer->updateHover(mousePosition);
}

void CurrentGameInterface::handleKeyInput(const sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Q) {
		sortHand();
	}
	else if (key == sf::Keyboard::Key::Num0) {
		_debugModeEnabled = !_debugModeEnabled;
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num9) {
		revealHands();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num8) {
		toggleTurnDirection();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num7) {
		_bottomPlayer->emptyHand();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num6) {
		_bottomPlayer->removeCard(_bottomPlayer->getHand().at(0));
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num5) {
		_turnActionSequenceManager->toggleDebugShowTreeOnNewAction();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num4) {
		_turnActionSequenceManager->toggleDebugShowTaskActionNotes();
	}
	else {
		// Note: The OverlayManager does not actually use the keys at all, but could be used in future.
		_overlayManager->handleKeyInput(key);
	}
}

void CurrentGameInterface::jumpIn(const int playerID, Card * cardToPlay)
{
	Card* topCard = _recentCardPile.getTopCard();
	if (!_turnActionSequenceManager->hasActiveTurnAction() && _currentPlayerID != playerID
		&& topCard->getFaceValueID() == cardToPlay->getFaceValueID()
		&& topCard->getColourID() == cardToPlay->getColourID()) {
		_currentPlayerID = playerID;
		showGeneralOverlay("JumpIn" + playerID);
		_turnActionSequenceManager->setSequence(TurnActionFactory::playCardAsAction(_currentPlayerID, 
			cardToPlay->getUniqueCardID(), cardToPlay->getFaceValueID(), cardToPlay->getColourID()));
	}
}

void CurrentGameInterface::showOverlayForTurnAction()
{
	if (typeid(_turnActionSequenceManager->getCurrentTurnAction()) == typeid(TurnDecisionAction)) {
		_overlayManager->showDecisionOverlay(dynamic_cast<TurnDecisionAction*>(_turnActionSequenceManager->getCurrentTurnAction()));
	}
}

void CurrentGameInterface::showGeneralOverlay(const std::string& overlayName)
{
	_overlayManager->showGeneralOverlay(overlayName);
}

void CurrentGameInterface::revealHands()
{
	for (auto player : _players) {
		player->revealHand(true);
	}
}

void CurrentGameInterface::sortHand()
{
	_bottomPlayer->sortHand();
}

Player * CurrentGameInterface::getBottomPlayer() const
{
	return _bottomPlayer;
}

void CurrentGameInterface::toggleTurnDirection()
{
	_isIncreasing = !_isIncreasing;
	_playDirectionAnimation->setIsIncreasing(_isIncreasing);
}

void CurrentGameInterface::moveToNextPlayer()
{
	updateUNOState();
	if (_isIncreasing) {
		_currentPlayerID++;
		if (_currentPlayerID >= _players.size()) {
			_currentPlayerID = 0;
		}
	}
	else {
		_currentPlayerID--;
		if (_currentPlayerID < 0) {
			_currentPlayerID = _players.size() - 1;
		}
	}
}

void CurrentGameInterface::applyAntiUno(const int playerID)
{
	showGeneralOverlay("AntiUnoCalled" + playerID);
	// Set to safe to prevent multiple anti-uno callouts.
	_players.at(playerID)->setUnoState(Player::UNOState::Safe);
	_players.at(playerID)->addCardToHand(_deck->drawCard());
	_players.at(playerID)->addCardToHand(_deck->drawCard());
}

bool CurrentGameInterface::isIncreasing() const
{
	return _isIncreasing;
}

void CurrentGameInterface::setCurrentTurnAction(TurnActionSequence<TurnAction>* newSequence)
{
	_turnActionSequenceManager->setSequence(newSequence);
}

TurnAction * CurrentGameInterface::getCurrentTurnAction() const
{
	return _turnActionSequenceManager->getCurrentTurnAction();
}

RuleSet * CurrentGameInterface::getRuleSet() const
{
	return _ruleSet;
}

Player * CurrentGameInterface::getCurrentPlayer() const
{
	return _players.at(_currentPlayerID);
}

std::vector<Player*> CurrentGameInterface::getAllPlayers() const
{
	return _players;
}

Player * CurrentGameInterface::getPlayerByID(const int playerID) const
{
	return _players.at(playerID);
}

Deck * CurrentGameInterface::getDeck() const
{
	return _deck;
}

WndResultState CurrentGameInterface::getResultState() const
{
	return _resultState;
}

void CurrentGameInterface::checkForEndOfRound()
{
	for (auto player : _players) {
		if (player->getHand().size() == 0) {
			int totalScore = 0;
			for (auto playerForValue : _players) {
				if (playerForValue != player) {
					playerForValue->setCurrentRoundScore(0);
					totalScore += playerForValue->getHandTotalScore();
				}
			}
			player->setCurrentRoundScore(totalScore);
			player->setWon();
			_resultState = WndResultState::Finished;
			return;
		}
	}
}

void CurrentGameInterface::updateTurnAction()
{
	/*
	        if(currentTurnAction != null) {
            // Tree Debug Output
            if(GamePanel.DEBUG_MODE && debugShowTaskActionNotes) {
                if (currentTurnAction instanceof TurnActionFactory.TurnDecisionAction) {
                    if (!((TurnActionFactory.TurnDecisionAction) currentTurnAction).hasRunOnce) {
                        System.out.println(currentTurnAction.actionDebugText);
                    }
                } else {
                    System.out.println(currentTurnAction.actionDebugText);
                }
            }
            currentTurnAction.performAction();
            currentTurnAction = currentTurnAction.getNext();
            if(queuedTurnAction != null) {
                currentTurnAction = queuedTurnAction;
                queuedTurnAction = null;
            }
        }
	*/
}

void CurrentGameInterface::updateUNOState()
{
	_players.at(_currentPlayerID)->setUnoState(_players.at(_currentPlayerID)->getHand().size() == 1 ? Player::UNOState::NotSafe : Player::UNOState::Safe);
	for (auto player : _players) {
		if (player->getPlayerID() != _currentPlayerID) {
			player->setUnoState(Player::UNOState::Safe);
		}
	}
}

std::vector<Player*> CurrentGameInterface::createPlayersFromLobby(const std::vector<LobbyPlayer*> playerList, sf::IntRect bounds, const sf::Font& font)
{
	/*
	        List<Player> result = new ArrayList<>();
        List<LobbyPlayer> playersToAdd = playerList.stream().filter(LobbyPlayer::isEnabled).collect(Collectors.toList());
        if(playersToAdd.size() != 2 && playersToAdd.size() != 4) {
            System.out.println("Critical Error. Only combinations of 2 or 4 players are allowed");
            return result;
        }
        int thisPlayerIndex = -1;
        for(int i = 0; i < playersToAdd.size(); i++) {
            if(playersToAdd.get(i).getPlayerType() == Player.PlayerType.ThisPlayer) {
                if(thisPlayerIndex == -1) {
                    thisPlayerIndex = i;
                } else {
                    System.out.println("Critical Error. Only one ThisPlayer is allowed.");
                    return result;
                }
            }
        }
        if(thisPlayerIndex == -1) {
            System.out.println("Critical Error. One ThisPlayer is required!");
            return result;
        }

        for (int i = 0; i < playersToAdd.size(); i++) {
            Rectangle playerRegion;
            boolean showNameLeft;
            if(playersToAdd.size() == 4) {
                playerRegion = getPlayerRect((i + 4 - thisPlayerIndex) % 4, bounds);
                showNameLeft = (i + 4 - thisPlayerIndex) % 2 == 0;
            } else {
                playerRegion = getPlayerRect(playersToAdd.get(i).getPlayerType() == Player.PlayerType.ThisPlayer ? 0 : 2, bounds);
                showNameLeft = true;
            }
            if(playersToAdd.get(i).getPlayerType() == Player.PlayerType.AIPlayer) {
                result.add(new AIPlayer(i, playersToAdd.get(i).getPlayerName(), playerRegion, playersToAdd.get(i).getAIStrategy(), showNameLeft));
            } else {
                result.add(new Player(i, playersToAdd.get(i).getPlayerName(), playersToAdd.get(i).getPlayerType(), playerRegion, showNameLeft));
            }
        }
        return result;
	*/

	return std::vector<Player*>();
}

sf::IntRect CurrentGameInterface::getPlayerRect(const int direction, const sf::IntRect & bounds)
{
	switch (direction) {
		case 1: return sf::IntRect(bounds.left, bounds.top + bounds.height / 2 - 150, (CARD_WIDTH + 4) * 6, bounds.height / 2 - 100 - 10);
		case 2: return sf::IntRect(bounds.left + bounds.width / 2 - (CARD_WIDTH + 4) * 10 / 2, bounds.top - 30,
									(CARD_WIDTH + 4) * 10, bounds.height / 2 - 100 - 10);
		case 3: return sf::IntRect(bounds.left + bounds.width - ((CARD_WIDTH + 4) * 6 + 50) + 50,
									bounds.top + bounds.height / 2 - 150, (CARD_WIDTH + 4) * 6, bounds.height / 2 - 100 - 10);
		default: return sf::IntRect(bounds.left +bounds.width / 2 - (CARD_WIDTH + 4) * 10 / 2,
								bounds.top + bounds.height / 2 + 130, (CARD_WIDTH + 4) * 10, bounds.height / 2 - 100 - 10);
	}
}
