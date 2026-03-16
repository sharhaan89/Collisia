#pragma once

#include <SFML/Graphics.hpp>

namespace Utils {
    
struct Point {
    float x;
    float y;
};

struct Vec3 {
    float x;
    float y;
    float z;
};  

struct Triangle {
    char color;
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
};

sf::Color getColor(char color);
float getTriangleDepth(Triangle& t, float camZ);
Point transformVertex(Vec3& vertex, Vec3& cam, float f, float WINDOW_WIDTH, float WINDOW_HEIGHT);
void drawTriangle(sf::RenderWindow& window, Triangle& tr, Vec3& cam, float f);

}