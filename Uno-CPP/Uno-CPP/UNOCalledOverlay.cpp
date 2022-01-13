#include "UNOCalledOverlay.h"
#include "Card.h"

UNOCalledOverlay::UNOCalledOverlay(sf::Vector2f position, const sf::Font & font)
	: PlayerFlashOverlay(position, "UNO!", sf::Color::Black, 40, font)
{
	std::string message = "UNO!";
	for (int i = 0; i < message.length(); i++) {
		_UNOLetters.emplace_back(sf::Text(message.at(i), font, 40));
		_UNOLetters.at(i).setFillColor(Card::getColourByID(i % 4));
		_UNOLetters.at(i).setPosition(position.x + 2 + i * 30, position.y + 2);
	}
}

UNOCalledOverlay::~UNOCalledOverlay()
{
}

void UNOCalledOverlay::draw(sf::RenderWindow & renderWindow) const
{
	if (fmod(_displayTimer, 0.2f) < 0.15f) {
		renderWindow.draw(_shadowText);
		for (const auto& letter : _UNOLetters) {
			renderWindow.draw(letter);
		}
	}
}
