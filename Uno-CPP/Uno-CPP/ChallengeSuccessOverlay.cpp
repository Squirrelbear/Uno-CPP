#include "ChallengeSuccessOverlay.h"

ChallengeSuccessOverlay::ChallengeSuccessOverlay(const sf::IntRect & bounds)
	: WndInterface(bounds)
{
	setEnabled(false);
	int widthDiv6 = bounds.width / 6;
	int x = bounds.left;
	int y = bounds.top;
	int heightDiv6 = bounds.height / 6;

	std::vector<sf::Vector2f> pointList;
	pointList.emplace_back(sf::Vector2f(x, y + heightDiv6 * 4));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6, y + heightDiv6 * 3));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6 * 2, y + heightDiv6 * 4));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6 * 5, y + heightDiv6 * 2));
	pointList.emplace_back(sf::Vector2f(x + bounds.width, y + heightDiv6 * 3));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6 * 2, y + bounds.height));

	_tickShape = new PolygonShape(pointList);
	_tickShape->setFillColor(sf::Color(106, 163, 22));
	_tickShape->setOutlineThickness(1);
	_tickShape->setOutlineColor(sf::Color::Black);
}

ChallengeSuccessOverlay::~ChallengeSuccessOverlay()
{
	delete _tickShape;
}

void ChallengeSuccessOverlay::update(const float deltaTime)
{
	_displayTimer -= deltaTime;
	if (_displayTimer <= 0) {
		setEnabled(false);
	}
}

void ChallengeSuccessOverlay::draw(sf::RenderWindow & renderWindow) const
{
	if (fmod(_displayTimer, 0.2f) < 0.15f) {
		renderWindow.draw(*_tickShape);
	}
}

void ChallengeSuccessOverlay::showOverlay()
{
	setEnabled(true);
	_displayTimer = 2.0f;
}
