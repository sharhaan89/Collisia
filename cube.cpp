#include <SFML/Graphics.hpp>
#include <cmath>
#include <utility>
#include <vector>

#define FPS 60
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define f 400.0f
#define rotationSpeed 7.f

float angleX = 0.f;
float angleY = 0.f;
float angleZ = 0.f;
float cubeSize = 3.f;
float cx = 0.f;
float cy = 0.f;
float cz = 10.f;

float camX = 0.f;
float camY = 0.f;
float camZ = 0.f;

void setCamera(float x, float y, float z) {
    camX = x;
    camY = y;
    camZ = z;
}

void setCubeCenter(float x, float y, float z) {
    cx = x;
    cy = y;
    cz = z;
}

sf::Color getColor(char color) {
    sf::Color resColor;
    switch(color) {
        case 'R': resColor = sf::Color::Red; break;
        case 'B': resColor = sf::Color::Blue; break;
        case 'G': resColor = sf::Color::Green; break;
        case 'Y': resColor = sf::Color::Yellow; break;
        case 'C': resColor = sf::Color::Cyan; break;
        case 'M': resColor = sf::Color::Magenta; break;
        default: resColor = sf::Color::White;
    }
    return resColor;
}

struct Vertex {
    float x;
    float y;
    float z;
};  

struct Point {
    float x;
    float y;
};

struct Triangle {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    char color;
};

std::vector<Triangle> triangles;

void addVertices() {
    triangles.clear();

    float left = cx - cubeSize * 0.5f;
    float right = cx + cubeSize * 0.5f;
    float top = cy + cubeSize * 0.5f;
    float bottom = cy - cubeSize * 0.5f;
    float front = cz - cubeSize * 0.5f;
    float back = cz + cubeSize * 0.5f;

    //Back face
    triangles.push_back(Triangle{{left, top, back}, {right, top, back}, {left, bottom, back}, 'B'});
    triangles.push_back(Triangle{{right, top, back}, {right, bottom, back}, {left, bottom, back}, 'B'});

    //Left face
    triangles.push_back(Triangle{{left, top, back}, {left, top, front}, {left, bottom, back}, 'C'});
    triangles.push_back(Triangle{{left, top, front}, {left, bottom, front}, {left, bottom, back}, 'C'});

    //Right face
    triangles.push_back(Triangle{{right, top, front}, {right, top, back}, {right, bottom, front}, 'G'});
    triangles.push_back(Triangle{{right, top, back}, {right, bottom, back}, {right, bottom, front}, 'G'});

    //Top face
    triangles.push_back(Triangle{{left, top, back}, {right, top, back}, {left, top, front}, 'M'});
    triangles.push_back(Triangle{{right, top, back}, {right, top, front}, {left, top, front}, 'M'});

    //Bottom face
    triangles.push_back(Triangle{{left, bottom, front}, {right, bottom, front}, {left, bottom, back}, 'R'});
    triangles.push_back(Triangle{{right, bottom, front}, {right, bottom, back}, {left, bottom, back}, 'R'});

    //Front face
    triangles.push_back(Triangle{{left, top, front}, {right, top, front}, {left, bottom, front}, 'Y'});
    triangles.push_back(Triangle{{right, top, front}, {right, bottom, front}, {left, bottom, front}, 'Y'});
}

/*
x′ = xcosθ − ysinθ
y′ = xsinθ + ycosθ
*/

void rotateX(float dir) {
    float angle = (3.14 / 180) * rotationSpeed * dir;
    float y, z;
    for(int i = 0; i < 12; i++) {
        auto t = triangles[i];
        y = t.v1.y - cy;
        z = t.v1.z - cz;
        triangles[i].v1.y = y * cos(angle) - z * sin(angle) + cy;
        triangles[i].v1.z = y * sin(angle) + z * cos(angle) + cz;

        y = t.v2.y - cy;
        z = t.v2.z - cz;
        triangles[i].v2.y = y * cos(angle) - z * sin(angle) + cy;
        triangles[i].v2.z = y * sin(angle) + z * cos(angle) + cz;

        y = t.v3.y - cy;
        z = t.v3.z - cz;
        triangles[i].v3.y = y * cos(angle) - z * sin(angle) + cy;
        triangles[i].v3.z = y * sin(angle) + z * cos(angle) + cz;
    }
}

void rotateY(float dir) {
    float angle = (3.14 / 180) * rotationSpeed * dir;
    float x, z;
    for(int i = 0; i < 12; i++) {
        auto t = triangles[i];
        x = t.v1.x - cx;
        z = t.v1.z - cz;
        triangles[i].v1.x = x * cos(angle) + z * sin(angle) + cx;
        triangles[i].v1.z = -x * sin(angle) + z * cos(angle) + cz;

        x = t.v2.x - cx;
        z = t.v2.z - cz;
        triangles[i].v2.x = x * cos(angle) + z * sin(angle) + cx;
        triangles[i].v2.z = -x * sin(angle) + z * cos(angle) + cz;

        x = t.v3.x - cx;
        z = t.v3.z - cz;
        triangles[i].v3.x = x * cos(angle) + z * sin(angle) + cx;
        triangles[i].v3.z = -x * sin(angle) + z * cos(angle) + cz;
    }
}

void rotateZ(float dir) {
    float angle = (3.14 / 180) * rotationSpeed * dir;
    float x, y;
    for(int i = 0; i < 12; i++) {
        auto t = triangles[i];
        x = t.v1.x - cx;
        y = t.v1.y - cy;
        triangles[i].v1.x = x * cos(angle) - y * sin(angle) + cx;
        triangles[i].v1.y = x * sin(angle) + y * cos(angle) + cy;

        x = t.v2.x - cx;
        y = t.v2.y - cy;
        triangles[i].v2.x = x * cos(angle) - y * sin(angle) + cx;
        triangles[i].v2.y = x * sin(angle) + y * cos(angle) + cy;

        x = t.v3.x - cx;
        y = t.v3.y - cy;
        triangles[i].v3.x = x * cos(angle) - y * sin(angle) + cx;
        triangles[i].v3.y = x * sin(angle) + y * cos(angle) + cy;
    }
}

std::pair<bool, Point> transformVertex(Vertex vertex) {
    float viewX = vertex.x - camX;
    float viewY = vertex.y - camY;
    float viewZ = vertex.z - camZ;

    if(viewZ <= 0) {
        return {false, Point{0.f, 0.f}};
    }

    float screenX = viewX / viewZ * f;
    float screenY = viewY / viewZ * f;

    float pixelX = screenX + WINDOW_WIDTH * 0.5f;
    float pixelY = WINDOW_HEIGHT * 0.5f - screenY;

    return {true, Point{pixelX, pixelY}};
}

void drawTriangle(sf::RenderWindow& window, Triangle tr) {
    auto r1 = transformVertex(tr.v1);
    auto r2 = transformVertex(tr.v2);
    auto r3 = transformVertex(tr.v3);

    if(!r1.first || !r2.first || !r3.first) {
        return;
    }

    Point p1 = r1.second;
    Point p2 = r2.second;
    Point p3 = r3.second;

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(p1.x, p1.y));
    triangle.setPoint(1, sf::Vector2f(p2.x, p2.y));
    triangle.setPoint(2, sf::Vector2f(p3.x, p3.y));
    triangle.setFillColor(getColor(tr.color));

    window.draw(triangle);
}

void drawCube(sf::RenderWindow& window) {
    for(auto& triangle : triangles) {
        drawTriangle(window, triangle);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D CUBE!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);
    
    addVertices();

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            rotateY(1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            rotateY(-1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            rotateX(1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            rotateX(-1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            rotateZ(1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
            rotateZ(-1);
        }

        window.clear();
        drawCube(window);
        window.display();
    }
}