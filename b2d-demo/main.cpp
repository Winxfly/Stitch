#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include "tmx/MapLoader.hpp"
#include "tmx/Log.hpp"
#include "tmx/tmx2box2d.hpp"

#include "Constants.h"
#include "GameState.h"
#include "Hero.h"
#include "MapLoader.h"
#include "MainGameState.h"
#include "MenuGameState.h"

sf::RenderWindow* window;

b2Vec2 gravity(0.0f, -9.81f);
b2World world(gravity);

GameState* gameState = new MenuGameState();
GAME_STATE_INFO selectedMode = GAME_STATE_INFO::MAIN;

void updateGameState(GAME_STATE_INFO nextState) {
	GameState* newState = nullptr;

	switch (nextState) {
	case GAME_STATE_INFO::MAIN:
		selectedMode = nextState;
		newState = new MainGameState(&world);
		break;

	/*case GAME_STATE_INFO::OPTIONS:
		newState = new SettingsGameState();
		break;*/

	case GAME_STATE_INFO::SHOW_MENU:
		newState = new MenuGameState();
		break;

	case GAME_STATE_INFO::EXIT:
		exit(1);
		break;

	default:
		break;
	}

	if (newState != nullptr) {
		delete gameState;
		gameState = newState;
	}
}


int main() {
	srand(time(0));

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	//sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	//view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stitch", sf::Style::Close, sf::ContextSettings(0, 0, 0));
	
	sf::Clock clock;
	while (window->isOpen()) {

		float dt = clock.restart().asSeconds();
		//int fps = (int)(1 / dt);

		/*char buff[64];
		sprintf(buff, "FPS: %d", fps);
		window->setTitle(buff);*/

		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
			gameState->handleEvent(e);
		}
		updateGameState(gameState->nextGameState());
		

		////////////////////////////////////////////////////////////////////////
		//                 Механика цветов (по нажатию ЛКМ)

		//if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		//	timestep = 1.0f / 1500.0f;  // замедление
		//	isColorChange = true;  // меню выбора открыто
		//	//sf::Mouse::setPosition(sf::Vector2i(positions.x, positions.y));
		//	window->setMouseCursorVisible(true);


		//	if (e.type == sf::Event::MouseMoved) {
		//		sf::Vector2i mouseXY = sf::Mouse::getPosition();
		//		//std::cout << mouseXYFirst.x << " | " << mouseXYFirst.y << "||" << mouseXY.x << " | " << mouseXY.y << "\n";

		//		O = 0;  // смещение первого прямоугольника
		//		OO = 0;  // смещение второго
		//		OOO = 0;  // думаю ты понял
		//		OOOO = 0;
		//		if (mouseXYFirst.x > mouseXY.x && mouseXYFirst.y > mouseXY.y) {
		//			O = 10;
		//			localWorldColor = "blue";  // будет выбран такой цвет
		//		}
		//		if (mouseXYFirst.x < mouseXY.x && mouseXYFirst.y > mouseXY.y) {
		//			OO = 10;
		//			localWorldColor = "purple"; // будет выбран такой цвет
		//		}
		//		if (mouseXYFirst.x > mouseXY.x && mouseXYFirst.y < mouseXY.y) {
		//			OOO = 10;
		//			localWorldColor = "pink"; // будет выбран такой цвет
		//		}
		//		if (mouseXYFirst.x < mouseXY.x && mouseXYFirst.y < mouseXY.y) {
		//			OOOO = 10;
		//			localWorldColor = "orange"; // будет выбран такой цвет
		//		}

		//		if (mouseXYFirst.x != mouseXYTemp.x && mouseXYTemp.x != mouseXY.x) {  // чтобы курсор норм был
		//			mouseXYFirst.x = mouseXYTemp.x;
		//		}
		//		if (mouseXYFirst.y != mouseXYTemp.y && mouseXYTemp.y != mouseXY.y) {
		//			mouseXYFirst.y = mouseXYTemp.y;
		//		}

		//		mouseXYTemp.x = mouseXY.x;
		//		mouseXYTemp.y = mouseXY.y;
		//	}
		//}
		//else {
		//	timestep = 1.0f / 60.0f;
		//	isColorChange = false;
		//	mouseXYFirst = sf::Mouse::getPosition();
		//	window->setMouseCursorVisible(false);

		//	map.worldColor = localWorldColor;  // применение цвета
		//}





		world.Step(dt * 9.81, velocityIterations, positionIterations);

		/*if (isColorChange) {
			colorOne.setPosition(sf::Vector2f(positions.x - COLOR_SETTINGS_SIZE - O, WINDOW_HEIGHT - positions.y - COLOR_SETTINGS_SIZE - O));
			colorTwo.setPosition(sf::Vector2f(positions.x + OO, WINDOW_HEIGHT - positions.y - COLOR_SETTINGS_SIZE - OO));
			colorThree.setPosition(sf::Vector2f(positions.x - COLOR_SETTINGS_SIZE - OOO, WINDOW_HEIGHT - positions.y + OOO));
			colorFour.setPosition(sf::Vector2f(positions.x + OOOO, WINDOW_HEIGHT - positions.y + OOOO));
		}
		map.updateMap(dt, isColorChange);
		hero.update(dt, positions);*/

		window->clear();
		gameState->draw(window);
		window->display();
	}
	delete gameState;
	delete window;

	return 0;
}