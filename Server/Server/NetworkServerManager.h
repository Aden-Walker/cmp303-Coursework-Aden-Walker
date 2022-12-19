#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include "GameObject.h"
#include <vector>

class NetworkServerManager
{
	enum PACKETTYPE {
		NAME = 0,
		POSITION
	};


private:
	const short PORT = 25565;
	const short MAX_PLAYERS = 2;

	sf::TcpListener listener;
	sf::TcpSocket* newClient;

	std::vector<sf::TcpSocket*> clients;

	bool isFullFlag;

	void receivePacket(sf::TcpSocket* client);

public:
	NetworkServerManager();

	bool isServerFull();

	void listenForClients();
	
	void sendPacketToClient(sf::TcpSocket* client, sf::Packet packet); 
	
	void receivePackets(float deltaTime);

	void predictPositions(sf::TcpSocket* client, float deltaTime);

	void sendPlayerData();

	sf::Vector2f lerp(sf::Vector2f pos1, sf::Vector2f pos2, float time);



	GameObject player0;
	GameObject player1;
};

