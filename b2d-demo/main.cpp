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
GAME_STATE_INFO selectedMode = GAME_STATE_INFO::SHOW_MENU;

void updateGameState(GAME_STATE_INFO nextState) {
	GameState* newState = nullptr;

	switch (nextState) {
	case GAME_STATE_INFO::MAIN:
		selectedMode = nextState;
		newState = new MainGameState(&world);
		break;

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

	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stitch", sf::Style::Close, sf::ContextSettings(0, 0, 0));	
	sf::Clock clock;

	while (window->isOpen()) {
		dt = clock.restart().asSeconds();
		
		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
			}
		}
		gameState->handleEvent(e);
		updateGameState(gameState->nextGameState());	

		world.Step(timeStep, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

		window->clear();
		gameState->draw(window);
		window->display();
	}
	delete gameState;
	delete window;

	return 0;
}