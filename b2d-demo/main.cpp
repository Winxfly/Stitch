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

sf::RenderWindow *window;
tmx::MapLoader *mapLoader;

float xGround = 0.0f;
float angleGround = 49.0f;

int main() {
	srand(time(0));
	Player hero;

	b2Vec2 gravity(0.0f, -9.81f);
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(500, 0.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(500.0f, 50.0f);
	groundBody->CreateFixture(&groundBox, 1.0f);

	groundBody->SetTransform(groundBody->GetPosition(), angleGround / DEG);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(800.0f, 500.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	body->SetTransform(body->GetPosition(), 45.0f / DEG);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(35.0f, 50.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 100.3f;

	body->CreateFixture(&fixtureDef);

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	sf::RectangleShape rectangle(sf::Vector2f(70.f, 100.f));
	rectangle.setFillColor(sf::Color(200, 180, 240));
	rectangle.setOrigin(35.0f, 50.0f);
	rectangle.setRotation(-45.0f);

	sf::RectangleShape rectangle2(sf::Vector2f(1000.0f, 100.0f));
	rectangle2.setFillColor(sf::Color(0, 0, 240));
	rectangle2.setOrigin(500.0f, 50.0f);
	rectangle2.setPosition(sf::Vector2f(xGround + 500, 768.0f - 50.0f));
	rectangle2.setRotation(angleGround  * (-1));

	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

	mapLoader = new tmx::MapLoader("maps/");
	if (!mapLoader->load("desert.tmx")) {
		std::cout << "ERROR LOADING MAP\n";

		delete mapLoader;

		system("pause");
		return 1;
	}

	sf::Texture mapLayerTexture;
	/*mapLayerTexture.loadFromFile("maps//desert.tmx");

	if (!mapLayerTexture.loadFromFile("maps/desert.tmx")) {
		std::cout << "ERROR LOADING MAP TEXTURE\n";

		system("pause");
		return 1;
	}*/

	sf::RectangleShape mapLayer(sf::Vector2f(MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT));
	mapLayer.setTexture(&mapLayerTexture);

	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");

	sf::Clock clock;
	while (window->isOpen()) {

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500;

		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))) {
			hero.dx = -0.1;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) {
			hero.dx = 0.1;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))) {
			if (hero.onGround) {
				hero.dy = -0.4;
				hero.onGround = false;
			}
		}

		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		//body->SetAngularVelocity(-10.0f);
		float angle = body->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle * DEG);
		rectangle.setPosition(sf::Vector2f(position.x, 768.0f - position.y - 50.0f));
		rectangle.setRotation(angle * DEG * (-1));

		hero.update(time);
		if (hero.rect.left > WINDOW_WIDTH / 2) {
			hero.offsetX = hero.rect.left - WINDOW_WIDTH / 2;
		}
		if (hero.rect.top < WINDOW_HEIGHT / 2) {
			hero.offsetY = hero.rect.top - WINDOW_HEIGHT / 2;
		}

		mapLayer.setPosition(sf::Vector2f(TILE_SIZE - hero.offsetX, TILE_SIZE - hero.offsetY));

		

		window->clear();
		window->draw(mapLayer);
		window->draw(rectangle2);
		window->draw(rectangle);
		window->draw(hero.sprite);
		window->display();
	}

	delete window;
	delete mapLoader;

	return 0;
}