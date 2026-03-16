#pragma once

#include <SFML/Graphics.hpp>
#include "Utils.hpp"

class Rectangle {

private:

float x;
float y;
float z;
char color;
float width;
float height;
Utils::Triangle tr1;
Utils::Triangle tr2;

public:

static float camX;
static float camY;
static float camZ;
static float focalLength;

Rectangle(float x, float y, float z, float width, float height);
void setTriangles();
void render(sf::RenderWindow& window);
void setCenter(float x, float y, float z);
void setDimensions(float width, float height);
};