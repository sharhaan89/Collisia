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
Utils::Triangle tr1;
Utils::Triangle tr2;

public:

static Vec3 cam;
static float focalLength;

Rectangle(const Vec3& pos, const Vec3& normal, float width, float height);
void setTriangles();
void render(sf::RenderWindow& window);
void setCenter(Vec3& pos);
void setDimensions(float width, float height);
};