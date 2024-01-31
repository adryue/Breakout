#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "CommonDef.h"
#include <string>
#include <SFML/Graphics.hpp>

class Button
{
public:
    sf::Text text;
    sf::RectangleShape graphic;
    sf::Rect<int> hitbox;

    Button(std::string buttonText, sf::Vector2f position);
    void setPosition(sf::Vector2f position);
    bool isPressed(const sf::RenderWindow &window);
};

#endif // BUTTON_H_INCLUDED
