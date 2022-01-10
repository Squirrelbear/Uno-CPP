#include <SFML/Graphics.hpp>
#include "EllipseShape.h"
#include "DrawableShape.h"
#include "CardFrontObjectGroup.h"
#include <filesystem>
#include <iostream>
#include "ArcShape.h"
#include "CardBackGroupObject.h"
#include "Deck.h"

int main()
{
	//std::cout << "CWD: " << std::filesystem::current_path() << std::endl;
	sf::Font font;
	if (!font.loadFromFile("../Fonts/arial.ttf"))
	{
		throw("Failed to load font.");
	}

	sf::RenderWindow window(sf::VideoMode(1280, 720), "UNO - by Peter Mitchell (2022)");
	EllipseShape* shape = new EllipseShape(sf::Vector2f(100.0f, 50.0f));
	//sf::CircleShape* shape = new sf::CircleShape(100.0f);
	shape->setFillColor(sf::Color::Green);

	//DrawableShape test(shape, sf::Vector2f(0.0f,0.0f));
	CardFrontObjectGroup testGroup(Card(8,2,0),  sf::Vector2f(0,0), font);
	CardFrontObjectGroup testGroup2(Card(8, 3, 0), sf::Vector2f(100, 100), font);
	CardFrontObjectGroup testGroup3(Card(13, 3, 0), sf::Vector2f(165, 100), font);
	CardFrontObjectGroup testGroup4(Card(13, 4, 0), sf::Vector2f(230, 100), font);
	CardFrontObjectGroup testGroup5(Card(14, 4, 0), sf::Vector2f(295, 100), font);
	CardFrontObjectGroup testGroup6(Card(14, 2, 0), sf::Vector2f(360, 100), font);

	ArcShape* testArc = new ArcShape(0, 270, sf::Vector2f(30, 30));
	testArc->setPosition(sf::Vector2f(500, 500));
	testArc->setFillColor(sf::Color::Red);
	for (std::size_t i = 0; i < testArc->getPointCount(); i++) {
		sf::Vector2f point = testArc->getPoint(i);
		std::cout << point.x << " " << point.y << std::endl;
	}

	CardBackGroupObject cardBackTest(sf::Vector2f(700, 500), font);

	Deck deck = Deck(sf::Vector2f(100, 500), font);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//test.move(sf::Vector2f(0.1f, 0.1f));

		window.clear(sf::Color(93, 141, 74));
		//test.draw(window);
		testGroup.draw(window);
		testGroup2.draw(window);
		testGroup3.draw(window);
		testGroup4.draw(window);
		testGroup5.draw(window);
		testGroup6.draw(window);
		cardBackTest.setPositionWithOffset(sf::Vector2f(700, 500));
		cardBackTest.draw(window);
		cardBackTest.setPositionWithOffset(sf::Vector2f(800, 500));
		cardBackTest.draw(window);
		cardBackTest.setPositionWithOffset(sf::Vector2f(900, 500));
		cardBackTest.draw(window);
		window.draw(*testArc);
		deck.draw(window);
		//window.draw(shape);
		window.display();
	}
	
	return 0;
}