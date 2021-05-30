#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <time.h>

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include "tmx/MapLoader.hpp"
#include "tmx/Log.hpp"
#include "tmx/tmx2box2d.hpp"

#include "Constants.h"
#include "Hero.h"
#include "MapLoader.h"

sf::RenderWindow* window;
tmx::MapLoader* mapLoader;

int floorY = 0;


float ddx = 0;
float ddy = 0;

float xGround = 0.0f;
float angleGround = 0.0f;

float camX = 0;
float camY = 0;

int main() {
	srand(time(0));


	b2Vec2 gravity(0.0f, -9.81f);
	b2World world(gravity);

	Player hero(&world);

	int32 velocityIterations = 6;
	int32 positionIterations = 2;



	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

	mapLoader = new tmx::MapLoader("maps/");
	if (!mapLoader->load("desert.tmx")) {
		std::cout << "ERROR LOADING MAP\n";

		delete mapLoader;

		system("pause");
		return 1;
	}

	Map map(&world, mapLoader);


	sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST", sf::Style::Close, sf::ContextSettings(0, 0, 0));
	

	sf::Clock clock;
	while (window->isOpen()) {

		float dt = clock.restart().asSeconds();
		int fps = (int)(1 / dt);

		char buff[64];
		sprintf(buff, "FPS: %d", fps);
		window->setTitle(buff);



		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}

		b2Vec2 positions = hero.bodyM->GetPosition();

		b2Vec2 dddds = hero.bodyM->GetLinearVelocity();

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))) {
			hero.dx = -0.1;
			
			if (hero.onGround) {
				
					hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(-100, 0), true);
				
				//hero.bodyM->SetLinearVelocity(b2Vec2(-5.0f, 0));
			}
			else {
				if (dddds.x > -10) {
					hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(-10000, 0), true);
				}
			}
			
			
		}else
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) {
				hero.dx = 0.1;



				if (hero.onGround) {

					if (dddds.x < 10) {
						hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(50000, 0), true);
					}
					//hero.bodyM->SetLinearVelocity(b2Vec2(-5.0f, 0));
				}
				else {
					if (dddds.x < 10) {
						hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(10000, 0), true);
					}
				}

			}
			//sf::Vector2f position = hero.sprite.getPosition();

		if (positions.x - ddx < WINDOW_WIDTH / 2 && ddx >= 0 && dddds.x <= 0) {
			camX = (positions.x - (WINDOW_HEIGHT / 2) - ddx) * dt * 4;
			//camX = dddds.x * 27 * dt;
			ddx += camX;
			if (ddx > 0) {

				view.move(camX, 0);
				
			}
			else {
				ddx -= camX;
			}
			camX = 0;
		}else
		if (positions.x - ddx > WINDOW_WIDTH / 2) {
			camX = (positions.x - (WINDOW_HEIGHT / 2) - ddx) * dt * 4;
				//camX = dddds.x * 27 * dt;
				ddx += camX;
				if (ddx > 0) {

					view.move(camX, 0);

				}
				else {
					ddx -= camX;
				}
				camX = 0;
				
			}

	
			
				
			
		

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))) {
			//camY = -0.1 * dt;
			//	hero.bodyM->SetLinearVelocity(b2Vec2(0.0f, 15));
				//hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(0, 1500), true);
				//view.move(0, camY);
			if (hero.onGround) {
				hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(0, 90000), true);

			}


		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {
			
		}

		bool awaking = hero.bodyM->IsAwake();
		b2Vec2 ln = hero.bodyM->GetLinearVelocity();


		if (positions.y <= 0 && floorY == 0) {

			floorY = 1;
			view.move(0, MAP_SIZE_HEIGHT - WINDOW_HEIGHT);
		
		}
		else if (positions.y > 0 && floorY == 1) {

			floorY = 0;
			view.move(0, -(MAP_SIZE_HEIGHT - WINDOW_HEIGHT));
		
		}

		/*if (positions.y - ddy < WINDOW_HEIGHT / 4 && awaking == true) {

			
			camY = ln.y * 30 * dt;
			view.move(0, -camY);

			ddy += -camY;

			camY = 0;
		}
		else
			if (positions.y - ddy > WINDOW_HEIGHT / 4 && awaking == true && ddy > 0) {

				camY = ln.y * 30 * dt;
				view.move(0, -camY);

				ddy += -camY;

				camY = 0;
			}*/


		


		world.Step(1.0f / 60.0f, velocityIterations, positionIterations);


		//body->SetAngularVelocity(-10.0f);


		

		//b2Vec2 sdfsdf = hero.bodyM->GetLinearVelocity();

		//std::cout << sdfsdf.x << "||" << sdfsdf.y << "\n";

		map.updateMap(dt);

		hero.update(dt, positions, hero.bodyM, hero.dx, hero.dy);

		// sfml draw arc
		window->clear();
		window->setView(view);
		window->draw(*mapLoader);

		for (int i = 0; i < map.objCount; i++) {
			window->draw(map.rectangle4[i]);
			window->draw(map.convex[i]);
		}
		window->draw(hero.sprite);





		window->display();
	}

	delete window;
	delete mapLoader;

	return 0;
}