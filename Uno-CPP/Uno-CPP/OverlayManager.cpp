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

OverlayManager::OverlayManager(const sf::IntRect & bounds, std::vector<Player*> playerList, const sf::Font& font, GameStateData gameData)
	: WndInterface(bounds), _gameState(gameData)
{
	setEnabled(true);
    WildColourSelectionOverlay* wildColourSelectionOverlay = new WildColourSelectionOverlay(sf::IntRect(bounds.width/2-100,bounds.height/2-30,200,200), font);
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

    _unoButton = new UnoButton(sf::Vector2f(bounds.left + bounds.width - 80 - 40, bounds.top + bounds.height - 60-40), font, _gameState);
    _antiUnoButton = new AntiUnoButton(sf::Vector2f(bounds.left + bounds.width - 80-40-100, bounds.top + bounds.height - 60-40), font, _gameState);
    for(int i = 0; i < playerList.size(); i++) {
        sf::Vector2f playerCentre = playerList.at(i)->getCentreOfBounds();
        PlayerFlashOverlay* skipVisualOverlay = new PlayerFlashOverlay(playerCentre, "SKIPPED", sf::Color::Red, 40, font);
		_overlays["SkipVisual"+std::to_string(i)] = skipVisualOverlay;
        PlayerFlashOverlay* drawNMessageOverlay = new PlayerFlashOverlay(playerCentre, "", sf::Color::Red, 40, font);
		_overlays["DrawN" + std::to_string(i)] = drawNMessageOverlay;
        ChallengeSuccessOverlay* challengeSuccessOverlay = new ChallengeSuccessOverlay(sf::IntRect(playerCentre.x, playerCentre.y, 100,100));
		_overlays["ChallengeSuccess" + std::to_string(i)] = challengeSuccessOverlay;
        ChallengeFailedOverlay* challengeFailedOverlay = new ChallengeFailedOverlay(sf::IntRect(playerCentre.x, playerCentre.y, 100, 100));
		_overlays["ChallengeFailed" + std::to_string(i)] = challengeFailedOverlay;
        UNOCalledOverlay* unoCalledOverlay = new UNOCalledOverlay(sf::Vector2f(playerCentre.x,playerCentre.y+20), font);
		_overlays["UNOCalled" + std::to_string(i)] = unoCalledOverlay;
        PlayerFlashOverlay* antiUnoOverlay = new PlayerFlashOverlay(sf::Vector2f(playerCentre.x,playerCentre.y+20), "!", sf::Color(226, 173, 67), 50, font);
		_overlays["AntiUnoCalled" + std::to_string(i)] = antiUnoOverlay;
        PlayerFlashOverlay* jumpInOverlay = new PlayerFlashOverlay(sf::Vector2f(playerCentre.x,playerCentre.y+20), "JUMPED IN", sf::Color(255,215,0), 40, font);
		_overlays["JumpIn" + std::to_string(i)] = jumpInOverlay;
    }
	_overlays["UnoButton"] = _unoButton;
	_overlays["antiUnoButton"] = _antiUnoButton;
}

OverlayManager::~OverlayManager()
{
	for (auto &[key, overlay] : _overlays) {
		delete overlay;
	}
}

void OverlayManager::update(const float deltaTime, const TurnAction* currentTurnAction)
{
	if (_overlayAction != currentTurnAction) {
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
			TurnDecisionOverlayInterface* overlayToShow = dynamic_cast<TurnDecisionOverlayInterface*>(_overlays[currentAction->getFlagName()]);
			if (overlayToShow != nullptr) {
				overlayToShow->showOverlay(currentAction);
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
	GeneralOverlayInterface* overlayToShow = dynamic_cast<GeneralOverlayInterface*>(_overlays[splitOverlayName.at(0)]);
	if (overlayToShow != nullptr) {
		overlayToShow->showOverlay();
		if (splitOverlayName.at(0).rfind("DrawN", 0) == 0) {
			(dynamic_cast<PlayerFlashOverlay*>(_overlays[splitOverlayName.at(0)]))->setMessage("+"+splitOverlayName.at(1));
		}
	}
}

void OverlayManager::hideAllDecisionOverlays()
{
	for (auto &[key, overlay] : _overlays) {
		if (dynamic_cast<TurnDecisionOverlayInterface*>(overlay) != nullptr) {
			overlay->setEnabled(false);
		}
	}
	setEnabled(false);
}

PlayerUpdateResult OverlayManager::getUnoButtonsState()
{
	bool buttonTriggered = _unoButton->isTriggeredReset();
	if (buttonTriggered) {
		return { PlayerUpdateResultState::PlayerCalledUno, nullptr, _unoButton->getActionID(), nullptr };
	}
	buttonTriggered = _antiUnoButton->isTriggeredReset();
	if (buttonTriggered) {
		return { PlayerUpdateResultState::PlayerCalledAntiUno, nullptr, _antiUnoButton->getActionID(), nullptr };
	}

	return { PlayerUpdateResultState::PlayerDidNothing, nullptr, -1, nullptr };
}
