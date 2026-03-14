#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Rectangle.hpp"
#include "Utils.hpp"

#define FPS 60
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

const float dt = 1.f / FPS;

std::vector<Rectangle> objects;

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D GAME ENGINE!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);

    Player player(0.f, 150.f, 300.f, 0.f, 200.f, 130.f);
    Rectangle floor(0.f, 0.f, 600.f, 500.f, 500.f);
    
    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                player.moveForward(dt);
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                player.moveBackward(dt);
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                player.moveLeft(dt);
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                player.moveRight(dt);
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                player.rotateLeft(dt);
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                player.rotateRight(dt);
            }
        }

        Rectangle::camX = player.getCamX();
        Rectangle::camY = player.getCamY();
        Rectangle::camZ = player.getCamZ();

        window.clear();
        floor.render(window);
        window.display();
    }
}