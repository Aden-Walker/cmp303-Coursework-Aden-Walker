#include "GameObject.h"


GameObject::GameObject()
{
	speed = sf::Vector2f(0, 0);
	hasController = false;
	controller = nullptr;
}

GameObject::~GameObject()
{
}


void GameObject::setPosition(sf::Vector2f newPos)
{
	if (newPos.x > 0 &&
		newPos.y > 0 &&
		newPos.x < 400 &&
		newPos.y < 300) {
		position = newPos;
	}
}

void GameObject::setPredictedPosition(sf::Vector2f newPos)
{
	if (newPos.x > 0 &&
		newPos.y > 0 &&
		newPos.x < 400 &&
		newPos.y < 300) {
		predictedPos = newPos;
	}
}

sf::Vector2f GameObject::getPredictedPos()
{
	return predictedPos;
}

sf::Vector2f GameObject::getPosition()
{
	return position;
}

void GameObject::updateSpeed(sf::Vector2f lastPos)
{
	speed = position - lastPos;
}

sf::Vector2f GameObject::getSpeed()
{
	return speed;
}
