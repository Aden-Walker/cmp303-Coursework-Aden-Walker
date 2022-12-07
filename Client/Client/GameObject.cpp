#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::handleInput(float delta_time, sf::Keyboard keys)
{
	speed = sf::Vector2f(0.f, 0.f);
	if (keys.isKeyPressed(sf::Keyboard::W)) {
		speed += sf::Vector2f(0.f, 1.f);
	}
	if (keys.isKeyPressed(sf::Keyboard::A)) {
		speed += sf::Vector2f(-1.f, 0.f);
	}
	if (keys.isKeyPressed(sf::Keyboard::S)) {
		speed += sf::Vector2f(0.f, -1.f);
	}
	if (keys.isKeyPressed(sf::Keyboard::D)) {
		speed += sf::Vector2f(1.f, 0.f);
	}
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



