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

	sf::FloatRect rect;
	bool onGround;
	sf::Sprite sprite;
	float currentFrame;
	sf::Texture heroTexture;

	Player() {
		heroTexture.loadFromFile("mario.png");
		sprite.setTexture(heroTexture);

		rect = sf::FloatRect(200, 200, 32, 53);

		dx = dy = 0;
		currentFrame = 0;
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

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
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