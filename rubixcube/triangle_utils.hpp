#pragma once

#include <SFML/Graphics.hpp>

namespace TriangleUtils {

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

float getTriangleDepth(Triangle&, float);
std::pair<bool, Point> transformVertex(Vertex&, float, float, float, float, float, float);
sf::Color getColor(char);
void drawTriangle(sf::RenderWindow&, Triangle&, float, float, float, float);

}