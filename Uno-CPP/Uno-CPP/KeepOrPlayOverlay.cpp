#include "KeepOrPlayOverlay.h"

KeepOrPlayOverlay::~KeepOrPlayOverlay()
{
	if (_cardForChoice != nullptr) {
		delete _cardForChoice;
	}
}

void KeepOrPlayOverlay::draw(sf::RenderWindow & renderWindow) const
{
	for (const auto& button : _buttonList) { 
		button.draw(renderWindow)); 
	}
	// TODO card draw method needed
	//_cardForChoice->draw(renderWindow);
}

void KeepOrPlayOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	// TODO provide access to storedData
	/*_cardForChoice = new Card(_currentAction.storedData.get("faceValueID"),
		currentAction.storedData.get("colourID"),
		currentAction.storedData.get("cardID"));*/
	_cardForChoice->setPosition(_cardPosition);
	setEnabled(true);
}

void KeepOrPlayOverlay::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
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

void KeepOrPlayOverlay::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
}
