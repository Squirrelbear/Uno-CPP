#include "KeepOrPlayOverlay.h"
#include "Card.h"

KeepOrPlayOverlay::KeepOrPlayOverlay(const sf::IntRect & bounds, const sf::Font& font)
	: WndInterface(bounds), _font(font)
{
	setEnabled(false);
	sf::Vector2f centre = sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	_buttonList.emplace_back(Button(sf::IntRect(centre.x - 150, centre.y + 100, 100, 40), "Keep", 0, font));
	_buttonList.emplace_back(Button(sf::IntRect(centre.x + 50, centre.y + 100, 100, 40), "Play", 1, font));
	_cardPosition = sf::Vector2f(centre.x - CARD_WIDTH / 2, centre.y + 100 + 20 - CARD_HEIGHT / 2);
}

KeepOrPlayOverlay::~KeepOrPlayOverlay()
{
	if (_cardForChoice != nullptr) {
		delete _cardForChoice;
	}
}

void KeepOrPlayOverlay::draw(sf::RenderWindow & renderWindow) const
{
	for (const auto& button : _buttonList) { 
		button.draw(renderWindow); 
	}
	_cardForChoice->draw(renderWindow);
}

void KeepOrPlayOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;

	if (_cardForChoice != nullptr) {
		delete _cardForChoice;
	}
	_cardForChoice = new CardFrontObjectGroup(_currentAction->getPropertyValue("faceValueID"),
								_currentAction->getPropertyValue("colourID"), _cardPosition, _font);
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
