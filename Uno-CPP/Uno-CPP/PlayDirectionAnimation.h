#pragma once

#include <SFML/Graphics.hpp>

/**
 * Uno
 *
 * PlayDirectionAnimation class:
 * Represents a pair for orbs that circle clockwise or anti-clockwise to
 * show the direction of play for turn order.
 * 
 * @author Peter Mitchell
 * @version 2022.1
 */
class PlayDirectionAnimation
{
public:
	/**
	 * Initialises the ovals to represent showing play direction.
	 *
	 * @param centre Centre of the orbit.
	 * @param radiusFromCentre Distance the orbs are spaced out from the centre.
	 * @param indicatorSize The width and height of the ovals.
	 */
	PlayDirectionAnimation(const sf::Vector2f centre, const int radiusFromCentre, const int indicatorSize);
	virtual ~PlayDirectionAnimation() = default;

	/**
	 * Moves the two ovals around in a circle motion around the centre.
	 *
	 * @param deltaTime Time since last update.
	 */
	void update(const float deltaTime);

	// Draws the two ovals representing play direction.
	void draw(sf::RenderWindow& renderWindow) const;

	/**
	 * Changes the direction of the visual.
	 *
	 * @param isIncreasing When true the ovals move clockwise, when false they move anti-clockwise.
	 */
	void setIsIncreasing(bool isIncreasing);
private:
	// Centre of the orbit.
	const sf::Vector2f _centre;
	// Distance the orbs are spaced out from the centre.
	const int _radiusFromCentre;
	// The current angle of the orbit.
	float _currentAngle;
	// Direction of the orbit.
	bool _isIncreasing;
	// Object visuals
	sf::CircleShape _movingObject1, _movingObject2;
};

