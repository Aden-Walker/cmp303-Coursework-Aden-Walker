#include "GameObject.h"

GameObject::GameObject()
{
	texture.loadFromFile("Player.png");
	sprite = new sf::Sprite(texture);

}

GameObject::~GameObject()
{
}

void GameObject::handleInput(float delta_time, sf::Keyboard keys)
{
	speed = sf::Vector2f(0.f, 0.f);
	float s = 50.f; //s is speed without direction
	if (keys.isKeyPressed(sf::Keyboard::W)) {
		speed += sf::Vector2f(0.f, -s);
	}
	if (keys.isKeyPressed(sf::Keyboard::A)) {
		speed += sf::Vector2f(-s, 0.f);
	}
	if (keys.isKeyPressed(sf::Keyboard::S)) {
		speed += sf::Vector2f(0.f, s);
	}
	if (keys.isKeyPressed(sf::Keyboard::D)) {
		speed += sf::Vector2f(s, 0.f);
	}
	speed *= delta_time;
}

sf::Vector2f GameObject::getSpeed()
{
	return speed;
}

void GameObject::setSpeed(sf::Vector2f newSpeed)
{
	speed = newSpeed;
}

void GameObject::setPosition(sf::Vector2f newPos)
{
	sprite->setPosition(newPos);
}

sf::Sprite GameObject::getSprite()
{
	return *sprite;
}



