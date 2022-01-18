#include "StatusOverlay.h"

StatusOverlay::StatusOverlay(const sf::IntRect & bounds, const sf::Font & font, const GameStateData& gameData)
	: WndInterface(bounds), _gameData(gameData)
{
	_background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(20, 60)), sf::Color(184, 154, 143, 204));
	_statusText = new DrawableText(sf::Vector2f(0, 0), "NOTSET", font, 20, sf::Color::Black, sf::Text::Bold);
	_timeOutText = new DrawableText(sf::Vector2f(0, 0), "NOTSET", font, 20, sf::Color::Black, sf::Text::Bold);
	_timeOutShadowText = new DrawableText(sf::Vector2f(0, 0), "NOTSET", font, 20, sf::Color::Black, sf::Text::Bold);
}

StatusOverlay::~StatusOverlay()
{
	delete _statusText;
	delete _timeOutText;
	delete _background;
	delete _timeOutShadowText;
}

void StatusOverlay::update(const float deltaTime)
{
	_timeOut -= deltaTime;
	updateTimeOutLabel();
}

void StatusOverlay::draw(sf::RenderWindow & renderWindow) const
{
	_background->draw(renderWindow);
	_statusText->draw(renderWindow);
	_timeOutShadowText->draw(renderWindow);
	_timeOutText->draw(renderWindow);
}

void StatusOverlay::showOverlay(TurnDecisionAction * currentAction)
{
	setEnabled(true);
	std::string statusText = createContextString(currentAction);
	updateStatusLabel(statusText);
	_timeOut = _gameData.ruleSet->getDefaultTimeOut();
	updateTimeOutLabel();
}

std::string StatusOverlay::createContextString(TurnDecisionAction * currentAction)
{
	std::string result;

	if(currentAction->getFlagName() == "keepOrPlay") 
		result = "choosing to Keep or Play.";
	else if (currentAction->getFlagName() == "wildColour") 
		result = "choosing Wild Colour.";
	else if (currentAction->getFlagName() == "isChallenging") 
		result = "choosing Response to +4.";
	else if (currentAction->getFlagName() == "otherPlayer")
		result = "choosing Other Player to Swap With.";
	else 
		result = "thinking...";

	if (_gameData.players->at(*_gameData.currentPlayerID)->getPlayerType() == Player::PlayerType::ThisPlayer) {
		return "You are " + result;
	}
	return _gameData.players->at(*_gameData.currentPlayerID)->getPlayerName() + " is " + result;
}

void StatusOverlay::updateStatusLabel(const std::string& status)
{
	_statusText->setText(status);
	int strWidth = _statusText->getTextWidth();
	_statusText->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2, _centre.y - 20));
	_background->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2 - 10, _centre.y - 65));
	_background->setSize(sf::Vector2f(_centre.x - strWidth / 2 - 10, _centre.y - 65));
}

void StatusOverlay::updateTimeOutLabel()
{
	if (_timeOut < 0) {
		_timeOut = 0;
	}
	_timeOutText->setColour(_timeOut < 6 ? sf::Color::Red : sf::Color::Yellow);

	std::string time = static_cast<int>(_timeOut) + "s";
	_timeOutText->setText(time);
	_timeOutShadowText->setText(time);
	int strWidth = _statusText->getTextWidth();
	_timeOutText->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2, _centre.y - 40));
	_timeOutShadowText->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2 - 2, _centre.y - 40 + 2));
}
