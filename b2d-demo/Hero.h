#pragma once
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include "GameState.h"
#include "Constants.h"

class Hero : public GameState {
private:
	float currentFrame;
	bool isRight;

	sf::FloatRect *heroRect;
	sf::Texture *heroTexture;

	//Private methods
	void contact() {
		int countPointsContact = 0;
		for (b2ContactEdge* edge = bodyHero->GetContactList(); edge; edge = edge->next) {

			//b2Fixture* a = edge->contact->GetFixtureA();
			b2Fixture* fixtureB = edge->contact->GetFixtureB();   // получаем фиксуру второй фигуры(не гг )
			b2Body* bodyFixture = fixtureB->GetBody();
			b2Vec2 bodyPosition = bodyFixture->GetPosition();
			b2AABB bodySize = fixtureB->GetAABB(0);

			b2WorldManifold worldManifold;
			edge->contact->GetWorldManifold(&worldManifold);
			if (edge->contact->IsTouching() == 0) { onGround = false; }
			for (int i = 0; i < edge->contact->GetManifold()->pointCount; i++) {

				b2Vec2 pointsContact = worldManifold.points[i];
				countPointsContact++;

				if (edge->contact->IsTouching() == 1 && pointsContact.y < positions.y && bodyPosition.y + bodySize.upperBound.y - bodySize.lowerBound.y - 0.1f < pointsContact.y && bodyPosition.x < pointsContact.x && bodyPosition.x + bodySize.upperBound.x - bodySize.lowerBound.x - 0.1f > pointsContact.x) {
					onGround = true;
					//std::cout << onGround << "1 | ";   очень сложное и неправильно условие, я засунул в условие проверку на соприкосновение с верхней частью фигуры, а нужно было с нижней частью перса, тебе надо исправить

				}
				else {
					onGround = false;
				}
			}
		}

		if (countPointsContact > 2) {
			onGround = true;
		}
		countPointsContact = 0;
	}

public:
	float dx;
	float dy;
	bool onGround;

	sf::Clock clock;
	float dt = clock.restart().asSeconds();

	sf::Sprite* heroSprite;

	b2BodyDef bodyDefHero;
	b2Body* bodyHero;

	b2Vec2 positions;
	b2Vec2 linearVelocity;

	Hero(b2World* world) {
		//BOX2D
		bodyDefHero.type = b2_dynamicBody;
		bodyDefHero.position.Set(150.0f, 600.0f);

		b2PolygonShape dynamicHero;
		dynamicHero.SetAsBox(66.0f / 2, 97.0f / 2);

		bodyHero = world->CreateBody(&bodyDefHero);
		bodyHero->SetFixedRotation(true);

		b2FixtureDef fixtureDefHero;
		fixtureDefHero.shape = &dynamicHero;
		fixtureDefHero.density = 1.0f;
		fixtureDefHero.friction = 1.3f;
		bodyHero->CreateFixture(&fixtureDefHero);

		//SFML
		heroTexture = new sf::Texture();
		heroRect = new sf::FloatRect(200, 200, 66, 97);
		heroSprite = new sf::Sprite();
		
		heroTexture->loadFromFile("image/hero.png");
		heroSprite->setTexture(*heroTexture);
		heroSprite->setOrigin(66.0f / 2, 97.0f / 2);
		heroSprite->setTextureRect(sf::IntRect(66 * int(currentFrame), 0, 66, 97));

		dx = 0;
		dy = 0;
		currentFrame = 0;
		isRight = true;
		//update(positions);
	}
	~Hero() {
		delete heroRect;
		delete heroTexture;
		delete heroSprite;
	}

	//Public methods
	void update() {
		contact();
		positions = bodyHero->GetPosition();
		bodyHero->GetLinearVelocity();

		heroRect->left += dx * dt;
		heroRect->top += dy * dt;

		currentFrame += 133 * dt; // настроить
		if (currentFrame > 7) {
			currentFrame -= 6;
		}

		if (dx > 0) {
			heroSprite->setTextureRect(sf::IntRect(66 * int(currentFrame), 0, 66, 97));
			isRight = true;
		}
		else if (dx < 0) {
			heroSprite->setTextureRect(sf::IntRect(66 * int(currentFrame) + 66, 0, -66, 97));
			isRight = false;
		}
		else {
			if (isRight) {
				currentFrame = 0;
				heroSprite->setTextureRect(sf::IntRect(66 * int(currentFrame), 0, 66, 97));
			}
			else {
				currentFrame = 0;
				heroSprite->setTextureRect(sf::IntRect(66 * int(currentFrame) + 66, 0, -66, 97));
			}
		}

				
		heroSprite->setPosition(positions.x, WINDOW_HEIGHT - positions.y);
		dx = 0;
	}

	void heroLeft() {
		dx = -0.1;
		if (onGround == 1) {
			if (linearVelocity.x > -20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(-500 * 4, 0), true);
				//hero.bodyM->SetLinearVelocity(b2Vec2(-15.0f, 0));
			}
		}
		else {
			if (linearVelocity.x > -20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(-100 * 4, 0), true);
			}
		}
	}
	void heroRight() {
		dx = 0.1;
		if (onGround) {
			if (linearVelocity.x < 20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(500 * 4, 0), true);
			}
			//hero.bodyM->SetLinearVelocity(b2Vec2(15.0f, 0));
		}
		else {
			if (linearVelocity.x < 20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(100 * 4, 0), true);
			}
		}
	}
	void heroUp() {
		if (onGround) {
			bodyHero->ApplyLinearImpulseToCenter(b2Vec2(0, 55000), true);
		}
	}
	void heroDown() {

	}
	void camWidth(sf::View view, float camX, float ddx) {
		if (positions.x - ddx < WINDOW_WIDTH / 2 && ddx >= 0 && linearVelocity.x <= 0) {
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
	}
	void camHeight(int floorY, sf::View view, float camY, float ddy) {
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
	}

	void draw(sf::RenderWindow* window) {
		update();
		window->draw(*heroSprite);
	}
	
};