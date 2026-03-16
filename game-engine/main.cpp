#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Rectangle.hpp"
#include "Utils.hpp"

#define FPS 60
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

const float dt = 1.f / FPS;

const float tileSize = 100.f;
const int floorBreadth = 50; //number of tiles
const int floorLength = 50; //number of tiles
const float floorHeight = 0.f;

std::vector<Rectangle> objects;
std::vector<std::vector<Rectangle>> floor(floorLength);

void createFloor() {
    float floorStartX = - (floorBreadth * 0.5f) * tileSize;
    float floorStartZ = - (floorLength * 0.5f) * tileSize;
    
    for(int z = 0; z < floorLength; z++) {
        for(int x = 0; x < floorBreadth; x++) {
            float posX = floorStartX + x * tileSize;
            float posZ = floorStartZ + z * tileSize;
            Rectangle tile(posX, floorHeight, posZ, tileSize, tileSize);
            floor[z].push_back(tile);
        }
    }
}

void handleInput(Player& player) {
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

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D GAME ENGINE!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);

    Player player(0.f, 100.f, 0.f, 0.f, 500.f, 130.f);
    createFloor();

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        handleInput(player);

        Rectangle::camX = player.getCamX();
        Rectangle::camY = player.getCamY();
        Rectangle::camZ = player.getCamZ();

        window.clear();
        for(auto &z : floor) {
            for(auto &x : z) {
                x.render(window);
            }
        }
        window.display();
    }
}