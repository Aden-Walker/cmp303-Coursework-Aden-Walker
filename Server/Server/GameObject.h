#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#include "SFML\Network.hpp"


class GameObject
{
public:
	GameObject();
	~GameObject();

	
	void setPosition(sf::Vector2f newPos);
	sf::Vector2f getPosition();

	void setPredictedPosition(sf::Vector2f newPos);
	sf::Vector2f getPredictedPos();

	sf::TcpSocket* controller;
	std::string clientID = "";
	bool hasController;
	void updateSpeed(sf::Vector2f lastPos);
	sf::Vector2f getSpeed();

protected:
	// game object data
	sf::Vector2f speed;
	sf::Vector2f position;
	sf::Vector2f predictedPos;
	sf::Time timeSinceLastPacket;
};

