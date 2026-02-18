#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define FPS 60
#define g 600
#define e 0.9

const int ss = 1;
const float dt = 1.0f / (FPS * ss);

//we need to create a set of functions that we call sequentially
//to update the properties regarding the ball like position update
//velocity update, collision detection, etc.

class Ball {
private:
    int id;
    float radius;
    float posX, posY;
    float velX, velY;
    float accX, accY;
    sf::CircleShape shape;
    
public:
    Ball(int id, float radius, float posX, float posY, float velX, float velY) {
        this->id = id;
        this->radius = radius;
        this->posX = posX;
        this->posY = posY;
        this->velX = velX;
        this->velY = velY;
        this->accX = 0.f;
        this->accY = 0.f;
        
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin({radius, radius});
        shape.setPosition({posX, posY});
    }
    
    float getRadius() {
        return radius;
    }

    void setRadius(float radius) {
        this->radius = radius;
        shape.setRadius(radius);
    }

    int getId() {
        return id;
    }
    
    std::pair<float, float> getPos() {
        return {posX, posY};
    }

    void setPos(float posX, float posY) {
        this->posX = posX;
        this->posY = posY;
    }
    
    std::pair<float, float> getVel() {
        return {velX, velY};
    }

    void setVel(float velX, float velY) {
        this->velX = velX;
        this->velY = velY;
    }

    std::pair<float, float> getAcc() {
        return {accX, accY};
    }
    
    void updatePos() {
        posX += velX * dt;
        posY += velY * dt;
        shape.setPosition({posX, posY});
    }
    
    void updateVel() {
        velX += accX * dt;
        velY += accY * dt;
        velY += g * dt;
    }
    
    void handleCollisionWalls() {
        if(posX - radius < 0) {
            posX = radius;
            velX = -velX * e;
        } else if(posX + radius > WINDOW_WIDTH) {
            posX = WINDOW_WIDTH - radius; 
            velX = -velX * e;
        }

        if(posY - radius < 0) {
            posY = radius;
            velY = -velY * e;
        } else if(posY + radius > WINDOW_HEIGHT) {
            posY = WINDOW_HEIGHT - radius;
            velY = -velY * e;
        }
    }
    
    void simulate(sf::RenderWindow& window) {
        updateVel();
        updatePos(); 
        handleCollisionWalls();
        window.draw(shape);       
    }
};

float getDist(Ball& ball1, Ball& ball2) {
    auto [x1, y1] = ball1.getPos();
    auto [x2, y2] = ball2.getPos();
    float r1 = ball1.getRadius();
    float r2 = ball2.getRadius();
    return sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
}

bool detectCollision(Ball& ball1, Ball& ball2) {
    float d = getDist(ball1, ball2);
    float r1 = ball1.getRadius();
    float r2 = ball2.getRadius();
    return (d <= r1 + r2);
}

void handleCollision(Ball& ball1, Ball& ball2) {
    auto [x1, y1] = ball1.getPos();
    auto [x2, y2] = ball2.getPos();
    auto [vx1, vy1] = ball1.getVel();
    auto [vx2, vy2] = ball2.getVel();
    
    float r1 = ball1.getRadius();
    float r2 = ball2.getRadius();

    float m1 = r1 * r1;
    float m2 = r2 * r2;

    float v1 = sqrt(vx1 * vx1 + vy1 * vy1);
    float v2 = sqrt(vx2 * vx2 + vy2 * vy2);
    float theta1 = atan2(vy1, vx1);
    float theta2 = atan2(vy2, vx2);
    float phi = atan2(y2 - y1, x2 - x1);

    float v1n = v1 * cos(theta1 - phi);
    float v1t = v1 * sin(theta1 - phi);
    float v2n = v2 * cos(theta2 - phi);
    float v2t = v2 * sin(theta2 - phi);

    float v1nf = (m1 * v1n + m2 * v2n + m2 * e * (v2n - v1n)) / (m1 + m2);
    float v2nf = (m1 * v1n + m2 * v2n + m1 * e * (v1n - v2n)) / (m1 + m2);

    float v1fx = v1nf * cos(phi) - v1t * sin(phi);
    float v1fy = v1nf * sin(phi) + v1t * cos(phi);

    float v2fx = v2nf * cos(phi) - v2t * sin(phi);
    float v2fy = v2nf * sin(phi) + v2t * cos(phi);

    ball1.setVel(v1fx, v1fy);
    ball2.setVel(v2fx, v2fy);

    float dist = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float minSafeDist = r1 + r2;
    
    if(dist < minSafeDist) {
        float overlap = minSafeDist - dist;
        float nx = (x2 - x1) / dist;
        float ny = (y2 - y1) / dist;

        float totalM = m1 + m2;
        ball1.setPos(x1 - nx * overlap * (m2 / totalM), y1 - ny * overlap * (m2 / totalM));
        ball2.setPos(x2 + nx * overlap * (m1 / totalM), y2 + ny * overlap * (m1 / totalM));
    }
}

void handleCollisionBalls(std::vector<Ball>& balls) {
    for(int i = 0; i + 1 < (int)balls.size(); i++) {
        for(int j = i + 1; j < (int)balls.size(); j++) {
            if(detectCollision(balls[i], balls[j])) {
                handleCollision(balls[i], balls[j]);
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Balls Simulator");
    window.setFramerateLimit(FPS);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> posDist(50.0f, 750.0f); 
    std::uniform_real_distribution<float> velDist(-200.0f, 200.0f);
    
    std::vector<Ball> ballsList;
    for(int i = 0; i < 15; i++) {
        float randomX = posDist(gen);
        float randomY = posDist(gen);
        float randomVX = velDist(gen);
        float randomVY = velDist(gen);

        ballsList.push_back(Ball(i, 15.0f, randomX, randomY, randomVX, randomVY));
    }
    
    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) window.close();
        }

        window.clear(sf::Color::Black);
        for(Ball& ball : ballsList) {
            ball.simulate(window);
        }
        handleCollisionBalls(ballsList);
        window.display();
    }

    return 0;
}