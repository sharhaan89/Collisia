#pragma once

#include <SFML/Graphics.hpp>
#include "Utils.hpp"

using Utils::Vec3;

class Rectangle {

private:

Vec3 pos;
Vec3 normal;
char color;
float width;
float height;
char color1;
char color2;
Utils::Triangle tr1;
Utils::Triangle tr2;

public:

static Vec3 cam;
static float focalLength;

Rectangle(const Vec3& pos, const Vec3& normal, float width, float height);
void setTriangles();
void render(sf::RenderWindow& window, const Vec3& cam, float angle, float focalLength);
void setCenter(const Vec3& pos);
void setColor(char c1, char c2);
void setDimensions(float width, float height);
};