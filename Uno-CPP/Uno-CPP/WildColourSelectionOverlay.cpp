#include "WildColourSelectionOverlay.h"
#include "DrawableText.h"
#include "ArcShape.h"
#include "Card.h"

WildColourSelectionOverlay::WildColourSelectionOverlay(const sf::IntRect & bounds, const sf::Font & font)
	: WndInterface(bounds), _interactionRect(bounds)
{
	setEnabled(false);

	// TODO Need to handle visual change based on selection.

	_background = new DrawableObjectGroup();
	DrawableShape* backgroundRect = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width + 40-2, bounds.height + 60-2)),
		sf::Color::Black, sf::Vector2f(bounds.left - 20+1, bounds.top - 40+1));
	backgroundRect->setBorder(1, sf::Color::White);
	_background->addChild(backgroundRect);
	DrawableText* title = new DrawableText(sf::Vector2f(0, 0), "Choose Colour", font, 20, sf::Color::White, sf::Text::Bold);
	title->setOffset(sf::Vector2f(bounds.left + bounds.width / 2 - title->getTextWidth() / 2, bounds.top - 35));
	_background->addChild(title);
	
	for (int i = 0; i < 4; i++) {
		ArcShape* arcShape = new ArcShape(90 * i, 90 * (i + 1), sf::Vector2f( bounds.width / 2, bounds.height/2));
		DrawableShape* arc = new DrawableShape(arcShape, Card::getColourByID(i), sf::Vector2f(bounds.left, bounds.top));
		_background->addChild(arc);
	}
	for (int i = 0; i < 4; i++) {
		ArcShape* arcShape = new ArcShape(90 * i, 90 * (i + 1), sf::Vector2f(bounds.width / 2+10, bounds.height / 2+10));
		_hoverArcs.emplace_back(new DrawableShape(arcShape, Card::getColourByID(i), sf::Vector2f(bounds.left-10, bounds.top-10)));
		_hoverArcs.at(_hoverArcs.size() - 1)->setPositionWithOffset(sf::Vector2f(0, 0));
	}

	_background->setPositionWithOffset(sf::Vector2f(0,0));
}

WildColourSelectionOverlay::~WildColourSelectionOverlay()
{
	delete _background;
	for (auto p : _hoverArcs) {
		delete p;
	}
}

void WildColourSelectionOverlay::draw(sf::RenderWindow & renderWindow) const
{
	_background->draw(renderWindow);
	if (_hoveredRegion != -1) {
		_hoverArcs.at(_hoveredRegion)->draw(renderWindow);
	}
}

void WildColourSelectionOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}

void WildColourSelectionOverlay::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	handleMouseMove(mousePosition);
	if (_hoveredRegion != -1) {
		_currentAction->injectProperty("colourID", _hoveredRegion);
		_currentAction->injectFlagProperty(1);
		setEnabled(false);
	}
}

void WildColourSelectionOverlay::handleMouseMove(const sf::Vector2i & mousePosition)
{
	_hoveredRegion = -1;
	if (_interactionRect.isPositionInside(mousePosition)) {
		_hoverX = (mousePosition.x - _bounds.left) / (_bounds.width / 2);
		_hoverY = (mousePosition.y - _bounds.top) / (_bounds.height / 2);
		if (_hoverX == 0 && _hoverY == 0) _hoveredRegion = 2;
		else if (_hoverX == 1 && _hoverY == 0) _hoveredRegion = 3;
		else if (_hoverX == 1 && _hoverY == 1) _hoveredRegion = 0;
		else if (_hoverX == 0 && _hoverY == 1) _hoveredRegion = 1;
	}
}
