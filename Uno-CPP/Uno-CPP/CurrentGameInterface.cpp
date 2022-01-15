#include "CurrentGameInterface.h"



CurrentGameInterface::CurrentGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<Player*>& playerList, RuleSet* ruleSet, std::default_random_engine& randomEngine)
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
	//  isIncreasing = (Math.random() * 100 < 50); // TODO
	// TODO PlayDirection Animation: 
	// set isIncreasing PlayDir

	_overlayManager = new OverlayManager(bounds, playerList, font);
	_recentCardPile.forcePlayCard(_deck->drawCard());
}


CurrentGameInterface::~CurrentGameInterface()
{
	delete _overlayManager;
}

void CurrentGameInterface::update(const float deltaTime)
{
	if (!isEnabled()) return;

	//_playDirectionAnimation.update(deltaTime); // TODO
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
	// _recentCardPile.draw(renderWindow); // TODO
	for (const auto player : _players) {
		player->draw(renderWindow);
	}
	_overlayManager->draw(renderWindow);
	//_playDirectionAnimation.draw(renderWindow);
}

void CurrentGameInterface::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	_overlayManager->handleMousePress(mousePosition, isLeft);
	/* // TODO
	if (_currentTurnAction == nullptr && _currentPlayerID == _bottomPlayer->getPlayerID()) {
		if (deck.isPositionInside(mousePosition)) {
			currentTurnAction = TurnActionFactory.drawCardAsAction(currentPlayerID);
		}
		else {
			Card* cardToPlay = _bottomPlayer.chooseCardFromClick(mousePosition);
			Card* topCard = _recentCardPile.getTopCard();
			if (_bottomPlayer->getValidMoves(topCard->getFaceValueID(), topCard->getColourID()).contains(cardToPlay)) {
				_currentTurnAction = TurnActionFactory::playCardAsAction(currentPlayerID, cardToPlay.getCardID(), cardToPlay.getFaceValueID(), cardToPlay.getColourID());
			}
		}
	}
	else if (currentTurnAction == null && currentPlayerID != bottomPlayer.getPlayerID() &&
		CurrentGameInterface.getCurrentGame().getRuleSet().allowJumpInRule()) {
		Card cardToPlay = bottomPlayer.chooseCardFromClick(mousePosition);
		if (cardToPlay != null) {
			jumpIn(bottomPlayer.getPlayerID(), cardToPlay);
		}
	}*/
}

void CurrentGameInterface::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	_overlayManager->handleMouseMove(mousePosition);
	_bottomPlayer->updateHover(mousePosition);
}

void CurrentGameInterface::jumpIn(const int playerID, Card * cardToPlay)
{
	Card* topCard = _recentCardPile.getTopCard();
	// TODO
	/*if (_currentTurnAction == nullptr && _currentPlayerID != playerID
		&& topCard->getFaceValueID() == cardToPlay->getFaceValueID()
		&& topCard->getColourID() == cardToPlay->getColourID()) {
		_currentPlayerID = playerID;
		showGeneralOverlay("JumpIn" + playerID);
		_currentTurnAction = TurnActionFactory::playCardAsAction(currentPlayerID, cardToPlay.getCardID(),
			cardToPlay.getFaceValueID(), cardToPlay.getColourID());
	}*/
}

void CurrentGameInterface::showOverlayForTurnAction()
{
	/*if (_currentTurnAction instanceof TurnActionFactory.TurnDecisionAction) {
		overlayManager.showDecisionOverlay((TurnActionFactory.TurnDecisionAction) currentTurnAction);
	}*/
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
	/*isIncreasing = !isIncreasing;
	playDirectionAnimation.setIsIncreasing(isIncreasing);*/
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
	// TODO
	return false;
}

void CurrentGameInterface::setCurrentTurnAction(TurnAction * turnAction)
{
	/*if (currentTurnAction != null) {
		queuedTurnAction = turnAction;
		if (GamePanel.DEBUG_MODE && debugShowTreeOnNewAction) {
			System.out.println("Queued action sequence:");
			TurnActionFactory.debugOutputTurnActionTree(turnAction);
		}
	}
	else {
		currentTurnAction = turnAction;
		if (GamePanel.DEBUG_MODE && debugShowTreeOnNewAction) {
			System.out.println("Set action sequence:");
			TurnActionFactory.debugOutputTurnActionTree(turnAction);
		}
	}*/
}

TurnAction * CurrentGameInterface::getCurrentTurnAction() const
{
	// TODO
	return nullptr;
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
			// TODO
			//gamePanel.showPostGame(players, ruleSet);
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

std::vector<Player*> CurrentGameInterface::createPlayersFromLobby(const std::vector<LobbyPlayer*>& playerList, sf::IntRect & bounds)
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
