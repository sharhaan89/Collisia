#include <SFML/Graphics.hpp>
#include <cmath>

#define FPS 1
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

const float dt = 1;
const float scale = 70;
bool showGrids = true;

void drawPoint(sf::RenderWindow& window, float x, float y, char color) {
    sf::CircleShape point(5.f);
    point.setOrigin({5.f, 5.f});
    point.setPosition({x, y});
    sf::Color pointColor = sf::Color::White;
    if(color == 'R') pointColor = sf::Color::Red;
    point.setFillColor(pointColor);
    window.draw(point);
}

//y = mx + c
void drawLine(sf::RenderWindow& window, float m, float c) {
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);

    line[0].position = sf::Vector2f(0.f, (WINDOW_HEIGHT * 0.5f) - (m * (-WINDOW_WIDTH * 0.5f / scale) + c) * scale);
    line[0].color = sf::Color::Magenta;
    line[1].position = sf::Vector2f(WINDOW_WIDTH, (WINDOW_HEIGHT * 0.5f) - (m * (WINDOW_WIDTH * 0.5f / scale) + c) * scale);
    line[1].color = sf::Color::Magenta;

    window.draw(line);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Grapher!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);
    
    //draw the x-axis
    sf::VertexArray x_axis(sf::PrimitiveType::Lines, 2);
    x_axis[0].position = sf::Vector2f(0.0f, WINDOW_HEIGHT * 0.5f);
    x_axis[0].color = sf::Color::Blue;
    x_axis[1].position = sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT * 0.5f);
    x_axis[1].color = sf::Color::Blue;
    window.draw(x_axis);
    
    //draw the y-axis
    sf::VertexArray y_axis(sf::PrimitiveType::Lines, 2);
    y_axis[0].position = sf::Vector2f(WINDOW_WIDTH * 0.5f, 0.0f);
    y_axis[0].color = sf::Color::Blue;
    y_axis[1].position = sf::Vector2f(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT);
    y_axis[1].color = sf::Color::Blue;
    window.draw(y_axis);

    if(showGrids) {
        //draw some horizontal lines
        for(int y = WINDOW_HEIGHT * 0.5f - scale; y > 0; y -= scale) {
            sf::VertexArray horizontalLine(sf::PrimitiveType::Lines, 2);
            horizontalLine[0].position = sf::Vector2f(0.0f, y);
            horizontalLine[1].position = sf::Vector2f(WINDOW_WIDTH, y);
            window.draw(horizontalLine);
        }

        for(int y = WINDOW_HEIGHT * 0.5f + scale; y < WINDOW_HEIGHT; y += scale) {
            sf::VertexArray horizontalLine(sf::PrimitiveType::Lines, 2);
            horizontalLine[0].position = sf::Vector2f(0.0f, y);
            horizontalLine[1].position = sf::Vector2f(WINDOW_WIDTH, y);
            window.draw(horizontalLine);
        }

        //draw some vertical lines
        for(int x = WINDOW_WIDTH * 0.5f - scale; x > 0; x -= scale) {
            sf::VertexArray verticalLine(sf::PrimitiveType::Lines, 2);
            verticalLine[0].position = sf::Vector2f(x, 0.f);
            verticalLine[1].position = sf::Vector2f(x, WINDOW_HEIGHT);
            window.draw(verticalLine);
        }

        for(int x = WINDOW_WIDTH * 0.5f + scale; x < WINDOW_WIDTH; x += scale) {
            sf::VertexArray verticalLine(sf::PrimitiveType::Lines, 2);
            verticalLine[0].position = sf::Vector2f(x, 0.f);
            verticalLine[1].position = sf::Vector2f(x, WINDOW_HEIGHT);
            window.draw(verticalLine);
        }
    }

    //mark the origin
    drawPoint(window, WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 'R');
    drawLine(window, 1, 1);
    window.display();

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        //window.display();
    }
}