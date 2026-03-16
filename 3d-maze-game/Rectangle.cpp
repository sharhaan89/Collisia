#include "Rectangle.hpp"
#include <cmath>

using Utils::Vec3;

Rectangle::Rectangle(const Vec3& pos, const Vec3& normal, float width, float height) {
    this->pos = pos;
    this->normal = normal;
    this->width = width;
    this->height = height;
    setTriangles();
}

void Rectangle::setColor(char c1, char c2) {
    color1 = c1;
    color2 = c2;
}

void Rectangle::setTriangles() {
    float len = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    float nx = normal.x / len, ny = normal.y / len, nz = normal.z / len;

    Vec3 arbitrary = (std::abs(ny) < 0.9f) ? Vec3{0, 1, 0} : Vec3{1, 0, 0};

    Vec3 u = {
        ny * arbitrary.z - nz * arbitrary.y,
        nz * arbitrary.x - nx * arbitrary.z,
        nx * arbitrary.y - ny * arbitrary.x
    };

    float uLen = std::sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
    u = {u.x / uLen * width * 0.5f, u.y / uLen * width * 0.5f, u.z / uLen * width * 0.5f};

    Vec3 v = {
        ny * u.z - nz * u.y,
        nz * u.x - nx * u.z,
        nx * u.y - ny * u.x
    };

    float vLen = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v = {v.x / vLen * height * 0.5f, v.y / vLen * height * 0.5f, v.z / vLen * height * 0.5f};

    Vec3 A = {pos.x + u.x + v.x, pos.y + u.y + v.y, pos.z + u.z + v.z};
    Vec3 B = {pos.x - u.x - v.x, pos.y - u.y - v.y, pos.z - u.z - v.z};
    Vec3 C = {pos.x - u.x + v.x, pos.y - u.y + v.y, pos.z - u.z + v.z};
    Vec3 D = {pos.x + u.x - v.x, pos.y + u.y - v.y, pos.z + u.z - v.z};

    tr1 = {color1, A, B, C};
    tr2 = {color2, B, A, D};
}

void Rectangle::setCenter(const Vec3& pos) {
    this->pos = pos;
}

void Rectangle::setDimensions(float width, float height) {
    this->width = width;
    this->height = height;
}

void Rectangle::render(sf::RenderWindow& window, const Vec3& cam, float angle, float focalLength) {
    Utils::drawTriangle(window, tr1, cam, angle, focalLength);
    Utils::drawTriangle(window, tr2, cam, angle, focalLength);
}