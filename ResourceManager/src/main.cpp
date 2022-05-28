#include <iostream>
#include <SFML/Graphics.hpp>

#define DEBUG
#include <ExResource/ExResource.h>

std::shared_ptr<ExResource::ResourceStorageLoader> resourceLoader;

int main() {

	std::string filename = "res/tex.png";
	resourceLoader = std::make_shared<ExResource::ResourceStorageLoader>(filename);

	sf::RenderWindow window(sf::VideoMode(600, 400), "Test Window", sf::Style::Default);
	

	sf::Image img;
	img.loadFromFile(filename);

	sf::RectangleShape rect(sf::Vector2f(100, 100));
	sf::Texture tex;
	tex.loadFromImage(img);
	rect.setTexture(&tex);

	while (window.isOpen()) {
		sf::Event event;
		if (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		

		window.clear();

		window.draw(rect);

		window.display();
	}
	

	return 0;
}