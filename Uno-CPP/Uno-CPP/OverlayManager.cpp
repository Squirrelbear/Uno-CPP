#include "OverlayManager.h"

#include "TurnDecisionOverlayInterface.h"
#include "GeneralOverlayInterface.h"
#include <sstream>

OverlayManager::OverlayManager(const sf::IntRect & bounds, std::vector<Player*> playerList)
	: WndInterface(bounds)
{
	/*
	setEnabled(true);
        overlays = new HashMap<>();
        WildColourSelectorOverlay wildColourSelectorOverlay = new WildColourSelectorOverlay(new Position(bounds.width/2-100,bounds.height/2-100),200,200);
        KeepOrPlayOverlay keepOrPlayOverlay = new KeepOrPlayOverlay(new Rectangle(new Position(0,0), bounds.width, bounds.height));
        PlayerSelectionOverlay playerSelectionOverlay = new PlayerSelectionOverlay(new Rectangle(new Position(0,0), bounds.width, bounds.height), playerList);
        StatusOverlay statusOverlay = new StatusOverlay(new Rectangle(new Position(0,0), bounds.width, bounds.height));
        ChallengeOverlay challengeOverlay = new ChallengeOverlay(bounds);
        StackChoiceOverlay stackChoiceOverlay = new StackChoiceOverlay(bounds);
        overlays.put("wildColour", wildColourSelectorOverlay);
        overlays.put("keepOrPlay", keepOrPlayOverlay);
        overlays.put("otherPlayer", playerSelectionOverlay);
        overlays.put("statusOverlay", statusOverlay);
        overlays.put("isChallenging", challengeOverlay);
        overlays.put("isStacking", stackChoiceOverlay);

        UnoButton unoButton = new UnoButton(new Position(bounds.position.x + bounds.width - UnoButton.WIDTH-40,
                bounds.position.y + bounds.height - UnoButton.HEIGHT-40));
        AntiUnoButton antiUnoButton = new AntiUnoButton(new Position(bounds.position.x + bounds.width - UnoButton.WIDTH-40-100,
                bounds.position.y + bounds.height - UnoButton.HEIGHT-40));
        for(int i = 0; i < playerList.size(); i++) {
            Position playerCentre = playerList.get(i).getCentreOfBounds();
            PlayerFlashOverlay skipVisualOverlay = new PlayerFlashOverlay(playerCentre, "SKIPPED", Color.RED, 40);
            overlays.put("SkipVisual"+i,skipVisualOverlay);
            PlayerFlashOverlay drawNMessageOverlay = new PlayerFlashOverlay(playerCentre, "", Color.RED, 40);
            overlays.put("DrawN"+i,drawNMessageOverlay);
            ChallengeSuccessOverlay challengeSuccessOverlay = new ChallengeSuccessOverlay(new Rectangle(playerCentre, 100,100));
            overlays.put("ChallengeSuccess"+i,challengeSuccessOverlay);
            ChallengeFailedOverlay challengeFailedOverlay = new ChallengeFailedOverlay(new Rectangle(playerCentre, 100,100));
            overlays.put("ChallengeFailed"+i,challengeFailedOverlay);
            UNOCalledOverlay unoCalledOverlay = new UNOCalledOverlay(new Position(playerCentre.x,playerCentre.y+20));
            overlays.put("UNOCalled"+i,unoCalledOverlay);
            PlayerFlashOverlay antiUnoOverlay = new PlayerFlashOverlay(new Position(playerCentre.x,playerCentre.y+20),
                    "!", new Color(226, 173, 67), 50);
            overlays.put("AntiUnoCalled"+i,antiUnoOverlay);
            PlayerFlashOverlay jumpInOverlay = new PlayerFlashOverlay(new Position(playerCentre.x,playerCentre.y+20),
                    "JUMPED IN", Color.ORANGE, 40);
            overlays.put("JumpIn"+i, jumpInOverlay);
        }
        overlays.put("UnoButton", unoButton);
        overlays.put("antiUnoButton", antiUnoButton);
	*/
}

OverlayManager::~OverlayManager()
{
	for (auto &[key, overlay] : _overlays) {
		delete overlay;
	}
}

void OverlayManager::update(const float deltaTime)
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
