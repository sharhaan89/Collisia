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

    Player player(0.f, 0.f, 10.f, 0.f, 200.f, 130.f);
    Rectangle floor(0.f, 0.f, 0.f);
    
    while(window.isOpen()) {
        window.clear();
        floor.render(window);
        window.display();
    }
}