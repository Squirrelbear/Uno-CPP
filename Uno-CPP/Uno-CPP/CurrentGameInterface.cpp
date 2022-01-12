#include "CurrentGameInterface.h"



CurrentGameInterface::CurrentGameInterface(const sf::IntRect& bounds, const sf::Font& font, const std::vector<Player*>& playerList, RuleSet* ruleSet, std::default_random_engine& randomEngine)
	: WndInterface(bounds), _deck(sf::Vector2f(bounds.width/2-30-160, bounds.height / 2 - 45), font, randomEngine), 
	_overlayManager(bounds,playerList), _recentCardPile(sf::Vector2f(bounds.width / 2 - 30, bounds.height / 2 - 45))
{
}


CurrentGameInterface::~CurrentGameInterface()
{
}

void CurrentGameInterface::update(const float deltaTime)
{
}

void CurrentGameInterface::draw(sf::RenderWindow & renderWindow) const
{
}

void CurrentGameInterface::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
}

void CurrentGameInterface::handleMouseMove(const sf::Vector2i & mousePosition)
{
}

void CurrentGameInterface::jumpIn(const int playerID, Card * cardToPlay)
{
}

void CurrentGameInterface::showOverlayForTurnAction()
{
}

void CurrentGameInterface::showGeneralOverlay(const std::string& overlayName)
{
}

void CurrentGameInterface::revealHands()
{
}

void CurrentGameInterface::sortHand()
{
}

Player * CurrentGameInterface::getBottomPlayer() const
{
	return nullptr;
}

void CurrentGameInterface::toggleTurnDirection()
{
}

void CurrentGameInterface::moveToNextPlayer()
{
}

void CurrentGameInterface::applyAntiUno(const int playerID)
{
}

bool CurrentGameInterface::isIncreasing() const
{
	return false;
}

void CurrentGameInterface::setCurrentTurnAction(TurnAction * turnAction)
{
}

TurnAction * CurrentGameInterface::getCurrentTurnAction() const
{
	return nullptr;
}

RuleSet * CurrentGameInterface::getRuleSet() const
{
	return nullptr;
}

Player * CurrentGameInterface::getCurrentPlayer() const
{
	return nullptr;
}

std::vector<Player*> CurrentGameInterface::getAllPlayers() const
{
	return std::vector<Player*>();
}

Player * CurrentGameInterface::getPlayerByID(const int playerID) const
{
	return nullptr;
}

Deck * CurrentGameInterface::getDeck() const
{
	return nullptr;
}

void CurrentGameInterface::checkForEndOfRound()
{
}

void CurrentGameInterface::updateTurnAction()
{
}

void CurrentGameInterface::updateUNOState()
{
}

std::vector<Player*> CurrentGameInterface::createPlayersFromLobby(const std::vector<LobbyPlayer*>& playerList, sf::IntRect & bounds)
{
	return std::vector<Player*>();
}

sf::IntRect CurrentGameInterface::getPlayerRect(const int direction, const sf::IntRect & bounds)
{
	return sf::IntRect();
}
