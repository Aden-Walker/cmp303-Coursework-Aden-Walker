#include <SFML/Graphics.hpp>
#include "NetworkServerManager.h"

NetworkServerManager server;

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //IN THE GAME LOOP WE HAVE TO CALL THE NETWORK MANAGER FUNCTIONS!

        //The example here does the following:
        //Check if the server is full
        //If it isn't continue to check for new connections
        //YOU CAN ADD MORE CONDITIONS!
        if (!server.isServerFull()) {
            server.listenForClients();
        }
        
        //You have to check every frame or every X time if any of the sockets have received data
        //If they have then you can process afterwards!
        server.receivePackets();


        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}