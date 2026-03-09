#include "cube.hpp"
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

#define FPS 60
#define rotationSpeed 7.f
// #define WINDOW_WIDTH 700
// #define WINDOW_HEIGHT 700

float f = 400.0f;
float cubeSize = 1.f;
float offset = cubeSize + 0.3f;
float ox = 0.f;
float oy = 0.f;
float oz = 10.f;
float camX = 0.f;
float camY = 0.f;
float camZ = 0.f;
bool rotating = false;
bool zooming = false;

std::vector<Cube> rubixCube;
std::vector<Triangle> triangles;

float getTriangleDepth(Triangle& t) {
    float z1 = t.v1.z - camZ;
    float z2 = t.v2.z - camZ;
    float z3 = t.v3.z - camZ;
    return (z1 + z2 + z3) / 3.f;
}

std::pair<bool, Point> transformVertex(Vertex& vertex) {
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

void drawTriangle(sf::RenderWindow& window, Triangle& tr) {
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
    triangle.setOutlineThickness(1.f);
    triangle.setOutlineColor(sf::Color::Cyan);

    window.draw(triangle);
}

//painters for all
void drawCubes(sf::RenderWindow& window) {
    triangles.clear();

    for(auto& cube : rubixCube) {
        auto currentTriangles = cube.getTriangles();
        triangles.insert(triangles.end(), currentTriangles.begin(), currentTriangles.end());
    }
    std::sort(triangles.begin(), triangles.end(), [](Triangle& a, Triangle& b) {
        return getTriangleDepth(a) > getTriangleDepth(b);
    });
    for(auto& triangle : triangles) {
        drawTriangle(window, triangle);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D CUBE!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);
    
    //create the cubes
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            for(int z = -1; z <= 1; z++) {
                float cx = x * offset + ox;
                float cy = y * offset + oy;
                float cz = z * offset + oz;

                Cube cube(cubeSize, cx, cy, cz, 'R');
                cube.setCamera(camX, camY, camZ);
                cube.setFocalLength(f);
                cube.addVertices();

                rubixCube.push_back(cube);
            }
        }
    }

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.png");
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale({
        float(window.getSize().x) / backgroundTexture.getSize().x,
        float(window.getSize().y) / backgroundTexture.getSize().y
    });
    sf::SoundBuffer portalBuffer;
    sf::SoundBuffer rotationBuffer;
    sf::SoundBuffer zoomBuffer;
    portalBuffer.loadFromFile("portal.wav");
    rotationBuffer.loadFromFile("rotate.wav");
    zoomBuffer.loadFromFile("zoom.wav");
    sf::Sound portalSound(portalBuffer);
    sf::Sound rotationSound(rotationBuffer);
    sf::Sound zoomSound(zoomBuffer);
    portalSound.setLooping(true);
    rotationSound.setLooping(true);
    zoomSound.setLooping(true);

    portalSound.play();

    sf::Font font;
    if(!font.openFromFile("../../fonts/arial.ttf")) {
        //handle error
    }
    sf::Text focalInfo(font);
    sf::Text angleXInfo(font);
    sf::Text angleYInfo(font);
    sf::Text angleZInfo(font);

    float margin = 200.f;
    focalInfo.setCharacterSize(25);
    angleXInfo.setCharacterSize(25);
    angleYInfo.setCharacterSize(25);
    angleZInfo.setCharacterSize(25);
    focalInfo.setFillColor(sf::Color::Black);
    angleXInfo.setFillColor(sf::Color::Black);
    angleYInfo.setFillColor(sf::Color::Black);
    angleZInfo.setFillColor(sf::Color::Black);

    auto bounds = focalInfo.getLocalBounds();
    focalInfo.setPosition({WINDOW_WIDTH - bounds.size.x - margin, 10});

    bounds = angleXInfo.getLocalBounds();
    angleXInfo.setPosition({WINDOW_WIDTH - bounds.size.x - margin, 40});

    bounds = angleYInfo.getLocalBounds();
    angleYInfo.setPosition({WINDOW_WIDTH - bounds.size.x - margin, 70});

    bounds = angleZInfo.getLocalBounds();
    angleZInfo.setPosition({WINDOW_WIDTH - bounds.size.x - margin, 100});

    while(window.isOpen()) {
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        rotating = false;
        zooming = false;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            for(auto& cube : rubixCube) cube.rotateY(1, rotationSpeed, ox, oy, oz);
            rotating = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            for(auto& cube : rubixCube) cube.rotateY(-1, rotationSpeed, ox, oy, oz);
            rotating = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            for(auto& cube : rubixCube) cube.rotateX(1, rotationSpeed, ox, oy, oz);
            rotating = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            for(auto& cube : rubixCube) cube.rotateX(-1, rotationSpeed, ox, oy, oz);
            rotating = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            for(auto& cube : rubixCube) cube.rotateZ(1, rotationSpeed, ox, oy, oz);
            rotating = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
            for(auto& cube : rubixCube) cube.rotateZ(-1, rotationSpeed, ox, oy, oz);
            rotating = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
            for(auto& cube : rubixCube) f += 1.f;
            zooming = true;
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
            for(auto& cube : rubixCube) f -= 1.f;
            zooming = true;
        }

        if(rotating) {
            if(rotationSound.getStatus() != sf::Sound::Status::Playing)
                rotationSound.play();
        } else {
            if(rotationSound.getStatus() == sf::Sound::Status::Playing)
                rotationSound.stop();
        }

        if(zooming) {
            if(zoomSound.getStatus() != sf::Sound::Status::Playing) {
                zoomSound.play();
            }
        } else {
            if(zoomSound.getStatus() == sf::Sound::Status::Playing) {
                zoomSound.stop();
            }
        }

        window.clear();
        // focalInfo.setString("Focal length: " + std::to_string((int)f));
        // angleXInfo.setString("Angle X: " + std::to_string((int)std::round(angleX)));
        // angleYInfo.setString("Angle Y: " + std::to_string((int)std::round(angleY)));
        // angleZInfo.setString("Angle Z: " + std::to_string((int)std::round(angleZ)));
        window.draw(backgroundSprite);
        drawCubes(window);
        // window.draw(focalInfo);
        // window.draw(angleXInfo);
        // window.draw(angleYInfo);
        // window.draw(angleZInfo);
        window.display();
    }
}