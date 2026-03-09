#include "cube.hpp"
#include "triangle_utils.hpp"
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

#define FPS 60
#define rotationSpeed 7.f
// #define WINDOW_WIDTH 700
// #define WINDOW_HEIGHT 700

using TriangleUtils::Triangle;

float f = 400.0f;
float cubeSize = 1.f;
float offset = cubeSize + 0.1f;
float ox = 0.f;
float oy = 0.f;
float oz = 10.f;
float camX = 0.f;
float camY = 0.f;
float camZ = 0.f;
bool rotating = false;
bool zooming = false;

std::vector<Cube> rubixCube;
std::vector<TriangleUtils::Triangle> triangles;

Cube& getCube(int x, int y, int z) {
    int idx = (x - 1) * 9 + (y - 1) * 3 + z - 1;
    return rubixCube[idx];
}

void createCubes() {
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            for(int z = -1; z <= 1; z++) {
                float cx = x * offset + ox;
                float cy = y * offset + oy;
                float cz = z * offset + oz;

                Cube cube(cubeSize, cx, cy, cz, 'R');
                if(x == 0) cube.setCubeColor('G');
                if(x == 1) cube.setCubeColor('B');
                cube.setCamera(camX, camY, camZ);
                cube.setFocalLength(f);
                cube.addVertices();

                rubixCube.push_back(cube);
            }
        }
    }
}

//painters for all
void drawCubes(sf::RenderWindow& window) {
    triangles.clear();

    for(auto& cube : rubixCube) {
        auto currentTriangles = cube.getTriangles();
        triangles.insert(triangles.end(), currentTriangles.begin(), currentTriangles.end());
    }
    std::sort(triangles.begin(), triangles.end(), [](Triangle& a, Triangle& b) {
        return getTriangleDepth(a, camZ) > getTriangleDepth(b, camZ);
    });
    for(auto& triangle : triangles) {
        drawTriangle(window, triangle, camX, camY, camZ, f);
    }
}

//just rotates a face
void rotateFaceX(int x) {
    auto& centerCube = getCube(x, 2, 2);
    auto centerVertex = centerCube.getCubeCenter();
    float fcx = centerVertex.x;
    float fcy = centerVertex.y;
    float fcz = centerVertex.z;

    //i need to rotate around any arbitrary axis now
    
    //axis vector
    auto& firstCube = getCube(1, 2, 2);
    auto& lastCube = getCube(3, 2, 2);
    auto firstVertex = firstCube.getCubeCenter();
    auto lastVertex = lastCube.getCubeCenter();
    auto fcx0 = firstVertex.x;
    auto fcy0 = firstVertex.y;
    auto fcz0 = firstVertex.z;
    float fcx1 = lastVertex.x;
    float fcy1 = lastVertex.y;
    float fcz1 = lastVertex.z;
    float dx = fcx1 - fcx0;
    float dy = fcy1 - fcy0;
    float dz = fcz1 - fcz0;

    for(int y = 1; y <= 3; y++) {
        for(int z = 1; z <= 3; z++) {
            auto& currentCube = getCube(x, y, z);
            currentCube.rotateAxis(1, rotationSpeed, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateFaceY(int y) {
    auto& centerCube = getCube(2, y, 2);
    auto centerVertex = centerCube.getCubeCenter();
    float fcx = centerVertex.x;
    float fcy = centerVertex.y;
    float fcz = centerVertex.z;

    //i need to rotate around any arbitrary axis now
    
    //axis vector
    auto& firstCube = getCube(2, 1, 2);
    auto& lastCube = getCube(2, 3, 2);
    auto firstVertex = firstCube.getCubeCenter();
    auto lastVertex = lastCube.getCubeCenter();
    auto fcx0 = firstVertex.x;
    auto fcy0 = firstVertex.y;
    auto fcz0 = firstVertex.z;
    float fcx1 = lastVertex.x;
    float fcy1 = lastVertex.y;
    float fcz1 = lastVertex.z;
    float dx = fcx1 - fcx0;
    float dy = fcy1 - fcy0;
    float dz = fcz1 - fcz0;

    for(int x = 1; x <= 3; x++) {
        for(int z = 1; z <= 3; z++) {
            auto& currentCube = getCube(x, y, z);
            currentCube.rotateAxis(1, rotationSpeed, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateFaceZ(int z) {
    auto& centerCube = getCube(2, 2, z);
    auto centerVertex = centerCube.getCubeCenter();
    float fcx = centerVertex.x;
    float fcy = centerVertex.y;
    float fcz = centerVertex.z;

    //i need to rotate around any arbitrary axis now
    
    //axis vector
    auto& firstCube = getCube(2, 2, 1);
    auto& lastCube = getCube(2, 2, 3);
    auto firstVertex = firstCube.getCubeCenter();
    auto lastVertex = lastCube.getCubeCenter();
    auto fcx0 = firstVertex.x;
    auto fcy0 = firstVertex.y;
    auto fcz0 = firstVertex.z;
    float fcx1 = lastVertex.x;
    float fcy1 = lastVertex.y;
    float fcz1 = lastVertex.z;
    float dx = fcx1 - fcx0;
    float dy = fcy1 - fcy0;
    float dz = fcz1 - fcz0;

    for(int y = 1; y <= 3; y++) {
        for(int x = 1; x <= 3; x++) {
            auto& currentCube = getCube(x, y, z);
            currentCube.rotateAxis(1, rotationSpeed, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D CUBE!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);
    
    createCubes();

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

    //portalSound.play();

    sf::Font font;
    if(!font.openFromFile("../../../fonts/arial.ttf")) {
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
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1)) {
            rotateFaceX(1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2)) {
            rotateFaceX(2);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad3)) {
            rotateFaceX(3);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4)) {
            rotateFaceY(1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5)) {
            rotateFaceY(2);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6)) {
            rotateFaceY(3);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad7)) {
            rotateFaceZ(1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8)) {
            rotateFaceZ(2);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9)) {
            rotateFaceZ(3);
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