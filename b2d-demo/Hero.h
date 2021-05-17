#pragma once
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include "Constants.h"

class Player {

public:
	float dx = 0;
	float dy = 0;
	float offsetX = 0;
	float offsetY = 0;

	bool onGround;
	float currentFrame;

	sf::FloatRect rect;
	sf::Sprite sprite;
	sf::Texture heroTexture;

	b2BodyDef bodyDefM;
	b2Body* bodyM;
	

	Player(b2World* world) {
		//BOX2D
		
		
		bodyDefM.type = b2_dynamicBody;	
		bodyDefM.position.Set(150.0f, 600.0f);

		b2PolygonShape dynamicMario;
		dynamicMario.SetAsBox(32.0f / 2.54, 53.0f / 2.54);

		bodyM = world->CreateBody(&bodyDefM);
		bodyM->SetFixedRotation(true);
		
		b2FixtureDef fixtureDefM;
		
		fixtureDefM.shape = &dynamicMario;
		fixtureDefM.density = 10.0f;
		fixtureDefM.friction = 1.3f;
		bodyM->CreateFixture(&fixtureDefM);

		
		//SFML
		heroTexture.loadFromFile("mario2.png");
		sprite.setTexture(heroTexture);
		//sprite.setOrigin(32.0f, 53.0f);

		//rect = sf::FloatRect(0, 0, 32, 53);
		
		dx = 0;
		dy = 0;
		currentFrame = 0;

		sprite.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));
	}

	void update(float time, b2Vec2 position) {

		
		
		

		rect.left += dx * time;

		
		rect.top += dy * time;
		//onGround = false;

		

		currentFrame += 0.005*time;
		if (currentFrame > 3) {
			currentFrame -= 3;
		}

		if (dx < 0) {
			sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 53));
		}

		if (dx > 0) {
			sprite.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));
		}

		

		sprite.setPosition(position.x, WINDOW_HEIGHT - position.y);
		
		
		dx = 0;
	}



	//void Collision(int dir) {
	//	for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++) {
	//		for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++) {
	//			/*if ((TileMap[i][j] == 'B') || (TileMap[i][j] == 'V')) {
	//				if ((dx > 0) && dir == 0) {
	//					rect.left = j * 32 - rect.width;
	//				}
	//				if ((dx < 0) && dir == 0) {
	//					rect.left = j * 32 + 32;
	//				}
	//				if ((dy > 0) && dir == 1) {
	//					rect.top = i * 32 - rect.height;
	//					dy = 0;
	//					onGround = true;
	//				}
	//				if ((dy < 0) && dir == 1) {
	//					rect.top = i * 32 + 32;
	//					dy = 0;
	//				}
	//			}*/
	//			/*if ((dx > 0) && dir == 0) {
	//				rect.left = j * 32 - rect.width;
	//			}
	//			if ((dx < 0) && dir == 0) {
	//				rect.left = j * 32 + 32;
	//			}
	//			if ((dy > 0) && dir == 1) {
	//				rect.top = i * 32 - rect.height;
	//				dy = 0;
	//				onGround = true;
	//			}
	//			if ((dy < 0) && dir == 1) {
	//				rect.top = i * 32 + 32;
	//				dy = 0;
	//			}*/
	//			/*if (TileMap[i][j] == 'O') {
	//				TileMap[i][j] = ' ';
	//			}*/
	//		}
	//	}
	//}
};