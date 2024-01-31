#include <math.h>
#include <SFML/Graphics.hpp>
#include "slider.h"

const int KNOB_RADIUS = 10;
const sf::Vector2f PATH_SIZE(400, 20);

Slider::Slider(float minV, float maxV, float initV, int inc, sf::Vector2f position)
{
    minValue = minV;
    maxValue = maxV;
    precision = inc;

    isClicked = false;

    knob.setFillColor(sf::Color(100, 100, 100));
    knob.setRadius(KNOB_RADIUS);
    knob.setOrigin(KNOB_RADIUS, KNOB_RADIUS);
    knob.setPosition(position);

    path.setFillColor(sf::Color::White);
    path.setSize(PATH_SIZE);
    path.setPosition(position.x - path.getLocalBounds().width / 2,
                     position.y - path.getLocalBounds().height / 2);

    setValue(initV);
}

void Slider::checkForClick(const sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f knobPos = knob.getPosition();

    if ((mousePos.x - knobPos.x) * (mousePos.x - knobPos.x) + (mousePos.y - knobPos.y) * (mousePos.y - knobPos.y) <= KNOB_RADIUS * KNOB_RADIUS)
    {
        isClicked = true;
    }
}
void Slider::followMouseIfClicked(const sf::RenderWindow &window)
{
    if (isClicked)
    {
        knob.setPosition(sf::Mouse::getPosition(window).x, knob.getPosition().y);
        ensureWithinBounds();
    }
}

float Slider::getValue()
{
    float value = ((knob.getPosition().x - path.getPosition().x) / path.getLocalBounds().width) * (maxValue - minValue);
    //value = (int)(value * pow(10, precision)) / pow(10, precision);
    //value = (int)(value * 100) / 100;
    return value + minValue;
}
void Slider::setValue(float value)
{
    knob.setPosition(path.getPosition().x + (((value - minValue) / (maxValue - minValue)) * path.getLocalBounds().width), knob.getPosition().y);
    ensureWithinBounds();
}

void Slider::ensureWithinBounds()
{
    if (knob.getPosition().x < path.getPosition().x)
    {
        knob.setPosition(path.getPosition().x, knob.getPosition().y);
    }
    else if (knob.getPosition().x > path.getPosition().x + path.getLocalBounds().width)
    {
        knob.setPosition(path.getPosition().x + path.getLocalBounds().width, knob.getPosition().y);
    }
}
