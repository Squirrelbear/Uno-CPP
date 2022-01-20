#include "StackChoiceOverlay.h"

StackChoiceOverlay::StackChoiceOverlay(const sf::IntRect & bounds, const sf::Font & font, const GameStateData& gameData)
	: WndInterface(bounds)
{
	setEnabled(false);
	_declineButton = new Button(sf::IntRect(bounds.left + bounds.width / 2 - 50, bounds.top + bounds.height / 2 + 100, 100, 40), "Decline", 0, font);

	_playerReference = gameData.bottomPlayer;
}

StackChoiceOverlay::~StackChoiceOverlay()
{
	delete _declineButton;
}

void StackChoiceOverlay::draw(sf::RenderWindow & renderWindow) const
{
	_declineButton->draw(renderWindow);
}

void StackChoiceOverlay::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	if (_declineButton->isPositionInside(mousePosition)) {
		_currentAction->injectFlagProperty(0);
		setEnabled(false);
		return;
	}

	auto clickedCard = _playerReference->chooseCardFromClick(mousePosition);
	if (clickedCard != nullptr && clickedCard->getFaceValueID() == 10) {
		_currentAction->injectProperty("faceValueID", clickedCard->getFaceValueID());
		_currentAction->injectProperty("colourID", clickedCard->getColourID());
		_currentAction->injectProperty("cardID", clickedCard->getUniqueCardID());
		_currentAction->injectFlagProperty(1);
		setEnabled(false);
	}
}

void StackChoiceOverlay::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	_declineButton->setHovering(_declineButton->isPositionInside(mousePosition));
}

void StackChoiceOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}
