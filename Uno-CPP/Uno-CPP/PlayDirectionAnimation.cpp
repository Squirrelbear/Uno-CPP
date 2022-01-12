#include "PlayDirectionAnimation.h"

PlayDirectionAnimation::PlayDirectionAnimation(const sf::Vector2f centre, const int radiusFromCentre, const int indicatorSize)
	: _centre(centre), _radiusFromCentre(radiusFromCentre),
	  _movingObject1(indicatorSize), _movingObject2(indicatorSize)
{
	_currentAngle = 0;
	_isIncreasing = true;
	_movingObject1.setFillColor(sf::Color::Black);
	_movingObject2.setFillColor(sf::Color::Black);
}

void PlayDirectionAnimation::update(const int deltaTime)
{
	if (_isIncreasing) { 
		_currentAngle += deltaTime / 1000.0f; 
	}
	else { 
		_currentAngle -= deltaTime / 1000.0f; 
	}
	if (_currentAngle > 3.141592654f * 2 || _currentAngle < -3.141592654f * 2) {
		_currentAngle = 0; 
	}

	_movingObject1.setPosition(sf::Vector2f(_centre.x + _radiusFromCentre * cos(_currentAngle),
											_centre.y + _radiusFromCentre * sin(_currentAngle)));
	_movingObject2.setPosition(sf::Vector2f(_centre.x - _radiusFromCentre * cos(_currentAngle),
											_centre.y - _radiusFromCentre * sin(_currentAngle)));
}

void PlayDirectionAnimation::draw(sf::RenderWindow & renderWindow) const
{
	renderWindow.draw(_movingObject1);
	renderWindow.draw(_movingObject2);
}

void PlayDirectionAnimation::setIsIncreasing(bool isIncreasing)
{
	_isIncreasing = isIncreasing;
}
