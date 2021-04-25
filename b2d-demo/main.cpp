#include <stdio.h>

#include "box2d/box2d.h"

#include "SFML/Graphics.hpp"

#include "tmx/MapLoader.hpp"
#include "tmx/Log.hpp"
#include "tmx/tmx2box2d.hpp"

sf::RenderWindow *window;
tmx::MapLoader *mapLoader;

float x = 0.0f;
float y = 0.0f;

int main() {

	///////////////////////////////////////////////////////
	//      ���������� + ���������
	b2Vec2 gravity(0.0f, -0.3f);  //����������
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(x, 250.0f);   // ����� ���������
	b2CircleShape groundBox;
	groundBox.m_p.Set(x, 0.0f);
	groundBox.m_radius = 50.0f;

	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	groundBody->CreateFixture(&groundBox, 0.0f);
	  
	//////////////////////////////////////////////////////
	//                      ���

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(50.0f, 600.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	b2CircleShape dynamicBox;
	dynamicBox.m_p.Set(0.0f, 0.0f);
	dynamicBox.m_radius = 50.0f;

	///////////////////////////////////////////////////////
	//         ���������, ������ � ����� � �������

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	
	/////////////////////////////////////////////////////
	//               �������� �����



	sf::Texture Water;
	Water.loadFromFile("water.jpg");

	sf::CircleShape rectangle;
	//rectangle.setFillColor(sf::Color(200, 180, 240));
	rectangle.setRadius(50.0f);
	rectangle.setTexture(&Water);

	sf::CircleShape rectangle2;
	rectangle2.setFillColor(sf::Color(0, 0, 240));
	rectangle2.setRadius(50.0f);
	rectangle2.setPosition(sf::Vector2f(x, 250.0f));

	////////////////////////////////////////////////////
	//              �������� �����    


	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

	mapLoader = new tmx::MapLoader("maps/");
	if (!mapLoader->load("desert.tmx")) {
		std::cout << "ERROR LOADING MAP\n";

		delete mapLoader;

		system("pause");
		return 1;
	}

	//////////////////////////////////////////////////////
	//                  �������� ����

	window = new sf::RenderWindow(sf::VideoMode(1024, 768), "TEST");
	while (window->isOpen()) {
		sf::Event e;

		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}

		////////////////////////////////////////////////////////
		//     ��������� ������� � ���� ���������� ����

		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float angle = body->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		rectangle.setPosition(sf::Vector2f(position.x, position.y));
		rectangle.setRotation(angle);

			
		/*if (Keyboard::isKeyPressed(Keyboard::Up)) {
		
		
		}*/


		/////////////////////////////////////////////////////////////
		//                    ���������


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