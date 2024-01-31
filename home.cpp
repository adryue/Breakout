#include <iostream>
#include <SFML/Graphics.hpp>
#include "button.h"
#include "home.h"

using namespace std;

ScreenName homeScreen(sf::RenderWindow &window)
{
    TextBox titleText("Brickout", 100, sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 2));

    Button playButton("Play", sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 3 * 2));

    Button settingsButton("Settings", sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN * 0.75));

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return ScreenName::none;
            }
            //check if the player has clicked any buttons
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (playButton.isPressed(window))
                {
                    return ScreenName::play;
                }
                else if (settingsButton.isPressed(window))
                {
                    return ScreenName::settings;
                }
            }
        }
        window.clear();

        //draw everything
        window.draw(titleText);
        window.draw(playButton.graphic);
        window.draw(playButton.text);
        window.draw(settingsButton.graphic);
        window.draw(settingsButton.text);

        window.display();
    }
    return ScreenName::none;
}
