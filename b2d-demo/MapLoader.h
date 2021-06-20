#pragma once

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

#include "Constants.h"
#include "GameState.h"

class Map : public GameState {
private:
	int objCount;
	bool isColorChange;

	tmx::MapLoader* mapLoader;
	tmx::MapObject* mapObj;

	b2BodyDef bodyDef;
	b2Body* body[32];

	std::string objectColor[32];

	sf::Texture blueTexture;
	sf::Texture purpleTexture;
	sf::Texture orangeTexture;
	sf::Texture pinkTexture;

	sf::Texture rectBlueTexture;
	sf::Texture rectPurpleTexture;
	sf::Texture rectOrangeTexture;
	sf::Texture rectPinkTexture;

public:
	std::string worldColor = "Gray";

	sf::FloatRect rect[32];
	sf::ConvexShape convex[32];

	sf::RectangleShape shadow;
	sf::RectangleShape colorOne;
	sf::RectangleShape colorTwo;
	sf::RectangleShape colorThree;
	sf::RectangleShape colorFour;
	
	Map(b2World* world) {
		//reset();

		isColorChange = false;

		tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);
		mapLoader = new tmx::MapLoader("maps/");
		mapLoader->load("desert.tmx");
		/*if (!mapLoader->load("desert.tmx")) {
			std::cout << "ERROR LOADING MAP\n";

			delete mapLoader;

			system("pause");
			return 1;
		}*/

		blueTexture.loadFromFile("image/blue.png");
		purpleTexture.loadFromFile("image/purple.png");
		orangeTexture.loadFromFile("image/orange.png");
		pinkTexture.loadFromFile("image/pink.png");

		rectBlueTexture.loadFromFile("image/blueRect.png");
		rectPurpleTexture.loadFromFile("image/purpleRect.png");
		rectOrangeTexture.loadFromFile("image/orangeRect.png");
		rectPinkTexture.loadFromFile("image/pinkRect.png");

		shadow.setFillColor(sf::Color(0, 0, 0, 128));
		shadow.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

		colorOne.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - COLOR_SETTINGS_SIZE, WINDOW_HEIGHT / 2 - COLOR_SETTINGS_SIZE));
		colorOne.setSize(sf::Vector2f(COLOR_SETTINGS_SIZE, COLOR_SETTINGS_SIZE));
		colorOne.setTexture(&blueTexture);

		colorTwo.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 + COLOR_SETTINGS_SIZE, WINDOW_HEIGHT / 2 - COLOR_SETTINGS_SIZE));
		colorTwo.setSize(sf::Vector2f(COLOR_SETTINGS_SIZE, COLOR_SETTINGS_SIZE));
		colorTwo.setTexture(&purpleTexture);

		colorThree.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - COLOR_SETTINGS_SIZE, WINDOW_HEIGHT / 2 + COLOR_SETTINGS_SIZE));
		colorThree.setSize(sf::Vector2f(COLOR_SETTINGS_SIZE, COLOR_SETTINGS_SIZE));
		colorThree.setTexture(&pinkTexture);

		colorFour.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 + COLOR_SETTINGS_SIZE, WINDOW_HEIGHT / 2 + COLOR_SETTINGS_SIZE));
		colorFour.setSize(sf::Vector2f(COLOR_SETTINGS_SIZE, COLOR_SETTINGS_SIZE));
		colorFour.setTexture(&orangeTexture);

		const std::vector<tmx::MapLayer>& layers = mapLoader->getLayers();
		int score = 0;
		objCount = layers[2].objects.size() - 1;

		for (const auto& l : layers)
		{					
			for (const auto& o : l.objects)
			{
				std::string fc = o.getName();				
				if (fc == "dynamicRect") {
					mapObj = new tmx::MapObject(o);
					std::vector<sf::Vector2f> pointss = mapObj->polyPoints();

					rect[score] = sf::FloatRect(mapObj->getAABB());
					objectColor[score] = o.getType();
					
					bodyDef.type = b2_dynamicBody;
					sf::Vector2f fff = mapObj->firstPoint();
					bodyDef.position.Set(rect[score].left, WINDOW_HEIGHT - rect[score].top - rect[score].height);

					b2PolygonShape polygonShape;
					
					body[score] = world->CreateBody(&bodyDef);
					body[score]->SetFixedRotation(true);

					convex[score].setPointCount(pointss.size());
					convex[score].setPosition(sf::Vector2f(mapObj->firstPoint()));
					int d = pointss.size();		

					b2Vec2 vertices[8];
					for (int i = 0; i < pointss.size(); i++) {
						vertices[i].Set(pointss[i].x + (fff.x - rect[score].left), rect[score].height - pointss[i].y);
						convex[score].setPoint(i, sf::Vector2f(pointss[i].x + (fff.x - rect[score].left), pointss[i].y));
					}

					if (objectColor[score] == "blue") {
						convex[score].setTexture(&rectBlueTexture);
					}
					else if (objectColor[score] == "purple") {
						convex[score].setTexture(&rectPurpleTexture);
					}
					else if(objectColor[score] == "pink") {
						convex[score].setTexture(&rectPinkTexture);
					}
					else if(objectColor[score] == "orange") {
						convex[score].setTexture(&rectOrangeTexture);
					}
					
					polygonShape.Set(vertices, pointss.size());

					b2FixtureDef fixtureDef;
					fixtureDef.shape = &polygonShape;
					fixtureDef.density = 0.3f;
					fixtureDef.friction = 1.3f;
					body[score]->CreateFixture(&fixtureDef);

					score++;				
				}
				else if (fc == "staticRect") {
					mapObj = new tmx::MapObject(o);
					std::vector<sf::Vector2f> pointss = mapObj->polyPoints();
					rect[score] = sf::FloatRect(mapObj->getAABB());
					objectColor[score] = o.getType();
					
					bodyDef.type = b2_staticBody;
					sf::Vector2f fff = mapObj->firstPoint();
					bodyDef.position.Set(rect[score].left, WINDOW_HEIGHT - rect[score].top - rect[score].height);

					b2PolygonShape polygonShape;
					polygonShape.SetAsBox(rect[score].width / 2, rect[score].height / 2);

					convex[score].setPointCount(pointss.size());
					convex[score].setFillColor(sf::Color(0, 0, 0, 0));
					convex[score].setPosition(sf::Vector2f(rect[score].left, rect[score].top));

					b2Vec2 vertices[8];
					for (int i = 0; i < pointss.size(); i++) {
						vertices[i].Set(pointss[i].x + (fff.x - rect[score].left), rect[score].height - pointss[i].y);
						//convex[score].setPoint(i, sf::Vector2f(pointss[i].x + (fff.x - rect[score].left), pointss[i].y));
					}

					if (objectColor[score] == "blue") {
						convex[score].setFillColor(sf::Color(0, 0, 205));
					}
					else if (objectColor[score] == "purple") {
						convex[score].setFillColor(sf::Color(178, 34, 34));
					}
					else if (objectColor[score] == "pink") {
						convex[score].setFillColor(sf::Color(50, 205, 50));
					}
					else if (objectColor[score] == "orange") {
						convex[score].setFillColor(sf::Color(255, 255, 0));
					}

					polygonShape.Set(vertices, pointss.size());

					body[score] = world->CreateBody(&bodyDef);
					body[score]->SetFixedRotation(true);

					b2FixtureDef fixtureDef;
					fixtureDef.shape = &polygonShape;
					fixtureDef.density = 10.0f;
					fixtureDef.friction = 1.3f;
					body[score]->CreateFixture(&fixtureDef);
					score++;
				}
			}			
		}		
	}
	~Map() {
		delete mapLoader;
		delete mapObj;
	}
	/*void reset() {
		if (mapLoader != nullptr) {
			delete mapLoader;
			delete mapObj;
		}
		updateMap();
	}*/

	bool getIsColorChange() {
		return isColorChange;
	}
	void setIsColorChange(bool isColorChange) {
		this->isColorChange = isColorChange;
	}

	void updateMap() {
		for (int i = 0; i < objCount; i++) {
			b2Vec2 positionBlock = body[i]->GetPosition();
			sf::Color color = convex[i].getFillColor();

			if (worldColor == objectColor[i]) {
				body[i]->SetEnabled(false);
				if (isColorChange == true) {					
					color.a = 128;
					convex[i].setFillColor(color);
				}							
			}
			else {
				body[i]->SetEnabled(true);
				color.a = 255;
				convex[i].setFillColor(color);
			}
			convex[i].setPosition(sf::Vector2f(positionBlock.x, WINDOW_HEIGHT - positionBlock.y - rect[i].height));
		}
	}

	void draw(sf::RenderWindow* window) {
		updateMap();
		window->draw(*mapLoader);
		for (int i = 0; i < objCount; i++) {
			if (worldColor != objectColor[i] || isColorChange == true) {
				window->draw(convex[i]);
			}
		}
		if (isColorChange) {
			window->draw(shadow);
			window->draw(colorOne);
			window->draw(colorTwo);
			window->draw(colorThree);
			window->draw(colorFour);
		}
	}
};
