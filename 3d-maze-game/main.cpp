#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Rectangle.hpp"
#include "Utils.hpp"

using Utils::Vec3;

auto desktop = sf::VideoMode::getDesktopMode();

#define FPS 60
#define WINDOW_WIDTH desktop.size.x
#define WINDOW_HEIGHT desktop.size.y

const float dt = 1.f / FPS;

const float focalLength = 600.f;
const float tileSize = 100.f;
const float floorStartX = 0.f;
const float floorStartZ = 0.f;
const int floorBreadth = 50; //number of tiles
const int floorLength = 50; //number of tiles
const float floorHeight = 0.f; 
const float sprintFactor = 2.f;
const int wallHeight = 5;

Vec3 playerPos = {0.f, 100.f, 200.f};

std::vector<Rectangle> objects;
std::vector<std::vector<Rectangle>> floor(floorLength);
std::vector<std::vector<Rectangle>> boundary(4);

void createFloor() {    
    for(int z = 0; z < floorLength; z++) {
        for(int x = 0; x < floorBreadth; x++) {
            float posX = floorStartX + x * tileSize;
            float posZ = floorStartZ + z * tileSize;
            Rectangle tile(Vec3{posX, floorHeight, posZ}, Vec3{0.f, 1.f, 0.f}, tileSize, tileSize);
            tile.setColor('M', 'G');
            floor[z].push_back(tile);
        }
    }
}

void createBoundaries() {
    //create the left & right boundary walls
    for(int z = 0; z < floorLength; z++) {
        for(int y = 0; y < wallHeight; y++) {
            float posZ = floorStartZ + z * tileSize;
            float posY = tileSize * 0.5f + y * tileSize;
            Rectangle tileLeft(Vec3{floorStartX - tileSize * 0.5f, posY, posZ}, Vec3{1.f, 0.f, 0.f}, tileSize, tileSize); tileLeft.setColor('W', 'Y');
            Rectangle tileRight(Vec3{floorStartX + tileSize * floorBreadth - tileSize * 0.5f, posY, posZ}, Vec3{-1.f, 0.f, 0.f}, tileSize, tileSize); tileRight.setColor('W', 'Y');
            boundary[0].push_back(tileLeft);
            boundary[2].push_back(tileRight);
        }
    }

    //create the front and back walls
    for(int x = 0; x < floorBreadth; x++) {
        for(int y = 0; y < wallHeight; y++) {
            float posX = floorStartX + x * tileSize;
            float posY = tileSize * 0.5f + y * tileSize;
            Rectangle tileFront(Vec3{posX, posY, floorStartZ - tileSize * 0.5f}, Vec3{0.f, 0.f, -1.f}, tileSize, tileSize); tileFront.setColor('W', 'Y');
            Rectangle tileBack(Vec3{posX, posY, floorStartZ + tileSize * floorLength - tileSize * 0.5f}, Vec3{0.f, 0.f, 1.f}, tileSize, tileSize); tileBack.setColor('W', 'Y');
            boundary[1].push_back(tileFront);
            boundary[3].push_back(tileBack);
        }
    }
}

void handleInput(Player& player) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        player.setSprint(true);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        player.moveForward(dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        player.moveBackward(dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        player.rotateLeft(dt);
        //player.moveLeft(dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player.rotateRight(dt);
        //player.moveRight(dt);
    }
    // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
    // }
    // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
    // }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D MAZE GAME!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);

    Player player(playerPos, 0.f, 500.f, 130.f);
    player.setSprintFactor(5.f);
    createFloor();
    createBoundaries();

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        player.setSprint(false);
        handleInput(player);

        window.clear();
        for(auto &z : floor) {
            for(auto &x : z) {
                x.render(window, player.getCam(), player.getAngle(), focalLength);
            }
        }

        for(auto& wall : boundary) {
            for(auto& tile : wall) {
                tile.render(window, player.getCam(), player.getAngle(), focalLength);
            }
        }

        window.display();
    }
}