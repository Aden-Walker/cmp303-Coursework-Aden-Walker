#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "GameObject.h"
#include <chrono>
#include <thread>


sf::Vector2f lerp(sf::Vector2f pos1, sf::Vector2f pos2, float time) {
    return (1 - time) * pos1 + time * pos2;
}

int main()
{
    //initialise keyboard input
    sf::Keyboard keys;
    bool updateInput = true;
    
    // clock object
    sf::Clock clock;

    //player objects and vector for bullets
    GameObject player0;
    GameObject player1;
    
    

    float syncTimer = 5.f;



    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(400, 300), "CMP303 Client");
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

    socket.receive(idPacket);
    float startX, startY;
    if (idPacket >> startX >> startY) {
        player0.setPosition(sf::Vector2f(startX, startY));
    }


    

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
                window.close();
            if (event.type == sf::Event::GainedFocus) {
                updateInput = true;
                window.setTitle("SFML Client In Focus");
            }

            if (event.type == sf::Event::LostFocus) {
                updateInput = false;
                window.setTitle("SFML Client Not In Focus");
            }
        }
        
        // handle user input
        if (updateInput) {
            player0.handleInput(deltaTime.asSeconds(), keys);
            player0.setPosition(player0.getSprite().getPosition() + player0.getSpeed());
        }
 
        //send user position
        enumID = 1; // 1 is position
        sf::Packet positionPacket;
        sf::Vector2f playerPos = player0.getSprite().getPosition();
        positionPacket << enumID << playerPos.x << playerPos.y;
        socket.send(positionPacket);

        //send user speed
        //enumID = 2;
        // TODO

        sf::Packet recievePacket;
        sf::SocketSelector selector;
        selector.add(socket);
        //recieve data and interpolate
        selector.wait();
        if (socket.receive(recievePacket) == sf::Socket::Done) {
         
            int packetType;
            int x, y;
            recievePacket >> packetType >> x >> y;
            if (packetType == 1) {
                if ((int)player1.getSprite().getPosition().x != x || (int)player1.getSprite().getPosition().y != y) {
                   
                    player1.setPosition(sf::Vector2f(x, y));
                }
                
            }
        }
        
        


        window.clear(sf::Color(247, 226, 166));
        window.draw(player0.getSprite());
        window.draw(player1.getSprite());
        window.display();
    }

    return 0;
}

