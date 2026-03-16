#include "Utils.hpp"
#include <cmath>

#define PI 3.14159265f

namespace Utils {
    
float getTriangleDepth(Triangle& t, float camZ) {
    float z1 = t.v1.z - camZ;
    float z2 = t.v2.z - camZ;
    float z3 = t.v3.z - camZ;
    return (z1 + z2 + z3) / 3.f;
}
    
sf::Color getColor(char color) {
    sf::Color resColor;
    switch(color) {
        case 'W': resColor = sf::Color::White; break;
        case 'G': resColor = sf::Color::Green; break;
        case 'R': resColor = sf::Color::Red; break;
        case 'B': resColor = sf::Color::Blue; break;
        case 'M': resColor = sf::Color::Magenta; break;
        case 'Y': resColor = sf::Color::Yellow; break;
        case 'C': resColor = sf::Color::Cyan; break;
        default: resColor = sf::Color::Black;
    }
    return resColor;
}

Point transformVertex(const Vec3& vertex, const Vec3& cam, float angle, float f, float WINDOW_WIDTH, float WINDOW_HEIGHT) {
    float viewX = vertex.x - cam.x;
    float viewY = vertex.y - cam.y;
    float viewZ = vertex.z - cam.z;

    float rad = -angle * PI / 180.f;
    float rotX = viewX * std::cos(rad) - viewZ * std::sin(rad);
    float rotZ = viewX * std::sin(rad) + viewZ * std::cos(rad);

    if(rotZ <= 0)
        return Point{-6999.f, -6999.f};

    float screenX = rotX / rotZ * f;
    float screenY = viewY / rotZ * f;

    float pixelX = screenX + WINDOW_WIDTH * 0.5f;
    float pixelY = WINDOW_HEIGHT * 0.5f - screenY;

    return Point{pixelX, pixelY};
}

void drawTriangle(sf::RenderWindow& window, Triangle& tr, const Vec3& cam, float angle, float f) {
    float WINDOW_WIDTH = window.getSize().x;
    float WINDOW_HEIGHT = window.getSize().y;

    auto p1 = transformVertex(tr.v1, cam, angle, f, WINDOW_WIDTH, WINDOW_HEIGHT);
    auto p2 = transformVertex(tr.v2, cam, angle, f, WINDOW_WIDTH, WINDOW_HEIGHT);
    auto p3 = transformVertex(tr.v3, cam, angle, f, WINDOW_WIDTH, WINDOW_HEIGHT);

    if(p1.x == -6999.f || p2.x == -6999.f || p3.x == -6999.f)
        return;

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(p1.x, p1.y));
    triangle.setPoint(1, sf::Vector2f(p2.x, p2.y));
    triangle.setPoint(2, sf::Vector2f(p3.x, p3.y));
    triangle.setFillColor(getColor(tr.color));
    triangle.setOutlineThickness(1.f);
    triangle.setOutlineColor(sf::Color::Black);
    window.draw(triangle);   

    // sf::Vertex line1[2];
    // line1[0].position = sf::Vector2f(p1.x, p1.y);
    // line1[0].color = sf::Color::Black;
    // line1[1].position = sf::Vector2f(p2.x, p2.y);
    // line1[1].color = sf::Color::Black;

    // sf::Vertex line2[2];
    // line2[0].position = sf::Vector2f(p2.x, p2.y);
    // line2[0].color = sf::Color::Black;
    // line2[1].position = sf::Vector2f(p3.x, p3.y);
    // line2[1].color = sf::Color::Black;

    // sf::Vertex line3[2];
    // line3[0].position = sf::Vector2f(p3.x, p3.y);
    // line3[0].color = sf::Color::Black;
    // line3[1].position = sf::Vector2f(p1.x, p1.y);
    // line3[1].color = sf::Color::Black;

    // window.draw(line1, 2, sf::PrimitiveType::Lines);
    // window.draw(line2, 2, sf::PrimitiveType::Lines);
    // window.draw(line3, 2, sf::PrimitiveType::Lines);
}

}