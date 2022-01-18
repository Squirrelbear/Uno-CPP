#include "ChallengeOverlay.h"

ChallengeOverlay::ChallengeOverlay(const sf::IntRect & bounds, const sf::Font & font, const GameStateData& gameData)
	: WndInterface(bounds)
{
	setEnabled(false);
	sf::Vector2f centre(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	if (gameData.ruleSet->getNoBluffingRule()) {
		_buttonList.emplace_back(Button(sf::IntRect(centre.x - 150, centre.y + 100, 100, 40), "Challenge", 1, font));
	}
	_buttonList.emplace_back(Button(sf::IntRect(centre.x + 50, centre.y + 100, 100, 40), "Decline", 0, font));

	_allowStacking = gameData.ruleSet->canStackCards();
	_playerReference = gameData.bottomPlayer;
}

void ChallengeOverlay::draw(sf::RenderWindow & renderWindow) const
{
	for (const auto& button : _buttonList) {
		button.draw(renderWindow);
	}
}

void ChallengeOverlay::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
}

void ChallengeOverlay::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	// Test if a button was clicked and return a non-chaining result.
	for (const auto& button : _buttonList) {
		if (button.isPositionInside(mousePosition)) {
			_currentAction->injectProperty("isChaining", 0);
			_currentAction->injectFlagProperty(button.getActionID());
			setEnabled(false);
			return;
		}
	}

	// If stacking is allowed and a draw 4 card has been clicked then inject properties to force a chain.
	if (_allowStacking) {
		Card* clickedCard = _playerReference->chooseCardFromClick(mousePosition);
		if (clickedCard != nullptr && clickedCard->getFaceValueID() == 13) {
			_currentAction->injectProperty("faceValueID", clickedCard->getFaceValueID());
			_currentAction->injectProperty("colourID", clickedCard->getColourID());
			_currentAction->injectProperty("cardID", clickedCard->getUniqueCardID());
			_currentAction->injectProperty("isChaining", 1);
			_currentAction->injectFlagProperty(0);
			setEnabled(false);
		}
	}
}

void ChallengeOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}
