#include "OverlayManager.h"

#include "TurnDecisionOverlayInterface.h"
#include "GeneralOverlayInterface.h"
#include <sstream>

OverlayManager::OverlayManager(const sf::IntRect& bounds)
	: WndInterface(bounds)
{
	for (auto &[key, overlay] : _overlays) {
		delete overlay;
	}
}

OverlayManager::~OverlayManager()
{
}

void OverlayManager::update(const int deltaTime)
{
	if (_overlayAction != _overlayAction) { // TODO: CurrentGameInterface.getCurrentGame().getCurrentTurnAction()
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
		if (true) { // TODO: CurrentGameInterface.getCurrentGame().getCurrentPlayer().getPlayerType() == Player.PlayerType.ThisPlayer
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
			// TODO
			//(dynamic_cast<PlayerFlashOverlay*>(overlayToShow))->setMessage("+"+splitOverlayName.at(1));
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
