#ifndef COMMONDEF_H_INCLUDED
#define COMMONDEF_H_INCLUDED

#include <string>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button.h"
#include "textBox.h"
#include "slider.h"

using namespace std;

enum ScreenName
{
    none,
    home,
    play,
    lose,
    settings
};

extern sf::Font font;

extern const int FRAMERATE;
extern const int WIN_X_LEN;
extern const int WIN_Y_LEN;

extern int BRICK_HORIZ_NUMBER;
extern float BRICK_HEIGHT;

extern int PADDLE_LENGTH;
extern float PADDLE_SPEED;

extern int score;
extern int highScore;

#endif // COMMONDEF_H_INCLUDED
