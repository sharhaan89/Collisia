#include <SFML/Graphics.hpp>
#include <cmath>

#define FPS 1
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

float dt = 1;
float scale = 50;
float gap = WINDOW_WIDTH / (scale * scale);
bool showGrids = true;

void drawPoint(sf::RenderWindow& window, float x, float y, char color) {
    x += WINDOW_WIDTH * 0.5f;
    y += WINDOW_HEIGHT * 0.5f;
    sf::CircleShape point(2.f);
    point.setOrigin({2.f, 2.f});
    point.setPosition({x, y});
    sf::Color pointColor;

    switch(color) {
        case 'R': pointColor = sf::Color::Red; break;
        case 'B': pointColor = sf::Color::Blue; break;
        case 'G': pointColor = sf::Color::Green; break;
        case 'Y': pointColor = sf::Color::Yellow; break;
        case 'C': pointColor = sf::Color::Cyan; break;
        case 'M': pointColor = sf::Color::Magenta; break;
        default: pointColor = sf::Color::White;
    }
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

void drawAxes(sf::RenderWindow& window) {
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
}

void drawGrids(sf::RenderWindow& window) {
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
}

//kinda bruteforce but ok like what else can i do
void drawSine(sf::RenderWindow& window) {
    for(float px = -WINDOW_WIDTH * 0.5f; px <= WINDOW_WIDTH * 0.5f; px += gap) {
        float x = px / scale;
        float y = -sin(x);
        float py = y * scale;
        drawPoint(window, px, py, 'B');
    }
}

//brother of sine
void drawCosine(sf::RenderWindow& window) {
    for(float px = -WINDOW_WIDTH * 0.5f; px <= WINDOW_WIDTH * 0.5f; px += gap) {
        float x = px / scale;
        float y = -cos(x);
        float py = y * scale;
        drawPoint(window, px, py, 'Y');
    }
}

//y = x^3
void drawCubic(sf::RenderWindow& window) {
    for(float px = -WINDOW_WIDTH * 0.5f; px <= WINDOW_WIDTH * 0.5f; px += gap) {
        float x = px / scale;
        float y = - x * x;
        float py = y * scale;
        drawPoint(window, px, py, 'G');
    }
}

void drawGaussianBellCurve(sf::RenderWindow& window) {
    //y = exp(-x^2) * sin(5*x)
    for(float px = -WINDOW_WIDTH * 0.5f; px <= WINDOW_WIDTH * 0.5f; px += gap) {
        float x = px / scale;
        float y = - exp(- x * x);
        float py = y * scale;
        drawPoint(window, px, py, 'R');
    }
}

/*
we can basically draw any function now
but need to write the equation manually
we need a parser or some shit

rules:
x^n -> x * x * ... * x (n times)
rest remains mostly same actually?
*/


int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Grapher!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);
    
    //buttons for setting the scale
    sf::RectangleShape zoomInButton(sf::Vector2f(40, 30));
    zoomInButton.setPosition({WINDOW_WIDTH - 60, 50});
    zoomInButton.setFillColor(sf::Color::Green);

    sf::RectangleShape zoomOutButton(sf::Vector2f(40, 30));
    zoomOutButton.setPosition({WINDOW_WIDTH - 100, 50});
    zoomOutButton.setFillColor(sf::Color::Red);

    //button for toggling the grid guy
    sf::RectangleShape gridToggleButton(sf::Vector2f(30, 30));
    gridToggleButton.setPosition({WINDOW_WIDTH - 80, 100});
    gridToggleButton.setFillColor(sf::Color::Cyan);

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }

            if(event->is<sf::Event::MouseButtonPressed>()) {
                auto mouse = sf::Mouse::getPosition(window);
                if(zoomInButton.getGlobalBounds().contains({(float)mouse.x, (float)mouse.y})) scale += 5;
                if(zoomOutButton.getGlobalBounds().contains({(float)mouse.x, (float)mouse.y})) scale -= 5;
                if(gridToggleButton.getGlobalBounds().contains({(float)mouse.x, (float)mouse.y})) showGrids = !showGrids;
            }
        }

        window.clear();
        drawAxes(window);
        drawGrids(window);
        drawPoint(window, 0, 0, 'R');
        drawLine(window, 1, 1);
        drawSine(window);
        drawCosine(window);
        drawCubic(window);
        drawGaussianBellCurve(window);
        window.draw(zoomOutButton);
        window.draw(zoomInButton);
        window.draw(gridToggleButton);
        window.display();
    }
}