#include "OvalHoverButton.h"

#include "EllipseShape.h"
#include "DrawableText.h"

OvalHoverButton::OvalHoverButton(const sf::IntRect & bounds, const sf::Font & font, const std::string & message, const sf::Color & bgColour)
	: WndInterface(bounds), _bgColour(bgColour)
{
	_isHovered = false;
	_isActive = true;

	DrawableText* _foreground = new DrawableText(message, font, 30, sf::Color(226, 173, 67), sf::Text::Bold);
	int strWidth = _foreground->getTextWidth();
	_foreground->setPositionWithOffset(sf::Vector2f(bounds.left + _bounds.width / 2 - strWidth / 2, bounds.top + _bounds.height / 2 - 20));
	DrawableText* _shadow = new DrawableText(message, font, 30, sf::Color::Black, sf::Text::Bold);
	_shadow->setPositionWithOffset(sf::Vector2f(bounds.left + _bounds.width / 2 - strWidth / 2 - 2, bounds.top + _bounds.height / 2 - 20 + 2));
	_text = new DrawableObjectGroup();
	_text->addChild(_shadow);
	_text->addChild(_foreground);

	_hoverShape = new DrawableShape(new EllipseShape(sf::Vector2f(_bounds.width / 2, _bounds.height / 2)), _bgColour);
	_hoverShape->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));
}

OvalHoverButton::~OvalHoverButton()
{
	delete _hoverShape;
	delete _text;
}

void OvalHoverButton::handleMouseMove(const sf::Vector2i & mousePosition)
{
	bool currentValue = _isHovered;
	_isHovered = _bounds.contains(mousePosition);
	// If the state changed
	if (currentValue != _isHovered) {
		_hoverShape->setBorder(_isHovered ? 10.0f : 0.0f, _bgColour);
	}
}

void OvalHoverButton::draw(sf::RenderWindow & renderWindow) const
{
	if (!_isActive) return;

	_hoverShape->draw(renderWindow);
	_text->draw(renderWindow);
}

bool OvalHoverButton::isTriggeredReset()
{
	bool result = _isTriggered;
	_isTriggered = false;
	return result;
}

int OvalHoverButton::getActionID() const
{
	return _actionID;
}
