#include "OverlayManager.h"

#include "WildColourSelectionOverlay.h"
#include "KeepOrPlayOverlay.h"
#include "PlayerFlashOverlay.h"
#include "ChallengeOverlay.h"
#include "StatusOverlay.h"
#include "PlayerSelectionOverlay.h"
#include "StackChoiceOverlay.h"
#include "ChallengeSuccessOverlay.h"
#include "ChallengeFailedOverlay.h"
#include "UNOCalledOverlay.h"
#include "UnoButton.h"
#include "AntiUnoButton.h"
#include "TurnDecisionOverlayInterface.h"
#include "GeneralOverlayInterface.h"
#include <sstream>
#include "Game.h"

OverlayManager::OverlayManager(const sf::IntRect & bounds, std::vector<Player*> playerList, const sf::Font& font, GameStateData gameData)
	: WndInterface(bounds), _gameState(gameData)
{
	setEnabled(true);
    WildColourSelectionOverlay* wildColourSelectionOverlay = new WildColourSelectionOverlay(sf::IntRect(bounds.width/2-100,bounds.height/2-100,200,200), font);
    KeepOrPlayOverlay* keepOrPlayOverlay = new KeepOrPlayOverlay(bounds, font);
    PlayerSelectionOverlay* playerSelectionOverlay = new PlayerSelectionOverlay(bounds, playerList, font);
    StatusOverlay* statusOverlay = new StatusOverlay(bounds, font, _gameState);
    ChallengeOverlay* challengeOverlay = new ChallengeOverlay(bounds, font, _gameState);
    StackChoiceOverlay* stackChoiceOverlay = new StackChoiceOverlay(bounds, font, _gameState);
    _overlays["wildColour"] = wildColourSelectionOverlay;
	_overlays["keepOrPlay"] = keepOrPlayOverlay;
	_overlays["otherPlayer"] = playerSelectionOverlay;
	_overlays["statusOverlay"] = statusOverlay;
	_overlays["isChallenging"] = challengeOverlay;
	_overlays["isStacking"] = stackChoiceOverlay;

    UnoButton* unoButton = new UnoButton(sf::Vector2f(bounds.left + bounds.width - 80 - 40, bounds.top + bounds.height - 60-40), font, _gameState);
    AntiUnoButton* antiUnoButton = new AntiUnoButton(sf::Vector2f(bounds.left + bounds.width - 80-40-100, bounds.top + bounds.height - 60-40), font, _gameState);
    for(int i = 0; i < playerList.size(); i++) {
        sf::Vector2f playerCentre = playerList.at(i)->getCentreOfBounds();
        PlayerFlashOverlay* skipVisualOverlay = new PlayerFlashOverlay(playerCentre, "SKIPPED", sf::Color::Red, 40, font);
		_overlays["SkipVisual"+i] = skipVisualOverlay;
        PlayerFlashOverlay* drawNMessageOverlay = new PlayerFlashOverlay(playerCentre, "", sf::Color::Red, 40, font);
		_overlays["DrawN"+i] = drawNMessageOverlay;
        ChallengeSuccessOverlay* challengeSuccessOverlay = new ChallengeSuccessOverlay(sf::IntRect(playerCentre.x, playerCentre.y, 100,100));
		_overlays["ChallengeSuccess"+i] = challengeSuccessOverlay;
        ChallengeFailedOverlay* challengeFailedOverlay = new ChallengeFailedOverlay(sf::IntRect(playerCentre.x, playerCentre.y, 100, 100));
		_overlays["ChallengeFailed"+i] = challengeFailedOverlay;
        UNOCalledOverlay* unoCalledOverlay = new UNOCalledOverlay(sf::Vector2f(playerCentre.x,playerCentre.y+20), font);
		_overlays["UNOCalled"+i] = unoCalledOverlay;
        PlayerFlashOverlay* antiUnoOverlay = new PlayerFlashOverlay(sf::Vector2f(playerCentre.x,playerCentre.y+20), "!", sf::Color(226, 173, 67), 50, font);
		_overlays["AntiUnoCalled"+i] = antiUnoOverlay;
        PlayerFlashOverlay* jumpInOverlay = new PlayerFlashOverlay(sf::Vector2f(playerCentre.x,playerCentre.y+20), "JUMPED IN", sf::Color(255,215,0), 40, font);
		_overlays["JumpIn"+i] = jumpInOverlay;
    }
	_overlays["UnoButton"] = unoButton;
	_overlays["antiUnoButton"] = antiUnoButton;
}

OverlayManager::~OverlayManager()
{
	for (auto &[key, overlay] : _overlays) {
		delete overlay;
	}
}

void OverlayManager::update(const float deltaTime)
{
	if (_overlayAction != Game::getCurrentGame()->getCurrentTurnAction()) {
		_overlayAction = nullptr;
		hideAllDecisionOverlays();
	}

	for (auto &[key, overlay] : _overlays) {
		if (overlay->isEnabled()) {
			overlay->update(deltaTime);
		}
	}
}

void OverlayManager::draw(sf::RenderWindow & renderWindow) const
{
	for (auto const&[key, overlay] : _overlays) {
		if (overlay->isEnabled()) {
			overlay->draw(renderWindow);
		}
	}
}

void OverlayManager::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	for (auto &[key, overlay] : _overlays) {
		if (overlay->isEnabled()) {
			overlay->handleMousePress(mousePosition, isLeft);
		}
	}
}

void OverlayManager::handleMouseMove(const sf::Vector2i & mousePosition)
{
	for (auto &[key, overlay] : _overlays) {
		if (overlay->isEnabled()) {
			overlay->handleMouseMove(mousePosition);
		}
	}
}

void OverlayManager::showDecisionOverlay(TurnDecisionAction * currentAction)
{
	if (currentAction->requiresTimeout()) {
		setEnabled(true);
		if (_gameState.players->at(*_gameState.currentPlayerID)->getPlayerType() == Player::PlayerType::ThisPlayer) {
			WndInterface* overlayToShow = _overlays[currentAction->getFlagName()];
			if (typeid(overlayToShow) == typeid(TurnDecisionOverlayInterface)) {
				(dynamic_cast<TurnDecisionOverlayInterface*>(overlayToShow))->showOverlay(currentAction);
			}
		}
		_overlayAction = currentAction;
		(dynamic_cast<TurnDecisionOverlayInterface*>(_overlays["statusOverlay"]))->showOverlay(currentAction);
	}
}

void OverlayManager::showGeneralOverlay(const std::string & overlayName)
{
	// Split to allow for parameter inputs separated by ;
	std::vector<std::string> splitOverlayName{};
	std::stringstream sstream(overlayName);
	std::string part;
	while (std::getline(sstream, part, ';')) {
		splitOverlayName.push_back(part);
	}
	WndInterface* overlayToShow = _overlays[splitOverlayName.at(0)];
	if (typeid(overlayToShow) == typeid(GeneralOverlayInterface)) {
		(dynamic_cast<GeneralOverlayInterface*>(overlayToShow))->showOverlay();
		if (splitOverlayName.at(0).rfind("DrawN", 0) == 0) {
			(dynamic_cast<PlayerFlashOverlay*>(overlayToShow))->setMessage("+"+splitOverlayName.at(1));
		}
	}
}

void OverlayManager::hideAllDecisionOverlays()
{
	for (auto &[key, overlay] : _overlays) {
		if (typeid(overlay) == typeid(TurnDecisionOverlayInterface)) {
			overlay->setEnabled(false);
		}
	}
	setEnabled(false);
}
