#pragma once
#include "WndInterface.h"
#include "DrawableObjectGroup.h"
#include "Button.h"

class PauseInterface :
	public WndInterface
{
public:
	PauseInterface(const sf::IntRect& bounds, const sf::IntRect& gameBounds, const sf::Font& font);
	virtual ~PauseInterface();

	// Do nothing during update.
	virtual void update(const float deltaTime) override {}
	virtual void draw(sf::RenderWindow & renderWindow) const override;

private:
	DrawableObjectGroup* _interfaceBackground;

	void initialiseInterface(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font);
};

