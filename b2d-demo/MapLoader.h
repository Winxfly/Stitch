#pragma once

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include "Constants.h"

class Map {

public:
	/*float dx = 0;
	float dy = 0;
	float offsetX = 0;
	float offsetY = 0;

	bool onGround;
	float currentFrame;

	sf::FloatRect rect;
	sf::Sprite sprite;
	sf::Texture heroTexture;

	b2BodyDef bodyDefM;
	b2Body* bodyM;*/
	sf::FloatRect recto;
	sf::Sprite sprites;
	sf::Texture heroTextures;
	float currentFrame = 0;
	b2BodyDef bodyDefM2;
	b2Body* bodyM2[5];
	sf::RectangleShape rectangle4[5];
	//std::vector<sf::RectangleShape> rectangles;

	
	tmx::MapObject* mapObj;


	Map(b2World* world, tmx::MapLoader* mapLoader) {
		//BOX2D





		std::vector<std::unique_ptr<sf::Shape>> debugBoxes;
		std::vector<DebugShape> debugShapes;
		std::map<b2Body*, sf::CircleShape> dynamicShapes; //we can use raw pointers because box2D manages its own memory

		//create a body creator
		tmx::BodyCreator bodyCreator;

		const std::vector<tmx::MapLayer>& layers = mapLoader->getLayers();
		int score = 0;

		for (const auto& l : layers)
		{

			//if (l.name == "test") {
			for (const auto& o : l.objects)
			{
				
				std::string fc = o.getName();
				

				if (fc == "test") {
				

					mapObj = new tmx::MapObject(o);
					recto = sf::FloatRect(mapObj->getAABB());

					//std::cout << recto.left << " | " << recto.top << "\n";
					//std::cout << recto.width << " | " << recto.height << "\n";

					bodyDefM2.type = b2_dynamicBody;
					bodyDefM2.position.Set(recto.left, WINDOW_HEIGHT - recto.top);

					b2PolygonShape dynamicMario2;
					dynamicMario2.SetAsBox(recto.width / 2.54, recto.height / 2.54);

					bodyM2[score] = world->CreateBody(&bodyDefM2);
					bodyM2[score]->SetFixedRotation(true);

					b2FixtureDef fixtureDefM2;

					fixtureDefM2.shape = &dynamicMario2;
					fixtureDefM2.density = 10.0f;
					fixtureDefM2.friction = 1.3f;
					bodyM2[score]->CreateFixture(&fixtureDefM2);

					b2Vec2 positionLocal = bodyM2[score]->GetPosition();

					//std::cout << positionLocal.x << " | " << positionLocal.y << "\n";

					//rectangles.emplace_back(sf::Vector2f(recto.width, recto.height));

					
						

					rectangle4[score].setSize(sf::Vector2f(recto.width, recto.height));
					rectangle4[score].setFillColor(sf::Color(100, 0, 240));
					rectangle4[score].setPosition(sf::Vector2f(positionLocal.x, WINDOW_HEIGHT - positionLocal.y));
					//rectangle4.setOrigin(recto.width / 2, recto.height / 2);
					

					heroTextures.loadFromFile("mario2.png");
					sprites.setTexture(heroTextures);
					

					sprites.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));

					score++;
					/*receive a pointer to the newly created body
					b2Body* b = bodyCreator.add(o, world, sf::Vector2u(32, 32));
					bodyCreator.add(o, world, sf::Vector2u(32, 32), ff);
					iterate over body info to create some visual debugging shapes to help visualise*/
					/*debugBoxes.push_back(std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(6.f, 6.f))));
					sf::Vector2f pos = tmx::boxToSfVec(b->GetPosition());
					debugBoxes.back()->setPosition(pos);
					debugBoxes.back()->setOrigin(3.f, 3.f);

					for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
					{
						b2Shape::Type shapeType = f->GetType();
						if (shapeType == b2Shape::e_polygon)
						{
							DebugShape ds;
							ds.setPosition(pos);
							b2PolygonShape* ps = (b2PolygonShape*)f->GetShape();

							int count = ps->m_count;
							for (int i = 0; i < count; i++) {
								b2Vec2 ddd = ps->m_vertices[i];
								sf::Vector2f pos = tmx::boxToSfVec(ps->m_vertices[i]);
								ds.addVertex(sf::Vertex(tmx::boxToSfVec(ps->m_vertices[i]), sf::Color::Green));
							}
							ds.addVertex(sf::Vertex(tmx::boxToSfVec(ps->m_vertices[0]), sf::Color::Green));
							debugShapes.push_back(ds);
						}
						else if (shapeType == b2Shape::e_circle)
						{
							b2CircleShape* cs = static_cast<b2CircleShape*>(f->GetShape());
							float radius = tmx::boxToSfFloat(cs->m_radius);
							std::unique_ptr<sf::CircleShape> c(new sf::CircleShape(radius));
							c->setPosition(pos);
							c->setOrigin(radius, radius);
							c->setOutlineColor(sf::Color::Green);
							c->setOutlineThickness(-1.f);
							c->setFillColor(sf::Color::Transparent);
							debugBoxes.push_back(std::move(c));
						}
					}*/
				}
				else if (fc == "test2") {
				

					mapObj = new tmx::MapObject(o);
					recto = sf::FloatRect(mapObj->getAABB());

					//std::cout << recto.left << " | " << recto.top << "\n";
					//std::cout << recto.width << " | " << recto.height << "\n";

					bodyDefM2.type = b2_staticBody;
					bodyDefM2.position.Set(recto.left, WINDOW_HEIGHT - recto.top);

					b2PolygonShape dynamicMario2;
					dynamicMario2.SetAsBox(recto.width / 2.54, recto.height / 2.54);

					bodyM2[score] = world->CreateBody(&bodyDefM2);
					bodyM2[score]->SetFixedRotation(true);

					b2FixtureDef fixtureDefM2;

					fixtureDefM2.shape = &dynamicMario2;
					fixtureDefM2.density = 10.0f;
					fixtureDefM2.friction = 1.3f;
					bodyM2[score]->CreateFixture(&fixtureDefM2);

					b2Vec2 positionLocal = bodyM2[score]->GetPosition();

					//std::cout << positionLocal.x << " | " << positionLocal.y << "\n";

					//rectangles.emplace_back(sf::Vector2f(recto.width, recto.height));




					rectangle4[score].setSize(sf::Vector2f(recto.width, recto.height));
					rectangle4[score].setFillColor(sf::Color(100, 0, 240));
					rectangle4[score].setPosition(sf::Vector2f(positionLocal.x, WINDOW_HEIGHT - positionLocal.y));
					//rectangle4.setOrigin(recto.width / 2, recto.height / 2);


					heroTextures.loadFromFile("mario2.png");
					sprites.setTexture(heroTextures);


					sprites.setTextureRect(sf::IntRect(32 * int(currentFrame) + 32, 0, -32, 53));

					score++;
				
				
				}
			}
			//}
		}
	}

	void updateMap(float time, b2Vec2 positionBlock) {





		currentFrame += 0.005 * time;
		if (currentFrame > 3) {
			currentFrame -= 3;
		}


		

		sprites.setPosition(positionBlock.x, WINDOW_HEIGHT - positionBlock.y);
		for (int i = 0; i <= 4; i++) {
			//rectangle4[i].setPosition(sf::Vector2f(positionBlock.x, WINDOW_HEIGHT - positionBlock.y));
		}
		//std::cout << positionBlock.x << " | " << WINDOW_HEIGHT - positionBlock.y << "\n";

		
	}



	
};
