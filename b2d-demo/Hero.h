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

	b2BodyDef bodyDef;
	b2Vec2 position;

	Player() {
		//BOX2D
		b2Vec2 gravity(0.0f, -9.81f);
		b2World world(gravity);

		bodyDef.type = b2_dynamicBody;
		b2Body* body = world.CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(32.0f / 2, 53.0f / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 100.3f;
		body->CreateFixture(&fixtureDef);

		position = body->GetPosition();

		//SFML
		heroTexture.loadFromFile("mario.png");
		sprite.setTexture(heroTexture);
		sprite.setOrigin(32.0f / 2, 53.0f / 2);

		rect = sf::FloatRect(200, 200, 32, 53);

		dx = 0;
		dy = 0;
		currentFrame = 0;

		sprite.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));
	}

	void update(float time) {

		rect.left += dx * time;

		//Collision(0);

		/*if (!onGround) {
			dy = dy + 0.0005 * time;
		}*/
		rect.top += dy * time;
		onGround = false;

		//Collision(1);

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

		bodyDef.position.Set(rect.left + 700.0f - offsetX, rect.top + 600.0f - offsetY);
		sprite.setPosition(rect.left - position.x + 32.0f / 2, WINDOW_HEIGHT / 2 - rect.top - position.y - 53.0f / 2);
		//sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
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