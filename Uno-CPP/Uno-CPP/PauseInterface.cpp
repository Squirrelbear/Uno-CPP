#include "PauseInterface.h"

PauseInterface::PauseInterface(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font)
	: WndInterface(bounds)
{
	initialiseInterface(bounds, gameBounds, font);
}

PauseInterface::~PauseInterface()
{
	delete _interfaceBackground;
}

void PauseInterface::draw(sf::RenderWindow & renderWindow) const
{
	_interfaceBackground->draw(renderWindow);
}

void PauseInterface::initialiseInterface(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font)
{
	_interfaceBackground = new DrawableObjectGroup();
	DrawableShape* transparentBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(gameBounds.width, gameBounds.height)), sf::Color(144, 143, 143, 204));
	_interfaceBackground->addChild(transparentBackground);
	DrawableShape* mainBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width - 2, bounds.height - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(bounds.left + 1, bounds.top + 1));
	mainBackground->setBorder(1, sf::Color::Black);
	_interfaceBackground->addChild(mainBackground);
	DrawableShape* leftBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(160 - 2, 90 - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(170 + 1, 300 + 1));
	_interfaceBackground->addChild(leftBackground);
	leftBackground->setBorder(1, sf::Color::Black);
	DrawableShape* rightBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(410 - 2, 300 - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(790+1, 220+1));
	_interfaceBackground->addChild(rightBackground);
	rightBackground->setBorder(1, sf::Color::Black);

	DrawableText* titleText = new DrawableText(sf::Vector2f(0, 0), "Paused", font, 40, sf::Color::Black, sf::Text::Bold);
	titleText->setOffset(sf::Vector2f(bounds.left + bounds.width / 2 - titleText->getTextWidth() / 2, bounds.top));
	_interfaceBackground->addChild(titleText);


	_interfaceBackground->setPositionWithOffset(sf::Vector2f(gameBounds.left, gameBounds.top));
}
