#include <string>
#include "textBox.h"

TextBox::TextBox(std::string text, unsigned int textSize, sf::Vector2f position)
{
    setFont(font);
    setCharacterSize(textSize);
    setFillColor(sf::Color::White);
    setString(text);
    setOrigin(
        (getLocalBounds().width  / 2) + getLocalBounds().left,
        (getLocalBounds().height / 2) + getLocalBounds().top
    ); //set the origin to the middle of the text
    setPosition(position);
}
