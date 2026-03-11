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
bool rotatingCube = false;
bool zooming = false;

//handle the rotation ka simulation
bool rotatingFace = false;
float rotatingDone = 0.f;
int rotatingAxis = -1;
int rotatingLayer = -1;
float steps = 6; //number of degrees to rotate per frame 

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
    createCubes();

    rotatingFace = false;
    rotatingDone = 0.f;
    rotatingAxis = -1;
    rotatingLayer = -1;
    steps = 6;
}

//just rotates a face
void rotateFaceX(int x, float rotationAmount) {
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
            currentCube.rotateAxis(1, rotationAmount, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateFaceY(int y, float rotationAmount) {
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
            currentCube.rotateAxis(1, rotationAmount, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateFaceZ(int z, float rotationAmount) {
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
            currentCube.rotateAxis(1, rotationAmount, fcx, fcy, fcz, dx, dy, dz);
        }
    }
}

void rotateLogicallyX(int x) {
    //anti-clockwise rotation
    for(int y = 0; y < 3; y++) {
        for(int z = y + 1; z < 3; z++) {
            std::swap(rubixCube[x][y][z], rubixCube[x][z][y]);
        }
    }
    
    for(int z = 0; z < 3; z++) {
        std::swap(rubixCube[x][0][z], rubixCube[x][2][z]);
    }
}

void rotateLogicallyY(int y) {
    //clockwise rotation
    for(int x = 0; x < 3; x++) {
        for(int z = x + 1; z < 3; z++) {
            std::swap(rubixCube[x][y][z], rubixCube[z][y][x]);
        }
    }
    
    for(int x = 0; x < 3; x++) {
        std::swap(rubixCube[x][y][0], rubixCube[x][y][2]);
    }
}

void rotateLogicallyZ(int z) {
    //anti-clockwise rotation
    for(int x = 0; x < 3; x++) {
        for(int y = x + 1; y < 3; y++) {
            std::swap(rubixCube[x][y][z], rubixCube[y][x][z]);
        }
    }
    
    for(int y = 0; y < 3; y++) {
        std::swap(rubixCube[0][y][z], rubixCube[2][y][z]);
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
        rotatingCube = false;
        zooming = false;

        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }

            if(const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if(!rotatingFace) {
                if(key->code == sf::Keyboard::Key::Numpad1) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 0;
                    rotatingLayer = 0;
      
                }
                else if(key->code == sf::Keyboard::Key::Numpad2) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 0;
                    rotatingLayer = 1;
                 
                }
                else if(key->code == sf::Keyboard::Key::Numpad3) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 0;
                    rotatingLayer = 2;
                  
                }
                else if(key->code == sf::Keyboard::Key::Numpad4) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 1;
                    rotatingLayer = 0;
               
                }
                else if(key->code == sf::Keyboard::Key::Numpad5) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 1;
                    rotatingLayer = 1;
                   
                }
                else if(key->code == sf::Keyboard::Key::Numpad6) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 1;
                    rotatingLayer = 2;
                   
                }
                else if(key->code == sf::Keyboard::Key::Numpad7) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 2;
                    rotatingLayer = 0;
                   
                }
                else if(key->code == sf::Keyboard::Key::Numpad8) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 2;
                    rotatingLayer = 1;
                  
                }
                else if(key->code == sf::Keyboard::Key::Numpad9) {
                    rotatingFace = true;
                    rotatingDone = 0.f;
                    rotatingAxis = 2;
                    rotatingLayer = 2;
                 
                }
                }
                
                if(key->code == sf::Keyboard::Key::R) {
                    resetCube();
                }
            }
        }

        if(rotatingFace && rotatingDone < 90) {
            if(rotatingAxis == 0) {
                rotateFaceX(rotatingLayer, steps);
                rotatingDone += steps;
            } else if(rotatingAxis == 1) {
                rotateFaceY(rotatingLayer, steps);
                rotatingDone += steps;
            } else if(rotatingAxis == 2) {
                rotateFaceZ(rotatingLayer, steps);
                rotatingDone += steps;
            }
        }

        if(rotatingDone >= 90) {
            if(rotatingAxis == 0) rotateLogicallyX(rotatingLayer);
            else if(rotatingAxis == 1) rotateLogicallyY(rotatingLayer);
            else if(rotatingAxis == 2) rotateLogicallyZ(rotatingLayer);
            rotatingFace = false;
            rotatingDone = 0.f;
            rotatingAxis = -1;
            rotatingLayer = -1;
            
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