#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML\System.hpp>
#include <iostream>
#include <vector>
#include "GameObject.h"


int main()
{
    //initialise keyboard input
    sf::Keyboard keys;
  

    //player objects and vector for bullets
    std::vector<GameObject*> players;
    std::vector<GameObject*> bullets;
    sf::Sprite player1;
    players.push_back(new GameObject);
    players.push_back(new GameObject);
    players[0]->controller = 0; // 0 will be this user 
    players[1]->controller = 1; // 1 is the opponent

    players[0]->setPosition(sf::Vector2f(10, 150));
    players[0]->setPosition(sf::Vector2f(390, 150));



    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(400, 300), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::TcpSocket socket;
    if (socket.connect("127.0.0.1", 25565) != sf::Socket::Done) {
        std::cout << "Error connecting to server\n";
        return 0;
    }

    int clientID;
    sf::Packet idPacket;
    socket.receive(idPacket);
    if (idPacket >> clientID) {
        std::cout << "Received ID: " << std::to_string(clientID) << " from server" << "\n";
    }

    int randomNumber = std::rand() % 99 + 1;
    int enumID = 0; //PACKETTYPE ENUM, 0 corresponds to NAME
    std::string playerName = "Player " + std::to_string(randomNumber);
    sf::Packet namePacket;
    namePacket << enumID << playerName;
    socket.send(namePacket);
    std::cout << "Sent My Name: " << playerName << "\n";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(players[0]->getSprite());
        window.display();
    }

    return 0;
}