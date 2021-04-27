#include <stdio.h>

#include "box2d/box2d.h"

#include "SFML/Graphics.hpp"

#include "tmx/MapLoader.hpp"
#include "tmx/Log.hpp"
#include "tmx/tmx2box2d.hpp"

sf::RenderWindow *window;
tmx::MapLoader *mapLoader;

const float DEG  =  57.29577f;

float xGround = 0.0f;
float angleGround = 45.0f;

int main() {
	b2Vec2 gravity(0.0f, -9.81f);
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(xGround, 0.0f);


	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(1000.0f, 50.0f, b2Vec2(500.0f, 25.0f), angleGround);
	groundBody->CreateFixture(&groundBox, 1.0f);
	//groundBody->SetTransform(groundBody->GetPosition(), -angleGround);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(600.0f, 500.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	body->SetTransform(body->GetPosition(), 45.0f);

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
	rectangle.setRotation(45.0f);

	sf::RectangleShape rectangle2(sf::Vector2f(1000.0f, 100.0f));
	rectangle2.setFillColor(sf::Color(0, 0, 240));
	rectangle2.setOrigin(500.0f, 50.0f);
	rectangle2.setPosition(sf::Vector2f(xGround + 500.0f, 768.0f - 50.0f));
	rectangle2.setRotation(angleGround  * (-1));

	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

	mapLoader = new tmx::MapLoader("maps/");
	if (!mapLoader->load("desert.tmx")) {
		std::cout << "ERROR LOADING MAP\n";

		delete mapLoader;

		system("pause");
		return 1;
	}

	window = new sf::RenderWindow(sf::VideoMode(1024, 768), "TEST");
	while (window->isOpen()) {
		sf::Event e;

		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}

		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		//body->SetAngularVelocity(-10.0f);
		float angle = body->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle * DEG);
		rectangle.setPosition(sf::Vector2f(position.x + 35.0f, 768.0f - position.y - 50.0f));
		rectangle.setRotation(angle * DEG * (-1));

		window->clear();
		//window->draw(*mapLoader);
		window->draw(rectangle2);
		window->draw(rectangle);
		window->display();
	}
	
	delete window;
	delete mapLoader;
	
	return 0;
}