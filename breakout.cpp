#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#define FPS 60
#define PADDLE_WIDTH 200.0f
#define PADDLE_HEIGHT 20.0f
#define BALL_RADIUS 15.0f

bool aiPlayerMode = true;

float paddle_y;
float paddle_x;
float paddle_vx;

int box_rows;
int box_cols;
float box_width;
float box_height;
float gap_x;
float gap_y;

float ball_vx;
float ball_vy;

int box_count;
bool gameOver = false;

const float dt = 1.0 / FPS;

void initVars() {
    paddle_y = 800.0f;
    paddle_x = 700.0f;
    paddle_vx = 700.0f;

    box_rows = 2;
    box_cols = 8;
    box_width = 150.0f;
    box_height = 100.0f;
    gap_x = 20.0f;
    gap_y = 20.0f;

    ball_vx = 600.0f;
    ball_vy = 600.0f;

    box_count = box_rows * box_cols;
    gameOver = false;
}

void displayMessage(sf::RenderWindow& window, sf::Text& message, std::string displayText, std::string color) {
    message.setString(displayText);
    message.setCharacterSize(80);
    if(color == "green") message.setFillColor(sf::Color::Green);
    else if(color == "red") message.setFillColor(sf::Color::Red);
    message.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = message.getLocalBounds();
    message.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
    message.setPosition({1400 / 2.0f, 3 * 900 / 5.0f});

    window.draw(message);
}

void handleWinGame(sf::RenderWindow& window, sf::Text& message) {
    displayMessage(window, message, "YOU WON MY FRIEND :)\nPress R to play again", "green");
    ball_vx = ball_vy = paddle_vx = 0;
    gameOver = true;
}

void handleLoseGame(sf::RenderWindow& window, sf::Text& message) {
    displayMessage(window, message, "YOU LOST MY FRIEND :(\nPress R to play again", "red");
    ball_vx = ball_vy = paddle_vx = 0;
    gameOver = true;
}

void movePaddleLeft(sf::RenderWindow& window, sf::RectangleShape& paddle) {
    paddle_x -= paddle_vx * dt;
    paddle.setPosition(sf::Vector2f(paddle_x, paddle_y));
}

void movePaddleRight(sf::RenderWindow& window, sf::RectangleShape& paddle) {
    paddle_x += paddle_vx * dt;
    paddle.setPosition(sf::Vector2f(paddle_x, paddle_y));
}

void handleBallMovement(sf::RenderWindow& window, sf::CircleShape& ball) {
    float ballX = ball.getPosition().x;
    float ballY = ball.getPosition().y;

    ballX += ball_vx * dt;
    ballY += ball_vy * dt;

    ball.setPosition({ballX, ballY});
}

void handleBallCollisionWall(sf::RenderWindow& window, sf::Text& message, sf::CircleShape& ball) {
    float ballX = ball.getPosition().x;
    float ballY = ball.getPosition().y;

    if(ballX - BALL_RADIUS < 0) {
        ball.setPosition({BALL_RADIUS, ballY});
        ball_vx = -ball_vx;
    } else if(ballX + BALL_RADIUS > 1400) {
        ball.setPosition({1400 - BALL_RADIUS, ballY});
        ball_vx = -ball_vx;
    } 

    if(ballY - BALL_RADIUS < 0) {
        ball.setPosition({ballX, BALL_RADIUS});
        ball_vy = -ball_vy;
    } else if(ballY + BALL_RADIUS > 900) {
        handleLoseGame(window, message);
    }
}

void handleBallCollisionBox(sf::RenderWindow& window, sf::CircleShape& ball, std::vector<std::vector<sf::RectangleShape>>& boxes) {
    float ballX = ball.getPosition().x;
    float ballY = ball.getPosition().y;

    for(int row = 0; row < box_rows; row++) {
        for(int col = 0; col < box_cols; col++) {
            auto intersection = ball.getGlobalBounds().findIntersection(boxes[row][col].getGlobalBounds());
            if(intersection) {
                if(intersection->size.x < intersection->size.y) {
                    ball_vx = -ball_vx;
                } else {
                    ball_vy = -ball_vy;
                }
                boxes[row][col].setSize({0, 0});
                box_count--;

                return;
            }
        }
    }
}

void handleBallCollisionPaddle(sf::RenderWindow& window, sf::CircleShape& ball, sf::RectangleShape& paddle) {
    float paddleX = paddle.getPosition().x;
    float paddleY = paddle.getPosition().y;
    float ballX = ball.getPosition().x;
    float ballY = ball.getPosition().y;

    if(ball.getGlobalBounds().findIntersection(paddle.getGlobalBounds())) {
        ball_vy = -ball_vy;
    }
}

void enableAIPlayer(sf::RenderWindow& window, sf::RectangleShape& paddle, sf::CircleShape& ball) {
    float ballX = ball.getPosition().x;
    float ballY = ball.getPosition().y;

    float paddleX = paddle.getPosition().x;
    
    if(paddleX < ballX) movePaddleRight(window, paddle);
    else if(paddleX > ballX) movePaddleLeft(window, paddle);
}

void startGame(sf::RenderWindow& window) {

    window.clear();
    initVars();

    //configure the paddle
    sf::RectangleShape paddle({PADDLE_WIDTH, PADDLE_HEIGHT});
    paddle.setOrigin(sf::Vector2f(PADDLE_WIDTH * 0.5f, PADDLE_HEIGHT * 0.5f));
    paddle.setPosition({paddle_x, paddle_y});
    
    //configure the boxes
    std::vector<std::vector<sf::RectangleShape>> boxes(box_rows, std::vector<sf::RectangleShape>(box_cols));
    for(int row = 0; row < box_rows; row++) {
        for(int col = 0; col < box_cols; col++) {
            float box_x = 105.0f + (box_width + gap_x) * col;
            float box_y = 100.0f + (box_height + gap_y) * row;
    
            sf::RectangleShape box({box_width, box_height});
            box.setOrigin(sf::Vector2f(box_width * 0.5f, box_height * 0.5f));
            box.setPosition({box_x, box_y});
    
            boxes[row][col] = box;
        }
    }
    
    //configure the ball
    sf::CircleShape ball(BALL_RADIUS);
    ball.setOrigin({BALL_RADIUS, BALL_RADIUS});
    ball.setFillColor(sf::Color::Red);
    ball.setPosition({paddle_x, paddle_y - 50.0f});
    
    //configure the font for messages
    sf::Font font;
    if(!font.openFromFile("../../fonts/arial.ttf")) {
        // handle error
    }
    sf::Text message(font);
    
    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
    
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if(keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if(keyPressed->code == sf::Keyboard::Key::R) {
                    startGame(window);
                }
            }
        }

        if(aiPlayerMode) {
            enableAIPlayer(window, paddle, ball);
        } else {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                movePaddleLeft(window, paddle);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                movePaddleRight(window, paddle);
            }
        }

        window.clear();
    
        //draw the paddle
        window.draw(paddle);
    
        //draw the boxes
        for(int row = 0; row < box_rows; row++) {
            for(int col = 0; col < box_cols; col++) {
                window.draw(boxes[row][col]);
            }
        }
    
        //draw the ball
        handleBallCollisionWall(window, message, ball);
        handleBallCollisionPaddle(window, ball, paddle);
        handleBallCollisionBox(window, ball, boxes);
        handleBallMovement(window, ball);
        window.draw(ball);
    
        if(box_count == 0) {
            handleWinGame(window, message);
        }
    
        window.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1400, 900}), "Pong Game!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);

    startGame(window);

    return 0;
}