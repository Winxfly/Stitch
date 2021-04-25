#include <stdio.h>

#include "box2d/box2d.h"

#include "SFML/Graphics.hpp"

#include "tmx/MapLoader.hpp"
#include "tmx/Log.hpp"
#include "tmx/tmx2box2d.hpp"

sf::RenderWindow *wnd;
tmx::MapLoader *mapLoader;

int main() {
	/*b2Vec2 grav = b2Vec2(0, 9.8f);
	b2World world(grav);*/

	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

	mapLoader = new tmx::MapLoader("maps/");
	if (!mapLoader->load("desert.tmx")) {
		std::cout << "ERROR LOADING MAP\n";

		delete mapLoader;

		system("pause");
		return 1;
	}

	wnd = new sf::RenderWindow(sf::VideoMode(800, 600), "TEST");
	while (wnd->isOpen()) {
		sf::Event e;

		while (wnd->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				wnd->close();
			}
		}

		wnd->clear();
		wnd->draw(*mapLoader);
		wnd->display();
	}
	
	delete wnd;
	delete mapLoader;
	
	return 0;
}