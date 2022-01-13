#include "WildColourSelectionOverlay.h"
#include "DrawableShape.h"
#include "DrawableText.h"
#include "ArcShape.h"
#include "Card.h"

WildColourSelectionOverlay::WildColourSelectionOverlay(const sf::IntRect & bounds, const sf::Font & font)
	: WndInterface(bounds), _interactionRect(bounds)
{
	setEnabled(false);

	_background = new DrawableObjectGroup();
	DrawableShape* backgroundRect = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width + 40-2, bounds.height + 60-2)),
		sf::Color::Black, sf::Vector2f(bounds.left - 20+1, bounds.top - 40+1));
	backgroundRect->setBorder(1, sf::Color::White);
	_background->addChild(backgroundRect);
	DrawableText* title = new DrawableText(sf::Vector2f(0, 0), "Choose Colour", font, 20, sf::Color::White, sf::Text::Bold);
	title->setOffset(sf::Vector2f(bounds.left + bounds.width / 2 - title->getTextWidth() / 2, bounds.top - 5));
	_background->addChild(title);
	_background->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));

	for (int i = 0; i < 4; i++) {
		ArcShape* arcShape = new ArcShape(90 * i, 90 * (i + 1), sf::Vector2f( bounds.width / 2, bounds.height/2));
		DrawableShape* arc = new DrawableShape(arcShape, Card::getColourByID(i), sf::Vector2f(bounds.left, bounds.top));
		_background->addChild(arc);
	}
}

WildColourSelectionOverlay::~WildColourSelectionOverlay()
{
	delete _background;
}

void WildColourSelectionOverlay::draw(sf::RenderWindow & renderWindow) const
{
	_background->draw(renderWindow);
}

void WildColourSelectionOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}

void WildColourSelectionOverlay::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	handleMouseMove(mousePosition);
	if (hoveredRegion != -1) {
		_currentAction->injectProperty("colourID", hoveredRegion);
		_currentAction->injectFlagProperty(1);
		setEnabled(false);
	}
}

void WildColourSelectionOverlay::handleMouseMove(const sf::Vector2i & mousePosition)
{
	hoveredRegion = -1;
	if (_interactionRect.isPositionInside(mousePosition)) {
		hoverX = (mousePosition.x - _bounds.left) / (_bounds.width / 2);
		hoverY = (mousePosition.y - _bounds.top) / (_bounds.height / 2);
		if (hoverX == 0 && hoverY == 0) hoveredRegion = 2;
		else if (hoverX == 1 && hoverY == 0) hoveredRegion = 1;
		else if (hoverX == 1 && hoverY == 1) hoveredRegion = 0;
		else if (hoverX == 0 && hoverY == 1) hoveredRegion = 3;
	}
}
