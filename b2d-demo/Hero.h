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

			b2Fixture* fixtureB = edge->contact->GetFixtureB();   // �������� ������� ������ ������(�� �� )
			b2Body* bodyFixture = fixtureB->GetBody();
			b2Vec2 bodyPosition = bodyFixture->GetPosition();
			b2AABB bodySize = fixtureB->GetAABB(0);

			b2WorldManifold worldManifold;
			edge->contact->GetWorldManifold(&worldManifold);
			if (edge->contact->IsTouching() == 0) {
				onGround = false;
			}

			for (int i = 0; i < edge->contact->GetManifold()->pointCount; i++) {
				b2Vec2 pointsContact = worldManifold.points[i];
				countPointsContact++;

				if (edge->contact->IsTouching() == 1 && pointsContact.y < positions.y && bodyPosition.y + bodySize.upperBound.y - bodySize.lowerBound.y - 0.1f < pointsContact.y &&
					bodyPosition.x < pointsContact.x && bodyPosition.x + bodySize.upperBound.x - bodySize.lowerBound.x - 0.1f > pointsContact.x) {
					onGround = true;
					//std::cout << onGround << "1 | ";   ����� ������� � ����������� �������, � ������� � ������� �������� �� ��������������� � ������� ������ ������, � ����� ���� � ������ ������ �����, ���� ���� ���������
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

		dx = 0.0f;
		dy = 0.0f;
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
		linearVelocity = bodyHero->GetLinearVelocity();

		heroRect->left += dx * dt;
		heroRect->top += dy * dt;

		currentFrame += 15.0f * dt; // ���������
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
		dx = 0.0f;
	}

	void heroLeft() {
		dx = -0.1f;
		if (onGround) {
			if (linearVelocity.x > -20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(-5000 * 4, 0), true);
			}
		}
		else {
			if (linearVelocity.x > -20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(-1000 * 4, 0), true);
			}
		}
	}
	void heroRight() {
		dx = 0.1f;
		if (onGround) {
			if (linearVelocity.x < 20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(5000 * 4, 0), true);
			}
		}
		else {
			if (linearVelocity.x < 20) {
				bodyHero->ApplyLinearImpulseToCenter(b2Vec2(1000 * 4, 0), true);
			}
		}
	}
	void heroUp() {
		if (onGround) {
			bodyHero->ApplyLinearImpulseToCenter(b2Vec2(0, 200000), true);
		}
	}
	void heroDown() {

	}
	void camWidth(sf::View &view, float &camX, float &ddx, sf::RectangleShape &shadow) {
		if (positions.x - ddx < WINDOW_WIDTH / 2 && ddx >= 0 && linearVelocity.x <= 0) {
			camX = (positions.x - (WINDOW_WIDTH / 2) - ddx) * dt * 4;
			ddx += camX;

			if (ddx > 0) {

				view.move(camX, 0);
				shadow.move(camX, 0);

			}
			else {
				ddx -= camX;
			}
			camX = 0;
		}
		if (positions.x - ddx > WINDOW_WIDTH / 2) {
			camX = (positions.x - (WINDOW_WIDTH / 2) - ddx) * dt * 4;
			ddx += camX;

			if (ddx > 0) {

				view.move(camX, 0);
				shadow.move(camX, 0);

			}
			else {
				ddx -= camX;
			}
			camX = 0;
		}
	}
	void camHeight(sf::View &view, float &camY, float &ddy, sf::RectangleShape &shadow) {
		if (positions.y - ddy < WINDOW_HEIGHT / 2) {
			camY = (positions.y - (WINDOW_HEIGHT / 2) - ddy) * dt * 4;
			ddy += camY;

			if (ddy < 0) {
				shadow.move(0, -camY);
				view.move(0, -camY);
			}
			else {
				ddy -= camY;
			}
			camY = 0;
		}
		else if(positions.y - ddy > WINDOW_HEIGHT / 2) {
			camY = (positions.y - (WINDOW_HEIGHT / 2) - ddy) * dt * 4;
			ddy += camY;

			if (ddy < 0) {
				shadow.move(0, -camY);
				view.move(0, -camY);
			}
			else {
				ddy -= camY;
			}
			camY = 0;
		}			
	}

	void draw(sf::RenderWindow* window) {
		update();
		window->draw(*heroSprite);
	}
};