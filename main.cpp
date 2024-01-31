#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"
#include "home.h"
#include "settings.h"
#include "lose.h"

using namespace std;

const int FRAMERATE = 60;
const int WIN_X_LEN = 800;
const int WIN_Y_LEN = 600;

int BRICK_HORIZ_NUMBER = 10;
float BRICK_HEIGHT = 25.0;

int PADDLE_LENGTH = 120;
float PADDLE_SPEED = 10.0;

int score = 0;
int highScore = 0;

sf::Font font;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WIN_X_LEN, WIN_Y_LEN), "Br[ic]kout");
    window.setFramerateLimit(FRAMERATE);

    font.loadFromFile("arial.ttf");

    ifstream fin;
    fin.open("Settings/Custom.txt");
    fin >> PADDLE_LENGTH;
    fin >> PADDLE_SPEED;
    fin >> BRICK_HORIZ_NUMBER;
    fin >> BRICK_HEIGHT;
    fin.close();

    ScreenName nextScreen = ScreenName::home;

    while (nextScreen != none)
    {
        switch (nextScreen)
        {
        case home:
            nextScreen = homeScreen(window);
            break;
        case play:
            nextScreen = playScreen(window);
            break;
        case lose:
            nextScreen = loseScreen(window);
            break;
        case settings:
            nextScreen = settingsScreen(window);
            break;
        case none:
            break;
        }
    }

    /*
	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();



        // Update the window
        window.display();
    }*/

    return EXIT_SUCCESS;
}
