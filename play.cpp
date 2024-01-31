#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "play.h"

using namespace std;

const int TEXT_SIZE = 50;
const int POPUP_DURATION = 120;

float BRICK_OFFSET = TEXT_SIZE * 1.2;

const int PADDLE_HEIGHT = 20;
const float PADDLE_Y_POS = WIN_Y_LEN * 0.9;

const int BALL_LENGTH = 15;
const float BALL_VERT_SPEED = 4.0;
//const float BALL_VERT_SPEED = 4.0;
const float BALL_SPEED_INCREASE = 1.0;
const float BALL_MAX_VERT_SPEED = 12.0;
const float BALL_MAX_HORIZ_SPEED = 5.0;

const int WIN_SCENE_DURATION = 120;

const int DEATH_ZONE_HEIGHT = WIN_Y_LEN * 0.1;

const int SCORE_ADD = 5;

class Entity
{
public:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Rect<float> hitbox;
    sf::RectangleShape texture;

    void setPosition(const sf::Vector2f &p)
    {
        position = p;
        hitbox.left = p.x;
        hitbox.top = p.y;
        texture.setPosition(p);
    }
    void setSize(const sf::Vector2f &s)
    {
        size = s;
        hitbox.width = s.x;
        hitbox.height = s.y;
        texture.setSize(s);
    }
    void move(const sf::Vector2f &offset)
    {
        setPosition(sf::Vector2f(position.x + offset.x, position.y + offset.y));
    }
};

class Brick : public Entity
{
public:
    Brick(const int x, const int y)
    {
        setPosition(sf::Vector2f(x, y));
        setSize(sf::Vector2f((float)WIN_X_LEN / BRICK_HORIZ_NUMBER, BRICK_HEIGHT));
        texture.setFillColor(sf::Color(220, 220, 220));
        texture.setOutlineThickness(-2.0);
        texture.setOutlineColor(sf::Color::Red);
    }
};

class Paddle : public Entity
{
public:
    Paddle()
    {
        setPosition(sf::Vector2f(WIN_X_LEN * 0.5 - (PADDLE_LENGTH / 2), PADDLE_Y_POS));
        setSize(sf::Vector2f(PADDLE_LENGTH, PADDLE_HEIGHT));
        texture.setFillColor(sf::Color::Green);
        //texture.setOutlineThickness(-2);
        //texture.setOutlineColor(sf::Color::Magenta);
    }
    void reset()
    {
        setPosition(sf::Vector2f(WIN_X_LEN * 0.5 - (PADDLE_LENGTH / 2), PADDLE_Y_POS));
    }
    void move(float offset)
    {
        setPosition(sf::Vector2f(position.x + offset, position.y));
    }
    void checkWithinWindow()
    {
        if (position.x < 0)
        {
            setPosition(sf::Vector2f(0, position.y));
        }
        else if (position.x + size.x > WIN_X_LEN)
        {
            setPosition(sf::Vector2f(WIN_X_LEN - size.x, position.y));
        }
    }
};

class Ball : public Entity
{
public:
    sf::Vector2f velocity;
    Ball()
    {
        velocity.y = BALL_VERT_SPEED;
        setPosition(sf::Vector2f(WIN_X_LEN * 0.5 - (BALL_LENGTH / 2), WIN_Y_LEN * 0.5 - (0.5 * BALL_LENGTH)));
        setSize(sf::Vector2f(BALL_LENGTH, BALL_LENGTH));
        texture.setFillColor(sf::Color(100, 100, 255));
        //texture.setOutlineThickness(-2.0);
        //texture.setOutlineColor(sf::Color::Blue);
    }
    void reset()
    {
        velocity.x = 0;
        setPosition(sf::Vector2f(WIN_X_LEN * 0.5 - (BALL_LENGTH / 2), WIN_Y_LEN * 0.5 - (0.5 * BALL_LENGTH)));
    }
    bool checkCollision(const Entity &e)
    {
        if (sf::Rect<float>(sf::Vector2f(position.x, position.y + velocity.y), size).intersects(e.hitbox)) //when adding the vertical speed makes a collision
        {
            velocity.y = -velocity.y;
            return true;
        }
        else if (sf::Rect<float>(sf::Vector2f(position.x + velocity.x, position.y), size).intersects(e.hitbox)) //when adding the horizontal speed makes a collision
        {
            velocity.x = -velocity.x;
            return true;
        }
        return false;
    }
    void checkWithinWindow()
    {
        if (position.y + velocity.y < 0 || position.y + size.y + velocity.y > WIN_Y_LEN)
        {
            velocity.y = -velocity.y;
        }
        if (position.x + velocity.x < 0 || position.x + size.x + velocity.x > WIN_X_LEN)
        {
            velocity.x = -velocity.x;
        }
    }
};

class Text : public sf::Text
{
public:
    Text()
    {
        setFont(font);
        setCharacterSize(TEXT_SIZE);
        setFillColor(sf::Color::Cyan);
    }
    void setText(string text)
    {
        setString(text);
    }
};

class Popup : public sf::Text
{
public:
    int time;
    Popup(const sf::Vector2f &position, const string &text)
    {
        time = 0;
        setFont(font);
        setCharacterSize(TEXT_SIZE * 0.6);
        setString(text);
        setFillColor(sf::Color::Cyan);
        setPosition(position);
    }
    void update()
    {
        time++;
        setFillColor(getFillColor() - sf::Color(0, 0, 0, 255 / POPUP_DURATION));
    }
    bool done()
    {
        return time >= POPUP_DURATION;
    }
};

vector<vector<Brick>> generateBricks()
{
    vector<vector<Brick>> bricks;
    for (int i = 0; i * BRICK_HEIGHT + BRICK_OFFSET < WIN_Y_LEN * 0.4; i++) //i is the row
    {
        vector<Brick> currentBricks;
        for (int j = 0; j * ((float)WIN_X_LEN / BRICK_HORIZ_NUMBER) < WIN_X_LEN; j++) //j is the column
        {
            currentBricks.push_back(Brick(j * ((float)WIN_X_LEN / BRICK_HORIZ_NUMBER), i * BRICK_HEIGHT + BRICK_OFFSET));
        }
        bricks.push_back(currentBricks);
    }
    return bricks;
}

ScreenName playScreen(sf::RenderWindow &window)
{
    //score
    score = 0;
    Text scoreText;
    scoreText.setText(to_string(score));
    int streakMultiplier = 1; //if the ball hits multiple bricks before hitting the paddle again, the streak multiplier will increase
    int scoreAdd = 10;
    vector<Popup> scorePopups;

    //generate bricks
    vector<vector<Brick>> bricks = generateBricks();

    //paddle
    Paddle paddle;

    //ball
    Ball ball;

    //death zone
    sf::RectangleShape deathZone;
    deathZone.setSize(sf::Vector2f(WIN_X_LEN, DEATH_ZONE_HEIGHT));
    deathZone.setPosition(0, PADDLE_Y_POS);
    deathZone.setFillColor(sf::Color::Red);

    //win scenario
    bool win = false;
    int winFrames = 0; //current number of frames that we have showed the win text
    Text winText;
    winText.setText("WIN!");
    winText.setPosition((WIN_X_LEN / 2) - (winText.getLocalBounds().width / 2), (WIN_Y_LEN / 2) - (winText.getLocalBounds().height / 2));
    float ballSpeed = BALL_VERT_SPEED;

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

        if (win)
        {
            if (winFrames < WIN_SCENE_DURATION)
            {
                window.draw(winText);
                winFrames++;
            }
            else
            {
                //reset game
                win = false;
                winFrames = 0;
                bricks = generateBricks();
                scoreAdd += SCORE_ADD;
                paddle.reset();
                ball.reset();
                ballSpeed += BALL_SPEED_INCREASE;
                if (ballSpeed > BALL_MAX_VERT_SPEED)
                {
                    ballSpeed = BALL_MAX_VERT_SPEED;
                }
                ball.velocity.y = ballSpeed;
            }
        }

        //handle player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            paddle.move(-PADDLE_SPEED);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            paddle.move(PADDLE_SPEED);
        }
        paddle.checkWithinWindow();

        //handle ball
        //bounce with bricks
        for (int i = 0; i < bricks.size(); i++)
        {
            for (int j = 0; j < bricks[i].size(); j++)
            {
                if (ball.checkCollision(bricks[i][j]))
                {
                    //destroy the brick
                    bricks[i].erase(bricks[i].begin() + j);
                    j--;
                    //add a popup where the brick was destroyed
                    scorePopups.push_back(Popup(ball.position, "+" + to_string(scoreAdd * streakMultiplier)));
                    //update the score
                    score += scoreAdd * streakMultiplier;
                    streakMultiplier++;
                    scoreText.setString(to_string(score));
                }
            }
            if (bricks[i].empty())
            {
                bricks.erase(bricks.begin() + i);
                i--;
            }
        }
        if (bricks.empty()) //all the bricks are destroyed
        {
            win = true;
        }
        //bounce with paddle
        if (ball.checkCollision(paddle))
        {
            streakMultiplier = 1;

            //alter the velocity of the ball based on where it hit the paddle
            ball.velocity.x += 0.1 * ((ball.position.x + BALL_LENGTH / 2) - (paddle.position.x + PADDLE_LENGTH / 2));
            if (ball.velocity.x < -BALL_MAX_HORIZ_SPEED)
            {
                ball.velocity.x = -BALL_MAX_HORIZ_SPEED;
            }
            else if (ball.velocity.x > BALL_MAX_HORIZ_SPEED)
            {
                ball.velocity.x = BALL_MAX_HORIZ_SPEED;
            }

        }
        ball.checkWithinWindow();
        ball.move(ball.velocity);
        if (ball.position.y + ball.size.y > WIN_Y_LEN - DEATH_ZONE_HEIGHT) //lose
        {
            return ScreenName::lose;
        }

        //draw everything
        window.draw(deathZone);
        for (vector<Brick> &bI : bricks)
        {
            for (Brick &b : bI)
            {
                window.draw(b.texture);
            }
        }
        window.draw(paddle.texture);
        window.draw(ball.texture);
        for (int i = 0; i < scorePopups.size(); i++)
        {
            scorePopups[i].update();
            if (scorePopups[i].done())
            {
                scorePopups.erase(scorePopups.begin() + i);
                i--;
            }
            else
            {
                window.draw(scorePopups[i]);
            }
        }
        window.draw(scoreText);

        // Update the window
        window.display();
    }

    return ScreenName::none;
}
