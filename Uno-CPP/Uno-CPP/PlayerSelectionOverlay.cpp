#include "PlayerSelectionOverlay.h"

PlayerSelectionOverlay::PlayerSelectionOverlay(const sf::IntRect & bounds, const sf::Font & font, std::vector<Player*> playerList)
	: WndInterface(bounds)
{
	setEnabled(false);
	for (int i = 0; i < playerList.size(); i++) {
		if (playerList.at(i)->getPlayerType() != Player::PlayerType::ThisPlayer) {
			sf::Vector2f centre = playerList.at(i)->getCentreOfBounds();
			_buttonList.emplace_back(Button(sf::IntRect(centre.x - 100, centre.y - 20, 200, 40), "Choose Player", i, font));
		}
	}
}

PlayerSelectionOverlay::~PlayerSelectionOverlay()
{
}

void PlayerSelectionOverlay::draw(sf::RenderWindow & renderWindow) const
{
	for (const auto& button : _buttonList) {
		button.draw(renderWindow);
	}
}

void PlayerSelectionOverlay::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		if (button.isPositionInside(mousePosition)) {
			setEnabled(false);
			_currentAction->injectFlagProperty(button.getActionID());
			break;
		}
	}
}

void PlayerSelectionOverlay::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
}

void PlayerSelectionOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}
