#pragma once

#include "triangle_utils.hpp"
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

class Cube {

private:
    
float f;
float cubeSize;
int x, y, z; //logical coordinates
float cx, cy, cz; //physical coordinates
float camX, camY, camZ;
float angleX, angleY, angleZ;
std::vector<TriangleUtils::Triangle> triangles;
sf::Color getColor(char);
std::pair<bool, TriangleUtils::Point> transformVertex(TriangleUtils::Vertex&);

public:
char colorFront;
char colorBack;
char colorLeft;
char colorRight;
char colorTop;
char colorBottom;
Cube() {};
Cube(float, float, float, float);
Cube(float, float, float, float, char, char, char, char, char, char);
void setCubeSize(float);
void setCubeColor(char, char, char, char, char, char);
void setFocalLength(float);
float getFocalLength();
std::vector<TriangleUtils::Triangle> getTriangles();
void setCamera(float, float, float);
void setCubeCenter(float, float, float);
TriangleUtils::Vertex getCubeCenter();
void addVertices();
void rotateX(float, float, float, float, float);
void rotateY(float, float, float, float, float);
void rotateZ(float, float, float, float, float);
void rotateAxis(float, float, float, float, float, float, float, float);
};


