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
	DrawableShape* mainBackgroundBorder = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width, bounds.height)), sf::Color(0, 0, 0, 50), sf::Vector2f(bounds.left, bounds.top));
	_interfaceBackground->addChild(mainBackgroundBorder);
	DrawableShape* leftBackgroundBorder = new DrawableShape(new sf::RectangleShape(sf::Vector2f(160, 90)), sf::Color(0, 0, 0, 50), sf::Vector2f(170,300));
	_interfaceBackground->addChild(leftBackgroundBorder);
	DrawableShape* rightBackgroundBorder = new DrawableShape(new sf::RectangleShape(sf::Vector2f(410, 300)), sf::Color(0, 0, 0, 50), sf::Vector2f(790,220));
	_interfaceBackground->addChild(rightBackgroundBorder);
	DrawableShape* mainBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width - 2, bounds.height - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(bounds.left + 1, bounds.top + 1));
	_interfaceBackground->addChild(mainBackground);
	DrawableShape* leftBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(160 - 2, 90 - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(170 + 1, 300 + 1));
	_interfaceBackground->addChild(leftBackground);
	DrawableShape* rightBackground = new DrawableShape(new sf::RectangleShape(sf::Vector2f(410 - 2, 300 - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(790+1, 220+1));
	_interfaceBackground->addChild(rightBackground);

	DrawableText* titleText = new DrawableText(sf::Vector2f(0, 0), "Paused", font, 40, sf::Color::Black, sf::Text::Bold);
	titleText->setOffset(sf::Vector2f(bounds.left + bounds.width / 2 - titleText->getTextWidth() / 2, bounds.top));
	_interfaceBackground->addChild(titleText);


	_interfaceBackground->setPositionWithOffset(sf::Vector2f(gameBounds.left, gameBounds.top));
}
