#include <SFML/Graphics.hpp>
#include "EllipseShape.h"
#include "DrawableShape.h"
#include "CardFrontObjectGroup.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "UNO - by Peter Mitchell (2022)");
	EllipseShape* shape = new EllipseShape(sf::Vector2f(100.0f, 50.0f));
	//sf::CircleShape* shape = new sf::CircleShape(100.0f);
	shape->setFillColor(sf::Color::Green);

	//DrawableShape test(shape, sf::Vector2f(0.0f,0.0f));
	CardFrontObjectGroup testGroup(Card(8,2,0),  sf::Vector2f(0,0));
	CardFrontObjectGroup testGroup2(Card(8, 3, 0), sf::Vector2f(100, 100));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//test.move(sf::Vector2f(0.1f, 0.1f));

		window.clear();
		//test.draw(window);
		testGroup.draw(window);
		testGroup2.draw(window);
		//window.draw(shape);
		window.display();
	}

	return 0;
}