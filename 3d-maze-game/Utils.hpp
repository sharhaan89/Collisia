#pragma once

#include <SFML/Graphics.hpp>

namespace Utils {
    
struct Point {
    float x;
    float y;
};

struct Vertex {
    float x;
    float y;
    float z;
};  

struct Triangle {
    char color;
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

sf::Color getColor(char color);
float getTriangleDepth(Triangle& t, float camZ);
Point transformVertex(Vertex& vertex, float camX, float camY, float camZ, float f, float WINDOW_WIDTH, float WINDOW_HEIGHT);
void drawTriangle(sf::RenderWindow& window, Triangle& tr, float camX, float camY, float camZ, float f);

}