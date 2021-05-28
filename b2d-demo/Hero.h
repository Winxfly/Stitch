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
	//b2Vec2 position;

	b2Body* body;
	//b2Vec2 gravity;

	Player(b2World* world) {
		//BOX2D
		//gravity.Set(0.0f, -9.81f);
		//world.SetGravity(gravity);

		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(700.0f, 600.0f);
		body = world->CreateBody(&bodyDef);
		body->SetFixedRotation(true);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(66.0f / 2, 97.0f / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 5.3f;
		body->CreateFixture(&fixtureDef);

		//SFML
		heroTexture.loadFromFile("image/hero.png");
		sprite.setTexture(heroTexture);
		sprite.setOrigin(66.0f / 2, 97.0f / 2);

		rect = sf::FloatRect(200, 200, 66, 97);

		dx = 0;
		dy = 0;
		currentFrame = 0;

		sprite.setTextureRect(sf::IntRect(66 * int(currentFrame) + 66, 0, -66, 97));
	}

	void update(float time, b2Vec2 position) {

		rect.left += dx * time;

		//Collision(0);

		/*if (!onGround) {
			dy = dy + 0.0005 * time;
		}*/
		rect.top += dy * time;
		//onGround = false;

		//Collision(1);

		currentFrame += 0.009 * time;
		if (currentFrame > 7) {
			currentFrame -= 6;
		}

		if (dx > 0) {
			sprite.setTextureRect(sf::IntRect(66 * int(currentFrame), 0, 66, 97));
		}

		if (dx < 0) {
			sprite.setTextureRect(sf::IntRect(66 * int(currentFrame) + 66, 0, -66, 97));
		}

		//position = body->GetPosition();
		//position.Set(rect.left - offsetX, rect.top - offsetY - 700);
		sprite.setPosition(position.x, WINDOW_HEIGHT - position.y - 50.0f);
		//sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	}

	
};