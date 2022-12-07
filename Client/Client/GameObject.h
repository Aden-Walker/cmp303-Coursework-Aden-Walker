#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"


class GameObject
{
public:
	GameObject();
	~GameObject();

	void handleInput(float delta_time, sf::Keyboard keys);
	sf::Vector2f getSpeed();
	void setSpeed(sf::Vector2f newSpeed);
	void setPosition(sf::Vector2f newPos);
	int controller;
	sf::Sprite getSprite();


protected:
	// game object data
	sf::Vector2f speed;
	sf::FloatRect hitbox;

	//member functions
	void setHitbox(float x, float y, float height, float width) { hitbox = sf::FloatRect(x, y, height, width); }
	sf::Texture* texture;
	sf::Sprite* sprite;
};

