#include "settings.h"

ScreenName settingsScreen(sf::RenderWindow &window)
{
    ifstream fin;
    ofstream fout;

    Button backButton("Back", sf::Vector2f(WIN_X_LEN * 0.1, WIN_Y_LEN * 0.1));
    Button saveButton("Save", sf::Vector2f(WIN_X_LEN / 4, WIN_Y_LEN * 0.85));
    Button resetButton("Reset", sf::Vector2f(WIN_X_LEN / 4 * 3, WIN_Y_LEN * 0.85));

    //temporary variables
    float paddleLength = PADDLE_LENGTH;
    float paddleSpeed = PADDLE_SPEED;
    int brickNumber = BRICK_HORIZ_NUMBER;
    float brickHeight = BRICK_HEIGHT;

    //paddle length
    TextBox paddleLengthText("Paddle Length", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.3));
    TextBox paddleLengthValueText(to_string(paddleLength), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.3 - 20));
    Slider paddleLengthSlider(80, 300, paddleLength, 1, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.3));

    //paddle speed
    TextBox paddleSpeedText("Paddle Speed", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.4));
    TextBox paddleSpeedValueText(to_string(paddleSpeed), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.4 - 20));
    Slider paddleSpeedSlider(6.0, 15.0, paddleSpeed, 2, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.4));

    //brick number
    TextBox brickNumberText("Brick Number", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.5));
    TextBox brickNumberValueText(to_string(brickNumber), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.5 - 20));
    Slider brickNumberSlider(3, 50, brickNumber, 0, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.5));

    //brick height
    TextBox brickHeightText("Brick Height", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.6));
    TextBox brickHeightValueText(to_string(brickHeight), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.6 - 20));
    Slider brickHeightSlider(10, 50, brickHeight, 0, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.6));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return ScreenName::none;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (backButton.isPressed(window))
                {
                    return ScreenName::home;
                }
                else if (saveButton.isPressed(window))
                {
                    PADDLE_LENGTH = paddleLength;
                    PADDLE_SPEED = paddleSpeed;
                    BRICK_HORIZ_NUMBER = brickNumber;
                    BRICK_HEIGHT = brickHeight;

                    fout.open("Settings/Custom.txt");
                    fout << paddleLength << endl;
                    fout << paddleSpeed << endl;
                    fout << brickNumber << endl;
                    fout << brickHeight << endl;
                    fout.close();
                }
                else if (resetButton.isPressed(window))
                {
                    fin.open("Settings/Default.txt");
                    fin >> paddleLength;
                    fin >> paddleSpeed;
                    fin >> brickNumber;
                    fin >> brickHeight;
                    fin.close();

                    //apply all the values to the sliders
                    paddleLengthSlider.setValue(paddleLength);
                    paddleLengthValueText.setString(to_string(paddleLength));

                    paddleSpeedSlider.setValue(paddleSpeed);
                    paddleSpeedValueText.setString(to_string(paddleSpeed));

                    brickNumberSlider.setValue(brickNumber);
                    brickNumberValueText.setString(to_string(brickNumber));

                    brickHeightSlider.setValue(brickHeight);
                    brickHeightValueText.setString(to_string(brickHeight));
                }
                paddleLengthSlider.checkForClick(window);
                paddleSpeedSlider.checkForClick(window);
                brickNumberSlider.checkForClick(window);
                brickHeightSlider.checkForClick(window);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                paddleLengthSlider.isClicked = false;
                paddleSpeedSlider.isClicked = false;
                brickNumberSlider.isClicked = false;
                brickHeightSlider.isClicked = false;
            }
        }
        window.clear();

        //paddle length
        paddleLengthSlider.followMouseIfClicked(window);
        paddleLength = paddleLengthSlider.getValue();
        paddleLengthValueText.setString(to_string(paddleLength));
        //paddle speed
        paddleSpeedSlider.followMouseIfClicked(window);
        paddleSpeed = paddleSpeedSlider.getValue();
        paddleSpeedValueText.setString(to_string(paddleSpeed));
        //brick number
        brickNumberSlider.followMouseIfClicked(window);
        brickNumber = (int)(brickNumberSlider.getValue() + 0.5);
        brickNumberValueText.setString(to_string(brickNumber));
        //brick height
        brickHeightSlider.followMouseIfClicked(window);
        brickHeight = brickHeightSlider.getValue();
        brickHeightValueText.setString(to_string(brickHeight));

        //draw everything
        //buttons
        window.draw(backButton.graphic);
        window.draw(backButton.text);
        window.draw(saveButton.graphic);
        window.draw(saveButton.text);
        window.draw(resetButton.graphic);
        window.draw(resetButton.text);
        //paddle length
        window.draw(paddleLengthText);
        window.draw(paddleLengthValueText);
        window.draw(paddleLengthSlider.path);
        window.draw(paddleLengthSlider.knob);
        //paddle speed
        window.draw(paddleSpeedText);
        window.draw(paddleSpeedValueText);
        window.draw(paddleSpeedSlider.path);
        window.draw(paddleSpeedSlider.knob);
        //brick number
        window.draw(brickNumberText);
        window.draw(brickNumberValueText);
        window.draw(brickNumberSlider.path);
        window.draw(brickNumberSlider.knob);
        //brick height
        window.draw(brickHeightText);
        window.draw(brickHeightValueText);
        window.draw(brickHeightSlider.path);
        window.draw(brickHeightSlider.knob);

        window.display();
    }
    return ScreenName::none;
}
