#include <SFML/Graphics.hpp>
#include <cmath>

/*
this is manually made without using the engine i wrote earlier
gotta add this to the engine as well later so i can add pendulums as many as i can :)
*/

#define radius 20.0f
#define gravity 1
#define FPS 60

const float dt = 1.0 / FPS;
const float armLength = 450.0f;

int main() {
    sf::RenderWindow window(sf::VideoMode({1400, 700}), "SHM of Pendulum Simulation!");
    window.setFramerateLimit(FPS);

    float angle = 0.8f;
    float angAcc = 0.f;
    float angVel = 0.f;
    float posX = 700.0f;
    float posY = 550.0f;

    //draw the pendulum
    sf::CircleShape pendulum(radius);
    pendulum.setOrigin({radius, radius});
    pendulum.setPosition({posX, posY});
    pendulum.setFillColor(sf::Color::Yellow);

    //draw the cable
    sf::Vector2f topPoint(700.0f, 100.0f);
    sf::Vector2f bottomPoint(700.0f, 100.0f + armLength);
    sf::VertexArray cable(sf::PrimitiveType::Lines, 2);
    cable[0].position = topPoint;
    cable[0].color = sf::Color::White;
    cable[1].position = bottomPoint;
    cable[1].color = sf::Color::White;

    //draw the support for cable
    sf::VertexArray support(sf::PrimitiveType::Lines, 2);
    support[0].position = sf::Vector2f(600, 100);
    support[0].color = sf::Color::Blue;
    support[1].position = sf::Vector2f(800, 100);
    support[1].color = sf::Color::Green;

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        angAcc = -(gravity / armLength) * std::sin(angle);
        angVel += angAcc;
        angle += angVel;

        posX = 700.0f + armLength * std::sin(angle);
        posY = 100.0f + armLength * std::cos(angle);

        pendulum.setPosition({posX, posY});
        cable[1].position = {posX, posY};
        
        window.clear();
        window.draw(cable);
        window.draw(support);
        window.draw(pendulum);
        window.display();
    }

    return 0;
}