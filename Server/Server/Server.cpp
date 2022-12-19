#include <SFML/Graphics.hpp>
#include "NetworkServerManager.h"
#include <thread>
#include <chrono>

NetworkServerManager server;


int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 300), "SFML server");
    sf::CircleShape shape(10.f);
    sf::CircleShape shape1(10.f);
    shape.setFillColor(sf::Color::Green);
    shape1.setFillColor(sf::Color::Green);
    
    sf::Clock clock;


    while (window.isOpen()){
        //only update every 20th of a second
        sf::Time deltaTime = clock.restart();
        if (deltaTime.asSeconds() < 1 / 20.f) {
            std::this_thread::sleep_for(std::chrono::milliseconds((100 / 2) - deltaTime.asMilliseconds()));
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //IN THE GAME LOOP WE HAVE TO CALL THE NETWORK MANAGER FUNCTIONS!

        //Check if the server is full
        //If it isn't continue to check for new connections
        if (!server.isServerFull()) {
            server.listenForClients();
        }
        
        //check if any sockets have recieved data
        server.receivePackets(deltaTime.asSeconds());


        //send player positions;
        server.sendPlayerData();

        shape.setPosition(server.player0.getPosition());
        shape1.setPosition(server.player1.getPosition());

        window.clear();
        window.draw(shape);
        window.draw(shape1);
        window.display();
    }

    return 0;
}