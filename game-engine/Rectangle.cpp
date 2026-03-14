#include "Rectangle.hpp"

float Rectangle::camX = 0.f;
float Rectangle::camY = 0.f;
float Rectangle::camZ = 10.f;
float Rectangle::focalLength = 500.f;

Rectangle::Rectangle(float x, float y, float z, float width, float height) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->width = width;
    this->height = height;
    setTriangles();
}

void Rectangle::setTriangles() {
    float gx = width * 0.5f;
    float gz = height * 0.5f;
    tr1 = {'R', {x + gx, y, z + gz}, {x - gx, y, z - gz}, {x - gx, y, z + gz}};
    tr2 = {'G', {x - gx, y, z - gz}, {x + gx, y, z + gz}, {x + gx, y, z - gz}};
}

void Rectangle::setCenter(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Rectangle::setDimensions(float width, float height) {
    this->width = width;
    this->height = height;
}

void Rectangle::render(sf::RenderWindow& window) {
    Utils::drawTriangle(window, tr1, camX, camY, camZ, focalLength);
    Utils::drawTriangle(window, tr2, camX, camY, camZ, focalLength);
}