#include <stdio.h>

#include "box2d/box2d.h"

#include "SFML/Graphics.hpp"

#include "const.h"
#include "tmx/MapLoader.hpp"
#include "tmx/Log.hpp"
#include "tmx/tmx2box2d.hpp"

sf::RenderWindow *window;
tmx::MapLoader *mapLoader;



int main() {

	///////////////////////////////////////////////////////
	//      гравитация + статический шар
	b2Vec2 gravity(0.0f, -1.0f);  //гравитация
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 250.0f);   // корды шара
	b2CircleShape groundBox;
	groundBox.m_p.Set(0.0f, 0.0f);

	groundBox.m_radius = 50.0f;

	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	groundBody->CreateFixture(&groundBox, 0.0f);



	///////////////////////////////////////////////////////
	//                    платформа

	b2BodyDef groundBod;
	groundBod.position.Set(0.0f, 0.0f);
	b2PolygonShape bed;
	bed.SetAsBox(4000.0f, 25.0f);

	b2Body* groundBo = world.CreateBody(&groundBod);
	groundBo->CreateFixture(&bed, 0.0f);
	  
	//////////////////////////////////////////////////////
	//                      шар

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(20.0f, 400.0f);
	
	b2Body* body = world.CreateBody(&bodyDef);

	b2CircleShape dynamicBox;
	dynamicBox.m_p.Set(0.0f, 0.0f);
	
	dynamicBox.m_radius = 25.0f;
	

	/////////////////////////////////////////////////////////
	//                  коробка


	b2BodyDef thebox;
	thebox.type = b2_dynamicBody;
	thebox.position.Set(600.0f, 200.0f);
	
	b2PolygonShape thedinamicbox;
	thedinamicbox.SetAsBox(100.0f, 100.0f);
	
	b2Body* bodybox = world.CreateBody(&thebox);
	bodybox->CreateFixture(&thedinamicbox, 0.0f);

	b2FixtureDef fixtureDe;
	fixtureDe.shape = &thedinamicbox;
	fixtureDe.density = 1.0f;
	fixtureDe.friction = 0.3f;

	bodybox->CreateFixture(&fixtureDe);



	/////////////////////////////////////////////////////////
	//                    коробка 2


	b2BodyDef thebox2;
	thebox2.type = b2_dynamicBody;
	thebox2.position.Set(550.0f, 800.0f);

	b2PolygonShape thedinamicbox2;
	thedinamicbox2.SetAsBox(50.0f, 50.0f);

	b2Body* bodybox2 = world.CreateBody(&thebox2);
	bodybox2->CreateFixture(&thedinamicbox2, 0.0f);

	b2FixtureDef fixtureDe2;
	fixtureDe2.shape = &thedinamicbox2;
	fixtureDe2.density = 100.0f;
	fixtureDe2.friction = 30.0f;

	bodybox2->CreateFixture(&fixtureDe2);
	/*b2Vec2 vertices[4];
	vertices.type = b2_dynamicBody;
	vertices[0].Set(600.0f, 300.0f);
	vertices[1].Set(650.0f, 300.0f);
	vertices[2].Set(600.0f, 400.0f);
	vertices[3].Set(650.0f, 400.0f);

	b2PolygonShape polygon;
	polygon.Set(vertices, 4);
	
	b2Body* groundB = world.CreateBody(polygon);
	groundB->CreateFixture(&bed, 0.0f);*/
	///////////////////////////////////////////////////////
	//         плотность, трение и кадры в секунду

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	
	/////////////////////////////////////////////////////
	//               создание фигур



	sf::Texture Water;
	Water.loadFromFile("water.jpg");
	sf::Texture Zombie;
	Zombie.loadFromFile("zombie.jpg");
	sf::Texture Cripa;
	Cripa.loadFromFile("cripa.png");


	sf::CircleShape rectangle;
	//rectangle.setFillColor(sf::Color(200, 180, 240));
	rectangle.setRadius(25.0f);
	rectangle.setOrigin(25.0f, 25.0f);
	rectangle.setTexture(&Water);

	sf::CircleShape rectangle2;
	rectangle2.setFillColor(sf::Color(0, 0, 240));
	rectangle2.setRadius(50.0f);
	rectangle2.setOrigin(50.0f, 50.0f);
	rectangle2.setPosition(sf::Vector2f(0.0f, WINDOW_HEIGHT - 250.0f - 50));

	sf::RectangleShape rectangle3(sf::Vector2f(1024.0f, 75.0f));
	//rectangle.setFillColor(sf::Color(200, 180, 240));
	rectangle3.setPosition(0.0f, WINDOW_HEIGHT - 75);
	rectangle3.setTexture(&Water);

	sf::RectangleShape rectangle4(sf::Vector2f(200.0f, 200.0f));
	rectangle4.setTexture(&Cripa);
	rectangle4.setPosition(600.0f, WINDOW_HEIGHT - 500 - 150);
	rectangle4.setOrigin(100.0f, 100.0f);
	//rectangle3.setTexture(&Water);

	sf::RectangleShape rectangle42(sf::Vector2f(100.0f, 100.0f));
	rectangle42.setTexture(&Zombie);
	rectangle42.setPosition(490.0f, WINDOW_HEIGHT - 800);
	rectangle42.setOrigin(50.0f, 50.0f);
	//rectangle3.setTexture(&Water);

	////////////////////////////////////////////////////
	//              загрузка карты    


	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

	mapLoader = new tmx::MapLoader("maps/");
	if (!mapLoader->load("desert.tmx")) {
		std::cout << "ERROR LOADING MAP\n";

		delete mapLoader;

		system("pause");
		return 1;
	}

	//////////////////////////////////////////////////////
	//                  Открытие окна

	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");
	while (window->isOpen()) {
		sf::Event e;

		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}

		////////////////////////////////////////////////////////
		//     получение позиции и угла подвижного шара

		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		b2Vec2 positions = bodybox->GetPosition();
		b2Vec2 positions2 = bodybox2->GetPosition();
		float angle = body->GetAngle();
		float angle2 = bodybox->GetAngle();
		float angle22 = bodybox2->GetAngle();
		angle = -angle;
		angle2 = -angle2;
		angle22 = -angle22;
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle22);
		rectangle.setPosition(sf::Vector2f(position.x, WINDOW_HEIGHT - position.y - 50));
		rectangle.setRotation(angle * DEG);
		rectangle4.setPosition(sf::Vector2f(positions.x, WINDOW_HEIGHT - positions.y - 50));
		rectangle4.setRotation(angle2 * DEG);
		rectangle42.setPosition(sf::Vector2f(positions2.x, WINDOW_HEIGHT - positions2.y - 50));
		rectangle42.setRotation(angle22 * DEG);
			
		/*if (Keyboard::isKeyPressed(Keyboard::Up)) {
		
		
		}*/


		/////////////////////////////////////////////////////////////
		//                    отрисовка


		window->clear();
		//window->draw(*mapLoader);
		window->draw(rectangle2);
		window->draw(rectangle);
		window->draw(rectangle3);
		window->draw(rectangle4);
		window->draw(rectangle42);
		window->display();
	}
	
	delete window;
	delete mapLoader;
	
	return 0;
}