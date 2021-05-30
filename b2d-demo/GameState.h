#pragma once
#include "Constants.h"
#include "Enums.h"
#include <SFML/Graphics.hpp>

class GameState {
public:
	virtual void handleEvent(sf::Event& e) {}
	virtual GAME_STATE_INFO nextGameState() { return GAME_STATE_INFO::UNKNOWN; }
	virtual void draw(sf::RenderWindow* window) {}
};