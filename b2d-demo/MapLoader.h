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
	
	int objCount;

	sf::FloatRect recto[32];
	sf::Sprite sprites;
	sf::Texture heroTextures;
	float currentFrame = 0;
	b2BodyDef bodyDefM2;
	b2Body* bodyM2[32];
	sf::RectangleShape rectangle4[32];
	sf::ConvexShape convex[32];
	//std::vector<sf::RectangleShape> rectangles;

	

	tmx::MapObject* mapObj;

	

	Map(b2World* world, tmx::MapLoader* mapLoader) {
		//BOX2D



		const std::vector<tmx::MapLayer>& layers = mapLoader->getLayers();
		int score = 0;

		objCount = layers[2].objects.size() - 1;

		for (const auto& l : layers)
		{
			
			
			//if (l.name == "test") {
			for (const auto& o : l.objects)
			{

				std::string fc = o.getName();


				if (fc == "test") {


					mapObj = new tmx::MapObject(o);
					std::vector<sf::Vector2f> pointss = mapObj->polyPoints();

					recto[score] = sf::FloatRect(mapObj->getAABB());

					//std::cout << recto.left << " 1| " << recto.top << "\n";
					//std::cout << recto.width << " | " << recto.height << "\n";

					bodyDefM2.type = b2_dynamicBody;
					sf::Vector2f fff = mapObj->firstPoint();
					bodyDefM2.position.Set(recto[score].left, WINDOW_HEIGHT - recto[score].top - recto[score].height);

					b2PolygonShape dynamicMario2;
					//dynamicMario2.SetAsBox(recto[score].width / 2, recto[score].height / 2);

					bodyM2[score] = world->CreateBody(&bodyDefM2);
					bodyM2[score]->SetFixedRotation(true);

					convex[score].setPointCount(pointss.size());
					convex[score].setFillColor(sf::Color(200, 50, 90));
					convex[score].setPosition(sf::Vector2f(mapObj->firstPoint()));
					int d = pointss.size();
					

					b2Vec2 vertices[8];
					for (int i = 0; i < pointss.size(); i++) {

						vertices[i].Set(pointss[i].x + (fff.x - recto[score].left), recto[score].height - pointss[i].y);



						convex[score].setPoint(i, sf::Vector2f(pointss[i].x + (fff.x - recto[score].left), pointss[i].y));

					}



					dynamicMario2.Set(vertices, pointss.size());

					b2FixtureDef fixtureDefM2;

					fixtureDefM2.shape = &dynamicMario2;
					fixtureDefM2.density = 10.0f;
					fixtureDefM2.friction = 1.3f;
					bodyM2[score]->CreateFixture(&fixtureDefM2);

					b2Vec2 positionLocal = bodyM2[score]->GetPosition();

					//std::cout << positionLocal.x << " 2| " << positionLocal.y << "\n";

					//rectangles.emplace_back(sf::Vector2f(recto.width, recto.height));





					/*rectangle4[score].setSize(sf::Vector2f(recto[score].width, recto[score].height));
					rectangle4[score].setFillColor(sf::Color(100, 0, 240));
					rectangle4[score].setOrigin(recto[score].width / 2, recto[score].height / 2);
					rectangle4[score].setPosition(sf::Vector2f(positionLocal.x, WINDOW_HEIGHT - positionLocal.y - (recto[score].height / 2)));*/




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
					std::vector<sf::Vector2f> pointss = mapObj->polyPoints();
					recto[score] = sf::FloatRect(mapObj->getAABB());

					//std::cout << recto[score].left << " 1| " << recto[score].top << "\n";
					//std::cout << recto[score].width << " 2| " << recto[score].height << "\n";

					bodyDefM2.type = b2_staticBody;
					sf::Vector2f fff = mapObj->firstPoint();
					bodyDefM2.position.Set(recto[score].left, WINDOW_HEIGHT - recto[score].top - recto[score].height);

					b2PolygonShape dynamicMario2;
					dynamicMario2.SetAsBox(recto[score].width / 2, recto[score].height / 2);

					convex[score].setPointCount(pointss.size());
					convex[score].setFillColor(sf::Color(200, 50, 90));
					convex[score].setPosition(sf::Vector2f(recto[score].left, recto[score].top));

					b2Vec2 vertices[4];
					for (int i = 0; i < pointss.size(); i++) {

						vertices[i].Set(pointss[i].x + (fff.x - recto[score].left), recto[score].height - pointss[i].y);
						//std::cout << pointss[i].x << " | " << pointss[i].y << "\n";


						convex[score].setPoint(i, sf::Vector2f(pointss[i].x + (fff.x - recto[score].left), pointss[i].y));

					}



					dynamicMario2.Set(vertices, 4);

					bodyM2[score] = world->CreateBody(&bodyDefM2);
					bodyM2[score]->SetFixedRotation(true);

					b2FixtureDef fixtureDefM2;

					fixtureDefM2.shape = &dynamicMario2;
					fixtureDefM2.density = 10.0f;
					fixtureDefM2.friction = 1.3f;
					bodyM2[score]->CreateFixture(&fixtureDefM2);

					b2Vec2 positionLocal = bodyM2[score]->GetPosition();

					//std::cout << positionLocal.x << " 2| " << positionLocal.y << "\n";

					//rectangles.emplace_back(sf::Vector2f(recto.width, recto.height));




					//rectangle4[score].setSize(sf::Vector2f(recto[score].width, recto[score].height));
					//rectangle4[score].setFillColor(sf::Color(100, 0, 240));
					rectangle4[score].setPosition(sf::Vector2f(positionLocal.x, WINDOW_HEIGHT - positionLocal.y - (recto[score].height / 3)));
					rectangle4[score].setOrigin(recto[score].width / 2, recto[score].height / 2);


					heroTextures.loadFromFile("mario2.png");


					score++;


				}
			}
			//}
		}
		
	}

	void updateMap(float time, b2Body* bodyM2[3]) {





		currentFrame += 0.005 * time;
		if (currentFrame > 3) {
			currentFrame -= 3;
		}




		for (int i = 0; i < objCount; i++) {
			b2Vec2 positionBlock = bodyM2[i]->GetPosition();
			//sprites.setPosition(positionBlock.x, WINDOW_HEIGHT - positionBlock.y);
			//rectangle4[i].setPosition(sf::Vector2f(positionBlock.x, WINDOW_HEIGHT - positionBlock.y));
			convex[i].setPosition(sf::Vector2f(positionBlock.x, WINDOW_HEIGHT - positionBlock.y - recto[i].height));
			//std::cout << positionBlock.x << " | " << WINDOW_HEIGHT - positionBlock.y << "\n";
		}



	}




};
