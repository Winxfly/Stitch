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

sf::RenderWindow *window;
tmx::MapLoader *mapLoader;


float dd = 0;

float xGround = 0.0f;
float angleGround = 0.0f;

float camX = 0;
float camY = 0;

int main() {
	srand(time(0));


	b2Vec2 gravity(0.0f, -0.81f);
	b2World world(gravity);

	Player hero(&world);


	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(500, 100.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(500.0f, 50.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(780.0f, 500.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	body->SetFixedRotation(true);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(35.0f, 50.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 3.3f;

	body->CreateFixture(&fixtureDef);

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	sf::RectangleShape rectangle(sf::Vector2f(70.f, 100.f));
	rectangle.setFillColor(sf::Color(200, 180, 240));
	rectangle.setOrigin(35.0f, 50.0f);


	sf::RectangleShape rectangle2(sf::Vector2f(1000.0f, 100.0f));
	rectangle2.setFillColor(sf::Color(0, 0, 240));
	//rectangle2.setOrigin(500.0f, 50.0f);
	rectangle2.setPosition(sf::Vector2f(0, WINDOW_HEIGHT - 100));

	sf::RectangleShape rectangle3(sf::Vector2f(32.0f, 53.0f));
	rectangle3.setFillColor(sf::Color(100, 0, 240));


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

	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");


	sf::Clock clock;
	while (window->isOpen()) {

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500;

		b2Vec2 positionHero = hero.bodyM->GetPosition();
		rectangle3.setPosition(sf::Vector2f(positionHero.x, WINDOW_HEIGHT - positionHero.y - 50));

		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}

		b2Vec2 positions = hero.bodyM->GetPosition();

		rectangle3.setPosition(sf::Vector2f(positions.x, WINDOW_HEIGHT - positions.y));

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))) {
			hero.dx = -0.1;

			hero.bodyM->SetLinearVelocity(b2Vec2(-5.0f, 0));
			if (positions.x - dd < WINDOW_WIDTH / 2 && dd >= 0) {
				camX = -0.1 * time;
			}

			view.move(camX, 0);
			dd += camX;
			camX = 0;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) {
			hero.dx = 0.1;
			//hero.bodyM->SetLinearVelocity(b2Vec2(5.0f, 0));
			//hero.bodyM->ApplyForceToCenter(b2Vec2(50.0f, 0), false);
			hero.bodyM->SetLinearVelocity(b2Vec2(5.0f, 0));
			sf::Vector2f position = hero.sprite.getPosition();

			if (positions.x - dd > WINDOW_WIDTH / 2) {
				camX = 0.1 * time;
			}



			view.move(camX, 0);
			dd += camX;
			camX = 0;

		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))) {
			camY = -0.1 * time;
			view.move(0, camY);
			/*if (hero.onGround) {
				hero.dy = -0.4;
				hero.onGround = false;
			}*/
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {
			//	hero.dy = 0.1;
			camY = 0.1 * time;
			view.move(0, camY);
		}


		world.Step(timeStep, velocityIterations, positionIterations);


		//body->SetAngularVelocity(-10.0f);
		float angle = body->GetAngle();
		b2Vec2 position = body->GetPosition();

		rectangle.setPosition(sf::Vector2f(position.x, WINDOW_HEIGHT - position.y));





		map.updateMap(time, map.bodyM2);

		hero.update(time, positions);

		// sfml draw arc
		window->clear();
		window->setView(view);
		window->draw(*mapLoader);

		for (int i = 0; i <= 2; i++) {
			window->draw(map.rectangle4[i]);
		}
		window->draw(hero.sprite);
		window->draw(rectangle2);
		//window->draw(rectangle3);
		window->draw(rectangle);



		window->display();
	}

	delete window;
	delete mapLoader;

	return 0;
}