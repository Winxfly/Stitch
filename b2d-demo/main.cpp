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
float timestep = 1.0f / 60.0f;

std::string localWorldColor = "gray";
bool isColorChange = false;
sf::Vector2i mouseXYFirst;
sf::Vector2i mouseXYTemp;
int O = 0;
int OO = 0;
int OOO = 0;
int OOOO = 0;


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

	sf::RectangleShape color;
	color.setPosition(sf::Vector2f(0, 0));
	color.setFillColor(sf::Color(0, 0, 0, 128));
	color.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

	sf::RectangleShape colorOne;
	colorOne.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 75));
	colorOne.setFillColor(sf::Color(0, 0, 205, 128));
	colorOne.setSize(sf::Vector2f(100, 150));

	sf::RectangleShape colorTwo;
	colorTwo.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT / 2 - 75));
	colorTwo.setFillColor(sf::Color(178, 34, 34, 128));
	colorTwo.setSize(sf::Vector2f(100, 150));

	sf::RectangleShape colorThree;
	colorThree.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 75));
	colorThree.setFillColor(sf::Color(50, 205, 50, 128));
	colorThree.setSize(sf::Vector2f(100, 150));

	sf::RectangleShape colorFour;
	colorFour.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT / 2 + 75));
	colorFour.setFillColor(sf::Color(255, 255, 0, 128));
	colorFour.setSize(sf::Vector2f(100, 150));




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

		

		
		////////////////////////////////////////////////////////////////
		//                управление(left and right)
		

		b2Vec2 positions = hero.bodyM->GetPosition();

		b2Vec2 dddds = hero.bodyM->GetLinearVelocity();
	//	std::cout << hero.onGround << " \n ";
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A))) && isColorChange == false) {
			hero.dx = -0.1;
			
			if (hero.onGround == 1) {	
				if (dddds.x > -20) {
					hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(-500 * 4, 0), true);
					//hero.bodyM->SetLinearVelocity(b2Vec2(-15.0f, 0));
				}
			}
			else {			
				if (dddds.x > -20) {
					hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(-100 * 4, 0), true);
				}
			}			
		}else
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D))) && isColorChange == false) {
				hero.dx = 0.1;

				if (hero.onGround) {	
					if (dddds.x < 20) {
						hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(500 * 4, 0), true);
					}
					//hero.bodyM->SetLinearVelocity(b2Vec2(15.0f, 0));
				}
				else {				
					if (dddds.x < 20) {
						hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(100 * 4, 0), true);
					}
				}
			}
			//sf::Vector2f position = hero.sprite.getPosition();

	
		///////////////////////////////////////////////////////////////////
		//                     управление (up and down)
			
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W))) && isColorChange == false) {
			
			if (hero.onGround) {
				hero.bodyM->ApplyLinearImpulseToCenter(b2Vec2(0, 55000), true);
			}
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {	

		}


		
		//////////////////////////////////////////////////////////////////////////
		//                             камера по X

		if (positions.x - ddx < WINDOW_WIDTH / 2 && ddx >= 0 && dddds.x <= 0) {
			camX = (positions.x - (WINDOW_WIDTH / 2) - ddx) * dt * 4;
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
		else
			if (positions.x - ddx > WINDOW_WIDTH / 2) {
				camX = (positions.x - (WINDOW_WIDTH / 2) - ddx) * dt * 4;
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
		
		//////////////////////////////////////////////////////////////
		//              переход по этажам (камера по Y)
		
		if (positions.y <= 0 && floorY == 0) {

			floorY = 1;
			//view.move(0, MAP_SIZE_HEIGHT - WINDOW_HEIGHT);
			
		}
		else if (positions.y > 0 && floorY == 1) {

			floorY = 0;
			//view.move(0, -(MAP_SIZE_HEIGHT - WINDOW_HEIGHT));
			
		}

		if (floorY == 1 && ddy <= HeightY) {
		
			camY = HeightY * dt * 3;
			ddy += camY;
			if (ddy <= HeightY) {
				
				view.move(0, camY);
			}
			else {
				ddy -= camY;
			}
			camY = 0;
		}
		else if (floorY == 0 && ddy >= 0) {
			camY = HeightY * dt * 3;
			ddy -= camY;
			if (ddy > 0) {
				view.move(0, -camY);
			}
			else {
				ddy += camY;
			}
			camY = 0;
		}
		//std::cout << ddy << "\n";

		////////////////////////////////////////////////////////////////////////
		//                 Механика цветов (по нажатию ЛКМ)

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			timestep = 1.0f / 1500.0f;  // замедление
			isColorChange = true;  // меню выбора открыто


			if (e.type == sf::Event::MouseMoved) {
				sf::Vector2i mouseXY = sf::Mouse::getPosition();
				//std::cout << mouseXYFirst.x << " | " << mouseXYFirst.y << "||" << mouseXY.x << " | " << mouseXY.y << "\n";

				O = 0;  // смещение первого прямоугольника
				OO = 0;  // смещение второго
				OOO = 0;  // думаю ты понял
				OOOO = 0;
				if (mouseXYFirst.x > mouseXY.x && mouseXYFirst.y > mouseXY.y) {
					O = 10;
					localWorldColor = "blue";  // будет выбран такой цвет
				}
				if (mouseXYFirst.x < mouseXY.x && mouseXYFirst.y > mouseXY.y) {
					OO = 10;
					localWorldColor = "red"; // будет выбран такой цвет
				}
				if (mouseXYFirst.x > mouseXY.x && mouseXYFirst.y < mouseXY.y) {
					OOO = 10;
					localWorldColor = "green"; // будет выбран такой цвет
				}
				if (mouseXYFirst.x < mouseXY.x && mouseXYFirst.y < mouseXY.y) {
					OOOO = 10;
					localWorldColor = "yellow"; // будет выбран такой цвет
				}



				if (mouseXYFirst.x != mouseXYTemp.x && mouseXYTemp.x != mouseXY.x) {  // чтобы курсор норм был
					mouseXYFirst.x = mouseXYTemp.x;
				}
				if (mouseXYFirst.y != mouseXYTemp.y && mouseXYTemp.y != mouseXY.y) {
					mouseXYFirst.y = mouseXYTemp.y;
				}

				mouseXYTemp.x = mouseXY.x;
				mouseXYTemp.y = mouseXY.y;
			}
		}
		else {
			timestep = 1.0f / 60.0f;
			isColorChange = false;
			mouseXYFirst = sf::Mouse::getPosition();

			map.worldColor = localWorldColor;  // применение цвета
		}



		/////////////////////////////////////////////////////////////
		//            Обновление всего чего только можно


		world.Step(timestep, velocityIterations, positionIterations);

		if (isColorChange) {
			// чуть выше середины экрана
		//	colorOne.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 100 - O, WINDOW_HEIGHT / 2 - 300 - O));
		//	colorTwo.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 + OO, WINDOW_HEIGHT / 2 - 300 - OO));
		//	colorThree.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 100 - OOO, WINDOW_HEIGHT / 2 - 150 + OOO));
		//	colorFour.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 + OOOO, WINDOW_HEIGHT / 2 - 150 + OOOO));


			// следует за персом
			colorOne.setPosition(sf::Vector2f(positions.x - 100 - O, WINDOW_HEIGHT - positions.y - 150 - O));
			colorTwo.setPosition(sf::Vector2f(positions.x + OO, WINDOW_HEIGHT - positions.y - 150 - OO));
			colorThree.setPosition(sf::Vector2f(positions.x - 100 - OOO, WINDOW_HEIGHT - positions.y + OOO));
			colorFour.setPosition(sf::Vector2f(positions.x + OOOO, WINDOW_HEIGHT - positions.y + OOOO));
		}
		map.updateMap(dt, isColorChange);

		hero.update(dt, positions, hero.bodyM);

		
		////////////////////////////////////////////////////////////////
		//                        вывод на экран


		window->clear();
		window->setView(view);
		window->draw(*mapLoader);

		for (int i = 0; i < map.objCount; i++) {
			
			if (map.worldColor != map.objectColor[i] || isColorChange == true) {
				
				window->draw(map.convex[i]);
			}
		}
		window->draw(hero.sprite);

		   //другой вариант

		if (isColorChange) {
			window->draw(color);
			window->draw(colorOne);
			window->draw(colorTwo);
			window->draw(colorThree);
			window->draw(colorFour);
			window->setView(window->getDefaultView());
			
		}

		window->display();
	}

	delete window;
	delete mapLoader;

	return 0;
}