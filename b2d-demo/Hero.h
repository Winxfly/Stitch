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
		bodyDefM.position.Set(150.0f + (32 / 2), 600.0f - (53.0f / 2.0f));

		b2PolygonShape dynamicMario;
		dynamicMario.SetAsBox(32.0f / 2, 53.0f / 2);

		bodyM = world->CreateBody(&bodyDefM);
		bodyM->SetFixedRotation(true);

		b2FixtureDef fixtureDefM;

		fixtureDefM.shape = &dynamicMario;
		fixtureDefM.density = 1.0f;
		fixtureDefM.friction = 1.3f;
		bodyM->CreateFixture(&fixtureDefM);



		//SFML
		heroTexture.loadFromFile("mario2.png");
		sprite.setTexture(heroTexture);
		sprite.setOrigin(32.0f / 2, 53.0f / 2);

		//rect = sf::FloatRect(0, 0, 32, 53);

		dx = 0;
		dy = 0;
		currentFrame = 0;

		sprite.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));
	}

	void update(float dt, b2Vec2 position, b2Body* bodyM3, float dxd, float dy) {

		rect.left += dxd * dt;

		rect.top += dy * dt;
		//onGround = false;

		currentFrame += 0.005 * dt;
		if (currentFrame > 3) {
			currentFrame -= 3;
		}

		if (dxd < 0) {
			sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 53));
		}

		if (dxd > 0) {
			sprite.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));
		}

		for (b2ContactEdge* edge = bodyM3->GetContactList(); edge; edge = edge->next) {

			b2Fixture* a = edge->contact->GetFixtureA();
			b2Fixture* b = edge->contact->GetFixtureB();



			b2Body* ll2 = b->GetBody();

			b2Vec2 hh2 = ll2->GetPosition();

			b2AABB ss = b->GetAABB(0);



			b2WorldManifold worldManifold;
			edge->contact->GetWorldManifold(&worldManifold);
			if (edge->contact->IsTouching() == 0) { onGround = false; }
			//std::cout << edge->contact->IsTouching() << "\n";
			for (int i = 0; i < edge->contact->GetManifold()->pointCount; i++) {

				b2Vec2 dd = worldManifold.points[i];

				edge->contact->IsTouching();
				if (edge->contact->IsTouching() == 1 && dd.y < position.y && hh2.y + ss.upperBound.y - ss.lowerBound.y - 1 < dd.y && hh2.x < dd.x && hh2.x + ss.upperBound.x - ss.lowerBound.x - 1 > dd.x) {
					onGround = true;
				}
				else {
					onGround = false;
				}

				//std::cout << "Count: " << edge->contact->GetManifold()->pointCount << "||" << dd.x << " | " << dd.y << " ||| ";

				//std::cout << position.x << " | " << position.y <<  "||||||";

				//std::cout << hh.x << " | " << hh.y << "|||||" << ss.lowerBound.x << " | " << ss.upperBound.y - ss.lowerBound.y << "|||||" << "\n";
			}




			//std::cout << sdf.x << " | " << sdf.y <<"\n";
		}

		sprite.setPosition(position.x, WINDOW_HEIGHT - position.y);


		dx = 0;
	}

	/*boolean isPlayerGrounded(float deltadt, b2World* world) {



			List<Contact> contactList = world->getWorld()->getContactList();

			for (int i = 0; i < contactList.size(); i++) {

					Contact contact = contactList.get(i);

					if (contact.isTouching() && (contact.getFixtureA() == world.getPlayer().playerSensorFixture ||

						contact.getFixtureB() == world.getPlayer().playerSensorFixture)) {



							Vector2 pos = world.getPlayer().getPosition();

							WorldManifold manifold = contact.getWorldManifold();

							boolean below = true;

							for (int j = 0; j < manifold.getNumberOfContactPoints(); j++) {

									below &= (manifold.getPoints()[j].y < pos.y - 0.4f);

							}



							if (below) {

									if (contact.getFixtureA().getUserData() != null && contact.getFixtureA().getUserData().equals("p")) {

											world.groundedPlatform = (MovingPlatform)contact.getFixtureA().getBody().getUserData();

											if (!keys.get(Keys.LEFT) && !keys.get(Keys.RIGHT))

												contact.setFriction(200F);

											else

												contact.setFriction(0F);

									}



									if (contact.getFixtureB().getUserData() != null && contact.getFixtureB().getUserData().equals("p")) {

											world.groundedPlatform = (MovingPlatform)contact.getFixtureB().getBody().getUserData();

											if (!keys.get(Keys.LEFT) && !keys.get(Keys.RIGHT))

												contact.setFriction(200F);

											else

												contact.setFriction(0F);

									}

									return true;

							}



							return false;

					}

			}

			return false;

	}*/

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