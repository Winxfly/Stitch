#pragma once
#include "GameState.h"
#include "Hero.h"
#include "MapLoader.h"

class MainGameState : public GameState {
private:
	GAME_STATE_INFO nextState = GAME_STATE_INFO::UNKNOWN;

	//sf::Clock clock;
	//sf::View view;

	//int fps = (int)(1 / dt);
	int floorY = 0;
	//float timestep = 1.0f / 60.0f;

	std::string localWorldColor = "gray";
	sf::Vector2i mouseXYFirst;
	sf::Vector2i mouseXYTemp;
	sf::Color backgroundColor;

	float ddx = 0;
	float ddy = 0;

	float xGround = 0.0f;
	float angleGround = 0.0f;

	float camX = 0;
	float camY = 0;

	/*int32 velocityIterations = 6;
	int32 positionIterations = 2;*/

	Map* map = nullptr;
	Hero* hero = nullptr;
public:
	//float dt;

	int rectShift[4];

	MainGameState(b2World* world) {
	    /*view = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
		view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);*/
		restartGame(world);
	}
	~MainGameState() {
		delete map;
		delete hero;
	}

	void restartGame(b2World* world) {
		if (map != nullptr) {
			delete map;
		}
		if (hero != nullptr) {
			delete hero;
		}

		map = new Map(world);
		hero = new Hero(world);
		backgroundColor = sf::Color(80, 80, 80);
	}

	virtual void handleEvent(sf::Event& e) {
		if (e.type == sf::Event::KeyPressed) {
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A))) && map->isColorChange == false) {
				hero->heroLeft();
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D))) && map->isColorChange == false) {
				hero->heroRight();
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W))) && map->isColorChange == false) {
				hero->heroUp();
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				nextState = GAME_STATE_INFO::SHOW_MENU;
			}
		}
		
		//hero->camWidth(view, camX, ddx);
		//hero->camHeight(floorY, view, camY, ddy);

		if (e.type == sf::Event::MouseButtonPressed) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				hero->dt = hero->clock.restart().asSeconds() / 1500.0f;
				map->isColorChange = true;

				if (e.type == sf::Event::MouseMoved) {
					sf::Vector2i mouseXY = sf::Mouse::getPosition();
					for (int i = 0; i < 4; i++) {
						rectShift[i] = 0;
					}
					
					if (mouseXYFirst.x > mouseXY.x && mouseXYFirst.y > mouseXY.y) {
						rectShift[0] = 10;
						localWorldColor = "blue";
						backgroundColor = sf::Color(0, 0, 205);
					}
					if (mouseXYFirst.x < mouseXY.x && mouseXYFirst.y > mouseXY.y) {
						rectShift[1] = 10;
						localWorldColor = "purple";
						backgroundColor = sf::Color(178, 34, 34);
					}
					if (mouseXYFirst.x > mouseXY.x && mouseXYFirst.y < mouseXY.y) {
						rectShift[2] = 10;
						localWorldColor = "pink";
						backgroundColor = sf::Color(50, 205, 50);
					}
					if (mouseXYFirst.x < mouseXY.x && mouseXYFirst.y < mouseXY.y) {
						rectShift[3] = 10;
						localWorldColor = "orange";
						backgroundColor = sf::Color(255, 255, 0);
					}

					if (mouseXYFirst.x != mouseXYTemp.x && mouseXYTemp.x != mouseXY.x) {
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
				hero->dt = hero->clock.restart().asSeconds();
				map->isColorChange = false;
				mouseXYFirst = sf::Mouse::getPosition();

				map->worldColor = localWorldColor;
			}
		}
	}

	GAME_STATE_INFO nextGameState() {
		return nextState;
	}

	virtual void draw(sf::RenderWindow* window) {
		hero->dt = hero->clock.restart().asSeconds();
		window->clear(backgroundColor);
		//window->setView(view);

		map->draw(window);
		if (map->isColorChange) {
			map->colorOne.setPosition(sf::Vector2f(hero->positions.x - COLOR_SETTINGS_SIZE - rectShift[0], WINDOW_HEIGHT - hero->positions.y - COLOR_SETTINGS_SIZE - rectShift[0]));
			map->colorTwo.setPosition(sf::Vector2f(hero->positions.x + rectShift[1], WINDOW_HEIGHT - hero->positions.y - COLOR_SETTINGS_SIZE - rectShift[1]));
			map->colorThree.setPosition(sf::Vector2f(hero->positions.x - COLOR_SETTINGS_SIZE - rectShift[2], WINDOW_HEIGHT - hero->positions.y + rectShift[2]));
			map->colorFour.setPosition(sf::Vector2f(hero->positions.x + rectShift[3], WINDOW_HEIGHT - hero->positions.y + rectShift[3]));
		}
		hero->draw(window);
	}
};