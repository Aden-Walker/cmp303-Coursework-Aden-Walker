#include "NetworkServerManager.h"

NetworkServerManager::NetworkServerManager()
{
	//Creates a container to where clients will be stored when connecting for the first time
	newClient = new sf::TcpSocket();
	newClient->setBlocking(false);

	//Start the listening socket
	listener.setBlocking(false);
	if (listener.listen(PORT) != sf::Socket::Done) {
		std::cout << "Error creating listening socket\n";
	}

	isFullFlag = false;

	std::cout << "Server is ready to listen for connections\n";

	player0.setPosition(sf::Vector2f(20, 150));
	player1.setPosition(sf::Vector2f(200, 150));
	player0.setPredictedPosition(player0.getPosition());
	player1.setPredictedPosition(player1.getPosition());
	player0.hasController = false;
;	player1.hasController = false;
}

bool NetworkServerManager::isServerFull()
{
	return isFullFlag;
}

void NetworkServerManager::listenForClients()
{
	auto status = listener.accept(*newClient);

	//check if there is a connection to accept, if none stop function
	if (status == sf::Socket::NotReady) {
		return;
	}

	//Check if there is any errors in accepting a new connection
	if (status != sf::Socket::Done) {
		std::cout << "Error in accepting new connection\n";
		return;
	}

	//Check if player can actually join the game:
	if (clients.size() > MAX_PLAYERS) {
		newClient->disconnect();
		return;
	}


	// player has connected so send them ID and set game object controller
	//Create a SFML Packet with the ID
	int clientID = clients.size(); //IDs will start at 0
	sf::Packet idPacket;
	idPacket << clientID;
	
	//Sends packet to the client with id
	sendPacketToClient(newClient, idPacket);
	sf::Packet pos;
	if (!player0.hasController) {
		player0.controller = newClient;
		player0.hasController = true;
		pos << player0.getPosition().x << player0.getPosition().y;
		sendPacketToClient(newClient, pos);
	}
	else {
		player1.controller = newClient;
		player1.hasController = true;
		pos << player1.getPosition().x << player1.getPosition().y;
		sendPacketToClient(newClient, pos);
	}
	

	//Assume the client receives the packet
	//add client connection to vector
	clients.push_back(newClient);
	std::cout << "New Client has connected\n";

	//Check if server is full
	isFullFlag = clients.size() >= MAX_PLAYERS;

	//reset new client buffer pointer
	newClient = new sf::TcpSocket();
	newClient->setBlocking(false);
}

void NetworkServerManager::sendPacketToClient(sf::TcpSocket* client, sf::Packet packet)
{
	
	if (client->send(packet) != sf::Socket::Done) {
		//DO ERROR HANDLING HERE
		std::cout << "Error in sending packet to client\n";
	}
}

void NetworkServerManager::receivePackets(float deltaTime)
{
	//Loop through all connect clients to check if they are receiving any data
	sf::SocketSelector selector;
	for (sf::TcpSocket* client : clients) {
		selector.add(*client);
		if (selector.wait(sf::milliseconds(50))) {
			receivePacket(client);
		}
		else {
			predictPositions(client, deltaTime);
		}
		selector.clear();
			
	}
}

void NetworkServerManager::predictPositions(sf::TcpSocket* client, float deltaTime)
{
	if (client == player0.controller) {
		player0.setPredictedPosition(player0.getPosition() + (player0.getSpeed()*deltaTime));
	}
	else if (client == player1.controller) {
		player1.setPredictedPosition(player1.getPosition() + (player1.getSpeed()*deltaTime));
	}
}

void NetworkServerManager::sendPlayerData()
{
	// loop through each client to send data
	if (clients.size() > 0) {
		sf::Packet position;
		if (player0.hasController) {
			position << PACKETTYPE::POSITION << (int)player1.getPosition().x << (int) player1.getPosition().y;
			sendPacketToClient(player0.controller, position);
			//std::cout << "Sent X: " << (int)player1.getPosition().x << " Y: " << (int)player1.getPosition().y << "to player0 ";
			position.clear();
		}

		if (player1.hasController) {
			position << PACKETTYPE::POSITION << (int)player0.getPosition().x << (int)player0.getPosition().y;
			sendPacketToClient(player1.controller, position);
			//std::cout << "Sent X: " << (int)player0.getPosition().x << " Y: " << (int)player0.getPosition().y << "to player1\n";
			position.clear();
		}
	}
	
}

sf::Vector2f NetworkServerManager::lerp(sf::Vector2f pos1, sf::Vector2f pos2, float time)
{
	return (1 - time) * pos1 + time * pos2;
}

void NetworkServerManager::receivePacket(sf::TcpSocket* client)
{
	sf::Packet packetContainer;
	auto status = client->receive(packetContainer);

	//check if there is a data to receive, if none stop function
	if (status == sf::Socket::NotReady) {
		//Socket doesnt have any data to receive
		return;
	}

	//Check if there is any errors in receiving data
	if (status != sf::Socket::Done) {
		//DO ERROR HANDLING HERE
		std::cout << "Error in receiving data\n";

		if (status == sf::Socket::Disconnected) {
			std::cout << "Client disconnected\n";

			if (client == player0.controller) {
				player0.controller = nullptr;
				player0.hasController = false;
			}
			else if (client == player1.controller) {
				player1.controller = nullptr;
				player1.hasController = false;
			}

			for (int i = 0; i < clients.size(); i++) {
				if (client == clients[i]) {
					clients.erase(clients.begin() + i);
					isFullFlag = false;
				}
			}
		}

		return;
	}

	//Unpack the data and process it
	int packetType;

	//Check if it unpacked first type correctly
	if (packetContainer >> packetType)
	{


		PACKETTYPE pType = (PACKETTYPE)packetType; //Convert the int to ENUM

		switch (pType)
		{
		case NAME:
		{
			//If you receive a NAME packet, then update this client player name!
			std::string playerName;

			if (packetContainer >> playerName) {
				std::cout << "Received Player Name: " << playerName << "\n";
				if (client == player0.controller) {
					player0.clientID = playerName;
				}
				else {
					player1.clientID = playerName;
				}
			}
			break;
		}

		case POSITION:
		{
			//If you receive a POSITION packet update the game object
			//that has this client as a controller
			//also update the speed value for position prediction
			float x, y;

			if (packetContainer >> x >> y) {
				if (client == player0.controller) {
					sf::Vector2f lastPos = player0.getPosition();
					player0.setPosition(lerp(sf::Vector2f(x, y), player0.getPredictedPos(), 0.1));
					player0.updateSpeed(lastPos);
				}
				else {
					sf::Vector2f lastPos = player1.getPosition();
					player1.setPosition(lerp(sf::Vector2f(x, y), player1.getPredictedPos(), 0.1));
					player1.updateSpeed(lastPos);
				}

			}

			break;
		}
		}
	}
}