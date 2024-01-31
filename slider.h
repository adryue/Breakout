#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "commonDef.h"

class Slider
{
public:
    sf::CircleShape knob;
    sf::RectangleShape path;

    float minValue;
    float maxValue;
    int precision;

    bool isClicked;

    Slider(float minV, float maxV, float initV, int inc, sf::Vector2f position);

    void checkForClick(const sf::RenderWindow &window);
    void followMouseIfClicked(const sf::RenderWindow &window);

    float getValue(); //get the current value of the slider
    void setValue(float value);

    void ensureWithinBounds();
};

#endif // SLIDER_H_INCLUDED
