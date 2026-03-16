#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Rectangle.hpp"
#include "Utils.hpp"

using Utils::Vec3;

#define FPS 60
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

const float dt = 1.f / FPS;


const float tileSize = 100.f;
const float floorStartX = 0.f;
const float floorStartZ = 0.f;
const int floorBreadth = 50; //number of tiles
const int floorLength = 50; //number of tiles
const float floorHeight = 0.f;
const int wallHeight = 10;

Vec3 playerPos = {0.f, 69.f, 0.f};

std::vector<Rectangle> objects;
std::vector<std::vector<Rectangle>> floor(floorLength);
std::vector<std::vector<Rectangle>> boundary(4);

void createFloor() {    
    for(int z = 0; z < floorLength; z++) {
        for(int x = 0; x < floorBreadth; x++) {
            float posX = floorStartX + x * tileSize;
            float posZ = floorStartZ + z * tileSize;
            Rectangle tile(Vec3{posX, floorHeight, posZ}, Vec3{0.f, 1.f, 0.f}, tileSize, tileSize);
            floor[z].push_back(tile);
        }
    }
}

void createBoundaries() {
    //create the left boundary wall
    for(int z = 0; z < floorLength; z++) {
        for(int y = 0; y < wallHeight; y++) {
            float posZ = floorStartZ + z * tileSize;
            float posY = tileSize * 0.5f + y * tileSize;
            Rectangle tile(Vec3{-tileSize * 0.5f, posY, posZ}, Vec3{0.f, 0.f, 0.f}, tileSize, tileSize);
            boundary[0].push_back(tile);
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

    Player player(playerPos, 0.f, 500.f, 130.f);
    createFloor();
    createBoundaries();

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        handleInput(player);

        Rectangle::cam = player.getCam();

        window.clear();
        for(auto &z : floor) {
            for(auto &x : z) {
                x.render(window);
            }
        }

        for(auto& cwall : boundary[0]) {
            cwall.render(window);
        }

        window.display();
    }
}