#include "cube.hpp"
#include "triangle_utils.hpp"
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>
#include <random>
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
bool rotatingCube = false;
bool zooming = false;

//handle the rotation ka simulation
bool rotatingFace = false;
float rotatingDone = 0.f;
int rotatingAxis = -1;
int rotatingLayer = -1;
float steps = 6; //number of degrees to rotate per frame 

//simple solver by destacking the moves in reverse
int stateIndex = -1;
bool isSolving = false;
std::vector<std::pair<int, int>> moves; //{axis, layer}

std::vector<TriangleUtils::Triangle> triangles;
Cube rubixCube[3][3][3];

void createCubes() {
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            for(int z = -1; z <= 1; z++) {
                float cx = x * offset + ox;
                float cy = y * offset + oy;
                float cz = z * offset + oz;

                Cube cube(cubeSize, cx, cy, cz);
                cube.setCamera(camX, camY, camZ);
                cube.setFocalLength(f);

                cube.colorRight = 'S';
                cube.colorLeft = 'S';
                cube.colorTop = 'S';
                cube.colorBottom = 'S';
                cube.colorFront = 'S';
                cube.colorBack = 'S';

                if(x == 1) cube.colorRight = 'R';
                if(x == -1) cube.colorLeft = 'M';
                if(y == 1) cube.colorTop = 'W';
                if(y == -1) cube.colorBottom = 'Y';
                if(z == -1) cube.colorFront = 'G';
                if(z == 1) cube.colorBack = 'B';

                cube.addVertices();
                rubixCube[x + 1][y + 1][z + 1] = cube;
            }
        }
    }
}

//painters for all
void drawCubes(sf::RenderWindow& window) {
    triangles.clear();

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                auto currentTriangles = rubixCube[i][j][k].getTriangles();
                triangles.insert(triangles.end(), currentTriangles.begin(), currentTriangles.end());
            }
        }
    }
    std::sort(triangles.begin(), triangles.end(), [](Triangle& a, Triangle& b) {
        return getTriangleDepth(a, camZ) > getTriangleDepth(b, camZ);
    });
    for(auto& triangle : triangles) {
        drawTriangle(window, triangle, camX, camY, camZ, f);
    }
}

void resetCube() {
    triangles.clear();
    moves.clear();
    createCubes();

    rotatingFace = false;
    isSolving = false;
    stateIndex = -1;
    rotatingDone = 0.f;
    rotatingAxis = -1;
    rotatingLayer = -1;
    steps = 6;
}

void rotateFace(int axis, int layer) {
    rotatingFace = true;
    rotatingDone = 0.f;
    rotatingAxis = axis;
    rotatingLayer = layer;
}

//just rotates a face
void rotateFaceX(int x, int dir, float rotationAmount) {
    auto& centerCube = rubixCube[x][1][1];
    auto centerVertex = centerCube.getCubeCenter();
    float fcx = centerVertex.x;
    float fcy = centerVertex.y;
    float fcz = centerVertex.z;

    //i need to rotate around any arbitrary axis now
    
    //axis vector
    auto& firstCube = rubixCube[0][1][1];
    auto& lastCube = rubixCube[2][1][1];
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

    for(int y = 0; y < 3; y++) {
        for(int z = 0; z < 3; z++) {
            auto& currentCube = rubixCube[x][y][z];
            currentCube.rotateAxis(dir, rotationAmount, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateFaceY(int y, int dir, float rotationAmount) {
    auto& centerCube = rubixCube[1][y][1];
    auto centerVertex = centerCube.getCubeCenter();
    float fcx = centerVertex.x;
    float fcy = centerVertex.y;
    float fcz = centerVertex.z;

    // axis vector
    auto& firstCube = rubixCube[1][0][1];
    auto& lastCube  = rubixCube[1][2][1];

    auto firstVertex = firstCube.getCubeCenter();
    auto lastVertex  = lastCube.getCubeCenter();

    float dx = lastVertex.x - firstVertex.x;
    float dy = lastVertex.y - firstVertex.y;
    float dz = lastVertex.z - firstVertex.z;

    for(int x = 0; x < 3; x++) {
        for(int z = 0; z < 3; z++) {
            auto& currentCube = rubixCube[x][y][z];
            currentCube.rotateAxis(dir, rotationAmount, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateFaceZ(int z, int dir, float rotationAmount) {
    auto& centerCube = rubixCube[1][1][z];
    auto centerVertex = centerCube.getCubeCenter();
    
    float fcx = centerVertex.x;
    float fcy = centerVertex.y;
    float fcz = centerVertex.z;
    
    // axis vector
    auto& firstCube = rubixCube[1][1][0];
    auto& lastCube  = rubixCube[1][1][2];
    
    auto firstVertex = firstCube.getCubeCenter();
    auto lastVertex  = lastCube.getCubeCenter();
    
    float dx = lastVertex.x - firstVertex.x;
    float dy = lastVertex.y - firstVertex.y;
    float dz = lastVertex.z - firstVertex.z;
    
    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 3; y++) {
            auto& currentCube = rubixCube[x][y][z];
            currentCube.rotateAxis(dir, rotationAmount, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateLogicallyX(int x, int direction) {
    //anti-clockwise rotation
    for(int y = 0; y < 3; y++) {
        for(int z = y + 1; z < 3; z++) {
            std::swap(rubixCube[x][y][z], rubixCube[x][z][y]);
        }
    }
    
    if(direction == 1) {
        for(int z = 0; z < 3; z++) {
            std::swap(rubixCube[x][0][z], rubixCube[x][2][z]);
        }
    } else if(direction == -1) {
        for(int y = 0; y < 3; y++) {
            std::swap(rubixCube[x][y][0], rubixCube[x][y][2]);
        }
    }
}

void rotateLogicallyY(int y, int direction) {
    //clockwise rotation
    for(int x = 0; x < 3; x++) {
        for(int z = x + 1; z < 3; z++) {
            std::swap(rubixCube[x][y][z], rubixCube[z][y][x]);
        }
    }
    
    if(direction == 1) {
        for(int x = 0; x < 3; x++) {
            std::swap(rubixCube[x][y][0], rubixCube[x][y][2]);
        }
    } else if(direction == -1) {
        for(int z = 0; z < 3; z++) {
            std::swap(rubixCube[0][y][z], rubixCube[2][y][z]);
        }
    }
}

void rotateLogicallyZ(int z, int direction) {
    //anti-clockwise rotation
    for(int x = 0; x < 3; x++) {
        for(int y = x + 1; y < 3; y++) {
            std::swap(rubixCube[x][y][z], rubixCube[y][x][z]);
        }
    }
    
    if(direction == 1) {
        for(int y = 0; y < 3; y++) {
            std::swap(rubixCube[0][y][z], rubixCube[2][y][z]);
        }
    } else if(direction == -1) {
        for(int x = 0; x < 3; x++) {
            std::swap(rubixCube[x][0][z], rubixCube[x][2][z]);
        }
    }
}

void scrambleCube(int scramblingFactor) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);

    for(int i = 0; i < scramblingFactor; i++) {
        int randomAxis = distrib(gen);
        int randomLayer = distrib(gen);
        if(randomAxis == 0) {
            rotateFaceX(randomLayer, 1, 90);
            rotateLogicallyX(randomLayer, 1);
        }
        else if(randomAxis == 1) {
            rotateFaceY(randomLayer, 1, 90);
            rotateLogicallyY(randomLayer, 1);
        }
        else if(randomAxis == 2) {
            rotateFaceZ(randomLayer, 1, 90);
            rotateLogicallyZ(randomLayer, 1);
        }

        moves.push_back({randomAxis, randomLayer});
    }

    stateIndex = moves.size() - 1;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "3D RUBIX CUBE!", sf::Style::Titlebar | sf::Style::Close);
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
        rotatingCube = false;
        zooming = false;

        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }

            if(const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if(!rotatingFace) {
                if(key->code == sf::Keyboard::Key::Numpad1) {
                    rotateFace(0, 0);
                    moves.push_back({0, 0});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad2) {
                    rotateFace(0, 1);
                    moves.push_back({0, 1});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad3) {
                    rotateFace(0, 2);
                    moves.push_back({0, 2});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad4) {
                    rotateFace(1, 0);
                    moves.push_back({1, 0});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad5) {
                    rotateFace(1, 1);
                    moves.push_back({1, 1});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad6) {
                    rotateFace(1, 2);
                    moves.push_back({1, 2});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad7) {
                    rotateFace(2, 0);
                    moves.push_back({2, 0});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad8) {
                    rotateFace(2, 1);
                    moves.push_back({2, 1});
                    stateIndex++;
                }
                else if(key->code == sf::Keyboard::Key::Numpad9) {
                    rotateFace(2, 2);
                    moves.push_back({2, 2});
                    stateIndex++;
                } else if(key->code == sf::Keyboard::Key::T) {
                    scrambleCube(10);
                } else if(key->code == sf::Keyboard::Key::Y) {
                    scrambleCube(20);
                } else if(key->code == sf::Keyboard::Key::U) {
                    scrambleCube(30);
                } else if(key->code == sf::Keyboard::Key::K) {
                    rotatingFace = true;
                    isSolving = true;
                    rotatingAxis = moves[stateIndex].first;
                    rotatingLayer = moves[stateIndex].second;
                    stateIndex--;
                }
                }
                
                if(key->code == sf::Keyboard::Key::R) {
                    resetCube();
                } 
            }
        }

        if(isSolving && rotatingDone < 90) {
            if(rotatingAxis == 0) {
                rotateFaceX(rotatingLayer, -1, steps);
                rotatingDone += steps;
            } else if(rotatingAxis == 1) {
                rotateFaceY(rotatingLayer, -1, steps);
                rotatingDone += steps;
            } else if(rotatingAxis == 2) {
                rotateFaceZ(rotatingLayer, -1, steps);
                rotatingDone += steps;
            }
        } else if(rotatingFace && rotatingDone < 90) {
            if(rotatingAxis == 0) {
                rotateFaceX(rotatingLayer, 1, steps);
                rotatingDone += steps;
            } else if(rotatingAxis == 1) {
                rotateFaceY(rotatingLayer, 1, steps);
                rotatingDone += steps;
            } else if(rotatingAxis == 2) {
                rotateFaceZ(rotatingLayer, 1, steps);
                rotatingDone += steps;
            }
        }

        if(rotatingDone >= 90) {
            if(isSolving) {
                if(rotatingAxis == 0) rotateLogicallyX(rotatingLayer, -1);
                else if(rotatingAxis == 1) rotateLogicallyY(rotatingLayer, -1);
                else if(rotatingAxis == 2) rotateLogicallyZ(rotatingLayer, -1);

                if(stateIndex < 0) {
                    moves.clear();
                    isSolving = false;
                    rotatingFace = false;
                    stateIndex = -1;
                    rotatingDone = 0.f;
                    rotatingAxis = -1;
                    rotatingLayer = -1;
                } else {
                    rotatingAxis = moves[stateIndex].first;
                    rotatingLayer = moves[stateIndex].second;
                    rotatingDone = 0.f;
                    stateIndex--;
                }
            } else {
                if(rotatingAxis == 0) rotateLogicallyX(rotatingLayer, 1);
                else if(rotatingAxis == 1) rotateLogicallyY(rotatingLayer, 1);
                else if(rotatingAxis == 2) rotateLogicallyZ(rotatingLayer, 1);
                rotatingFace = false;
                rotatingDone = 0.f;
                rotatingAxis = -1;
                rotatingLayer = -1;
            }

        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        rubixCube[x][y][z].rotateY(1, rotationSpeed, ox, oy, oz);
            rotatingCube = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        rubixCube[x][y][z].rotateY(-1, rotationSpeed, ox, oy, oz);
            rotatingCube = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        rubixCube[x][y][z].rotateX(1, rotationSpeed, ox, oy, oz);
            rotatingCube = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        rubixCube[x][y][z].rotateX(-1, rotationSpeed, ox, oy, oz);
            rotatingCube = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        rubixCube[x][y][z].rotateZ(1, rotationSpeed, ox, oy, oz);
            rotatingCube = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
            for(int x = 0; x < 3; x++)
                for(int y = 0; y < 3; y++)
                    for(int z = 0; z < 3; z++)
                        rubixCube[x][y][z].rotateZ(-1, rotationSpeed, ox, oy, oz);
            rotatingCube = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
            f += 30.f;
            zooming = true;
        } 
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
            f -= 30.f;
            zooming = true;
        } 

        if(rotatingFace) {
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