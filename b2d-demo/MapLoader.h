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
	std::string worldColor = "Gray";
	std::string objectColor[32];

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
				

				if (fc == "dynamicRect") {

					

					mapObj = new tmx::MapObject(o);
					std::vector<sf::Vector2f> pointss = mapObj->polyPoints();

					recto[score] = sf::FloatRect(mapObj->getAABB());
					objectColor[score] = o.getType();
					

					bodyDefM2.type = b2_dynamicBody;
					sf::Vector2f fff = mapObj->firstPoint();
					bodyDefM2.position.Set(recto[score].left, WINDOW_HEIGHT - recto[score].top - recto[score].height);

					b2PolygonShape dynamicMario2;
					

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

					if (objectColor[score] == "blue") {
						convex[score].setFillColor(sf::Color(0, 0, 205));
					}
					else if (objectColor[score] == "red") {
						convex[score].setFillColor(sf::Color(178, 34, 34));
					}
					else if(objectColor[score] == "green") {
						convex[score].setFillColor(sf::Color(50, 205, 50));
					}
					else if(objectColor[score] == "yellow") {
						convex[score].setFillColor(sf::Color(255, 255, 0));
					}
					

					dynamicMario2.Set(vertices, pointss.size());

					b2FixtureDef fixtureDefM2;

					fixtureDefM2.shape = &dynamicMario2;
					fixtureDefM2.density = 0.3f;
					fixtureDefM2.friction = 1.3f;
					bodyM2[score]->CreateFixture(&fixtureDefM2);

					

					

					score++;
					
				}
				else if (fc == "staticRect") {


					mapObj = new tmx::MapObject(o);
					std::vector<sf::Vector2f> pointss = mapObj->polyPoints();
					recto[score] = sf::FloatRect(mapObj->getAABB());
					objectColor[score] = o.getType();
					
					bodyDefM2.type = b2_staticBody;
					sf::Vector2f fff = mapObj->firstPoint();
					bodyDefM2.position.Set(recto[score].left, WINDOW_HEIGHT - recto[score].top - recto[score].height);

					b2PolygonShape dynamicMario2;
					dynamicMario2.SetAsBox(recto[score].width / 2, recto[score].height / 2);

					convex[score].setPointCount(pointss.size());
					convex[score].setFillColor(sf::Color(200, 50, 90));
					convex[score].setPosition(sf::Vector2f(recto[score].left, recto[score].top));

					b2Vec2 vertices[8];
					for (int i = 0; i < pointss.size(); i++) {

						vertices[i].Set(pointss[i].x + (fff.x - recto[score].left), recto[score].height - pointss[i].y);
						


						convex[score].setPoint(i, sf::Vector2f(pointss[i].x + (fff.x - recto[score].left), pointss[i].y));
						
					}

					if (objectColor[score] == "blue") {
						convex[score].setFillColor(sf::Color(0, 0, 205));
					}
					else if (objectColor[score] == "red") {
						convex[score].setFillColor(sf::Color(178, 34, 34));
					}
					else if (objectColor[score] == "green") {
						convex[score].setFillColor(sf::Color(50, 205, 50));
					}
					else if (objectColor[score] == "yellow") {
						convex[score].setFillColor(sf::Color(255, 255, 0));
					}

					dynamicMario2.Set(vertices, pointss.size());

					bodyM2[score] = world->CreateBody(&bodyDefM2);
					bodyM2[score]->SetFixedRotation(true);

					b2FixtureDef fixtureDefM2;

					fixtureDefM2.shape = &dynamicMario2;
					fixtureDefM2.density = 10.0f;
					fixtureDefM2.friction = 1.3f;
					bodyM2[score]->CreateFixture(&fixtureDefM2);

					


					score++;


				}
			}
			
		}
		
	}

	void updateMap(float time, bool isColorChange) {




		for (int i = 0; i < objCount; i++) {
			b2Vec2 positionBlock = bodyM2[i]->GetPosition();
			sf::Color hhh = convex[i].getFillColor();

			if (worldColor == objectColor[i]) {
				bodyM2[i]->SetEnabled(false);

				
				if (isColorChange == true) {
					
					hhh.a = 128;
					convex[i].setFillColor(hhh);
				}
				
				
			}
			else {
				bodyM2[i]->SetEnabled(true);

				hhh.a = 255;
				convex[i].setFillColor(hhh);
			}

			//sprites.setPosition(positionBlock.x, WINDOW_HEIGHT - positionBlock.y);
			//rectangle4[i].setPosition(sf::Vector2f(positionBlock.x, WINDOW_HEIGHT - positionBlock.y));
			convex[i].setPosition(sf::Vector2f(positionBlock.x, WINDOW_HEIGHT - positionBlock.y - recto[i].height));
			//std::cout << positionBlock.x << " | " << WINDOW_HEIGHT - positionBlock.y << "\n";
		}



	}




};
