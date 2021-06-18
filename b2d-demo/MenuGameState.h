#pragma once
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Constants.h"

class MenuGameState : public GameState {
private:
	GAME_STATE_INFO nextState = GAME_STATE_INFO::UNKNOWN;

	sf::RectangleShape* shape;
	sf::RectangleShape* backgroundShape;
	const int SHAPE_WIDTH = 160;
	const int SHAPE_HEIGHT = 80;

	sf::Texture backgroundTexture;
	sf::Texture startTexture;
	//sf::Texture optionsTexture;
	sf::Texture exitTexture;

public:
	MenuGameState() {
		shape = new sf::RectangleShape();
		shape->setSize(sf::Vector2f(SHAPE_WIDTH, SHAPE_HEIGHT));

		backgroundTexture.loadFromFile("image/background.png");
		startTexture.loadFromFile("image/start.png");
		//optionsTexture.loadFromFile("image/options.png");
		exitTexture.loadFromFile("image/exit.png");

		backgroundShape = new sf::RectangleShape();
		backgroundShape->setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
		backgroundShape->setTexture(&backgroundTexture);
	}

	~MenuGameState() {
		delete shape;
		delete backgroundShape;
	}

	virtual void handleEvent(sf::Event& e) {
		if (e.type == sf::Event::MouseButtonReleased) {
			int mx = e.mouseButton.x;
			int my = e.mouseButton.y;

			if (mx >= SHAPE_WIDTH / 4 && mx <= SHAPE_WIDTH + SHAPE_WIDTH / 4) {
				if (my >= (WINDOW_HEIGHT / 2) - 2 * SHAPE_HEIGHT / 2 && my <= (WINDOW_HEIGHT / 2) - 2 * SHAPE_HEIGHT / 2 + SHAPE_HEIGHT) {
					nextState = GAME_STATE_INFO::MAIN;
				}
				//else if (my >= (WINDOW_HEIGHT / 2) - 3 * SHAPE_HEIGHT + SHAPE_HEIGHT / 2 && my <= (WINDOW_HEIGHT / 2) - 3 * SHAPE_HEIGHT / 2 + SHAPE_HEIGHT * 2) {
				//	//nextState = GAME_STATE_INFO::OPTIONS;
				//}
				else if (my >= (WINDOW_HEIGHT / 2) - 2 * SHAPE_HEIGHT + (SHAPE_HEIGHT * 2) / 2 && my <= (WINDOW_HEIGHT / 2) - 2 * SHAPE_HEIGHT / 2 + SHAPE_HEIGHT * 2) {
					nextState = GAME_STATE_INFO::EXIT;
				}
			}
		}
	}

	virtual GAME_STATE_INFO nextGameState() {
		return nextState;
	}

	virtual void draw(sf::RenderWindow* window) {
		window->draw(*backgroundShape);
		for (int i = 0; i < 2; i++) {
			shape->setPosition(sf::Vector2f(SHAPE_WIDTH / 4, i * SHAPE_HEIGHT + (WINDOW_HEIGHT / 2) - 2 * SHAPE_HEIGHT / 2));

			switch (i) {
			case 0:
				shape->setTexture(&startTexture);
				break;
			case 1:
				shape->setTexture(&exitTexture);
				break;			
			}
			window->draw(*shape);
		}
	}
};