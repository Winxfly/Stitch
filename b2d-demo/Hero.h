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

	
	void update(float dt, b2Vec2 position, b2Body* bodyM3) {

		int fgt = 0;

		rect.left += dx * dt;

		rect.top += dy * dt;
		//onGround = false;

		currentFrame += 0.005 * dt;
		if (currentFrame > 3) {
			currentFrame -= 3;
		}

		if (dx < 0) {
			sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 53));
		}

		if (dx > 0) {
			sprite.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));
		}

		for (b2ContactEdge* edge = bodyM3->GetContactList(); edge; edge = edge->next) {

			//b2Fixture* a = edge->contact->GetFixtureA();
			b2Fixture* b = edge->contact->GetFixtureB();   // получаем фиксуру второй фигуры(не гг )

		//	std::cout << edge << "\n";

			b2Body* ll2 = b->GetBody();  // тело фиксуры

			b2Vec2 hh2 = ll2->GetPosition();  //  позиция тела

			b2AABB ss = b->GetAABB(0);  // размер тела
			
			//std::cout << edge->contact->GetManifold()->pointCount << "\n";

			b2WorldManifold worldManifold;
			edge->contact->GetWorldManifold(&worldManifold);
			if (edge->contact->IsTouching() == 0) { onGround = false; }
			//std::cout << edge->contact->IsTouching() << "\n";
			for (int i = 0; i < edge->contact->GetManifold()->pointCount; i++) {
				
				b2Vec2 dd = worldManifold.points[i]; // точки соприкосновения 
				
			
				fgt++; //  сколько точек соприк.


				if (edge->contact->IsTouching() == 1 && dd.y < position.y && hh2.y + ss.upperBound.y - ss.lowerBound.y - 0.1f < dd.y && hh2.x < dd.x && hh2.x + ss.upperBound.x - ss.lowerBound.x - 0.1f > dd.x) {
					onGround = true;
					//std::cout << onGround << "1 | ";   очень сложное и неправильно условие, я засунул в условие проверку на соприкосновение с верхней частью фигуры, а нужно было с нижней частью перса, тебе надо исправить
					
				}
				else {
					
						onGround = false;
					
						
					
				//	std::cout << onGround << "2\n";
				}
				
			//	std::cout << "Count: " << edge->contact->GetManifold()->pointCount << "||" << dd.x << " | " << dd.y << "\n";

				//std::cout << position.x << " | " << position.y <<  "||||||";

				//std::cout << hh.x << " | " << hh.y << "|||||" << ss.lowerBound.x << " | " << ss.upperBound.y - ss.lowerBound.y << "|||||" << "\n";
			}




			//std::cout << sdf.x << " | " << sdf.y <<"\n";
		}

		if (fgt > 2) {  // если точек соприк больше двух то норм
			onGround = true;
		}

		//std::cout << fgt << "\n";
		fgt = 0;
		sprite.setPosition(position.x, WINDOW_HEIGHT - position.y);


		dx = 0;
	}

	
};