#include <SFML/Graphics.hpp>
#include "EllipseShape.h"
#include "DrawableShape.h"
#include "CardFrontObjectGroup.h"
#include <filesystem>
#include <iostream>
#include "ArcShape.h"
#include "CardBackGroupObject.h"
#include "Deck.h"
#include "Button.h"
#include "PlayDirectionAnimation.h"
#include "PauseInterface.h"
#include "LobbyInterface.h"
#include "UnoButton.h"
#include "AntiUnoButton.h"

#include <chrono>       // std::chrono::system_clock
#include <iostream> // TODO remove

#include "SFMLGame.h"

int main()
{
	SFMLGame uno;
	uno.gameLoop();
	return 0;
}

void testing() {
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
	CardFrontObjectGroup testGroup(8,2,  sf::Vector2f(0,0), font);
	CardFrontObjectGroup testGroup2(8,3, sf::Vector2f(100, 100), font);
	CardFrontObjectGroup testGroup3(13,3, sf::Vector2f(165, 100), font);
	CardFrontObjectGroup testGroup4(13,4, sf::Vector2f(230, 100), font);
	CardFrontObjectGroup testGroup5(14,4, sf::Vector2f(295, 100), font);
	CardFrontObjectGroup testGroup6(14,2, sf::Vector2f(360, 100), font);

	PlayDirectionAnimation playDirectionAnimation = PlayDirectionAnimation(sf::Vector2f(1280 / 2.0f, 720 / 2.0f), 120, 5);

	ArcShape* testArc = new ArcShape(0, 270, sf::Vector2f(30, 30));
	testArc->setPosition(sf::Vector2f(500, 500));
	testArc->setFillColor(sf::Color::Red);
	for (std::size_t i = 0; i < testArc->getPointCount(); i++) {
		sf::Vector2f point = testArc->getPoint(i);
		std::cout << point.x << " " << point.y << std::endl;
	}

	CardBackGroupObject cardBackTest(sf::Vector2f(700, 500), font);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::default_random_engine randomEngine(seed);

	Deck deck = Deck(sf::Vector2f(100, 500), font, randomEngine);
	for (int i = 0; i < 10; i++) {
		Card* card = deck.drawCard();
		std::cout << "Drawn Card: " << card->getFaceValueID() << " " << card->getColourID() << " " << card->getUniqueCardID() << std::endl;
		delete card;
	}

	Button buttonTest(sf::IntRect(700,150,150,40), "Example Text", 0, font);

	//PauseInterface pauseInterface(sf::IntRect(1280/2-100, 720/2-100, 200, 200), sf::IntRect(0,0,1280,720), font);
	LobbyInterface lobbyInterface(sf::IntRect(0, 0, 1280, 720), font, randomEngine);
	//UnoButton testButton(sf::Vector2f(100, 100), font);
	//AntiUnoButton testButton2(sf::Vector2f(300, 100), font);


	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float deltaTime = elapsed.asSeconds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				//testButton.handleMousePress(sf::Mouse::getPosition(), true);
				lobbyInterface.handleMousePress(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), true);
			}
			if (event.type == sf::Event::MouseMoved) {
				//testButton.handleMouseMove(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				//testButton2.handleMouseMove(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				lobbyInterface.handleMouseMove(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			}
			if (event.type == sf::Event::KeyPressed) {
				
			}
		}
		//test.move(sf::Vector2f(0.1f, 0.1f));
		lobbyInterface.update(deltaTime);

		//playDirectionAnimation.update(deltaTime);
		//std::cout << deltaTime << std::endl;

		window.clear(sf::Color(93, 141, 74));
		/*//test.draw(window);
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
		buttonTest.draw(window);
		playDirectionAnimation.draw(window);
		//pauseInterface.draw(window);
		lobbyInterface.draw(window);*/
		//testButton.draw(window);
		//testButton2.draw(window);
		lobbyInterface.draw(window);
		window.display();
	}
}