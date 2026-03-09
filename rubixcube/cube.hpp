#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

struct Vertex {
    float x, y, z;
};  

struct Point {
    float x, y;
};

struct Triangle {
    Vertex v1, v2, v3;
    char color;
};

class Cube {

private:
    
float f;
char cubeColor;
float cubeSize;
float cx, cy, cz;
float camX, camY, camZ;
float angleX, angleY, angleZ;
std::vector<Triangle> triangles;

sf::Color getColor(char);
std::pair<bool, Point> transformVertex(Vertex&);
void drawTriangle(sf::RenderWindow&, Triangle&);
void paintersAlgorithm();

public:

Cube(float, float, float, float, char);
void setCubeSize(float);
void setCubeColor(char);
void setFocalLength(float);
float getFocalLength();
std::vector<Triangle> getTriangles();
void setCamera(float, float, float);
void setCubeCenter(float, float, float);
void addVertices();
void rotateX(float, float, float, float, float);
void rotateY(float, float, float, float, float);
void rotateZ(float, float, float, float, float);
float getTriangleDepth(Triangle&);
void drawCube(sf::RenderWindow&);
};


