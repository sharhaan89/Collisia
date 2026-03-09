#include <SFML/Graphics.hpp>
#include <cmath>

#define FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define g 50 * 9.8
#define e 1

const float dt = 1.0 / FPS;
const float circleRadius = 350.f;
const float cx = WINDOW_WIDTH * 0.5f;
const float cy = WINDOW_HEIGHT * 0.5f;

float bx = cx + 200;
float by = cy - 100;
float vx = 300.f;
float vy = 186.f;
float ballRadius = 20.f;

void handleBallMovement(sf::CircleShape& ball) {
    vy += g * dt;
    bx += vx * dt;
    by += vy * dt;
    ball.setPosition({bx, by});
}

void handleCollision(sf::CircleShape& ball, sf::CircleShape& circle) {
    float ccdistsq = (bx - cx) * (bx - cx) + (by - cy) * (by - cy);
    float threshold = (circleRadius - ballRadius) * (circleRadius - ballRadius);
    if(ccdistsq < threshold) {
        return;
    }

    float vx0 = vx;
    float vy0 = vy;
    float dx = bx - cx;
    float dy = by - cy;
    float d = sqrt(dx * dx + dy * dy);
    float nx = dx / d;
    float ny = dy / d;
    
    float dot = vx0 * nx + vy0 * ny;

    vx = vx0 - (1 + e) * dot * nx;
    vy = vy0 - (1 + e) * dot * ny;

    ballRadius += 5.f;
    if(ballRadius >= circleRadius) ballRadius = circleRadius - 20.f;
    ball.setRadius(ballRadius);
    ball.setOrigin({ballRadius, ballRadius});

    float theta = atan((bx - cx) / (by - cy));
    bx = cx + (circleRadius - ballRadius) * nx;
    by = cy + (circleRadius - ballRadius) * ny;

    ball.setPosition({bx, by});

    vx *= 1.2;
    vy *= 1.2;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Ball in Circle Simulation!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);
    
    //draw the circle
    sf::CircleShape circle(circleRadius);
    circle.setOrigin({circleRadius, circleRadius});
    circle.setPosition({cx, cy});
    circle.setFillColor(sf::Color::Black);
    circle.setOutlineThickness(2.f);
    circle.setOutlineColor(sf::Color::White);
    
    //draw the ball
    sf::CircleShape ball(ballRadius);
    ball.setOrigin({ballRadius, ballRadius});
    ball.setPosition({cx, cy});
    ball.setFillColor(sf::Color::Yellow);

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>())
                window.close();
        }
        
        window.clear();
        handleBallMovement(ball);
        handleCollision(ball, circle);
        window.draw(circle);
        window.draw(ball);
        window.display();
    }
}