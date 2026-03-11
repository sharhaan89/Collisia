#include "cube.hpp"

#pragma comment(lib, "winmm.lib")

using TriangleUtils::Triangle;
using TriangleUtils::Vertex;

#include <cmath>
#include <utility>
#include <algorithm>
#include <vector>
#include <windows.h>
#include <mmsystem.h>

Cube::Cube(float cubeSize, float cx, float cy, float cz) {
    this->cubeSize = cubeSize;
    this->cx = cx;
    this->cy = cy;
    this->cz = cz;
}

Cube::Cube(float cubeSize, float cx, float cy, float cz, char colorFront, char colorBack, char colorLeft, char colorRight, char colorTop, char colorBottom) {
    this->cubeSize = cubeSize;
    this->cx = cx;
    this->cy = cy;
    this->cz = cz;
    this->colorFront = colorFront;
    this->colorBottom = colorBottom;
    this->colorLeft = colorLeft;
    this->colorRight = colorRight;
    this->colorTop = colorTop;
    this->colorBottom = colorBottom;
}

void Cube::setCubeSize(float cs) {
    cubeSize = cs;
}

void Cube::setCamera(float x, float y, float z) {
    camX = x;
    camY = y;
    camZ = z;
}

void Cube::setCubeCenter(float x, float y, float z) {
    cx = x;
    cy = y;
    cz = z;
}

Vertex Cube::getCubeCenter() {
    return Vertex{cx, cy, cz};
}

void Cube::setCubeColor(char colorFront, char colorBack, char colorLeft, char colorRight, char colorTop, char colorBottom) {
    this->colorFront = colorFront;
    this->colorBottom = colorBottom;
    this->colorLeft = colorLeft;
    this->colorRight = colorRight;
    this->colorTop = colorTop;
    this->colorBottom = colorBottom;
}

void Cube::setFocalLength(float focalLength) {
    f = focalLength;
}

float Cube::getFocalLength() {
    return f;
}

std::vector<Triangle> Cube::getTriangles() {
    return triangles;
}

void Cube::addVertices() {
    triangles.clear();

    float left = cx - cubeSize * 0.5f;
    float right = cx + cubeSize * 0.5f;
    float top = cy + cubeSize * 0.5f;
    float bottom = cy - cubeSize * 0.5f;
    float front = cz - cubeSize * 0.5f;
    float back = cz + cubeSize * 0.5f;

    //Back face
    triangles.push_back(Triangle{{left, top, back}, {right, top, back}, {left, bottom, back}, colorBack});
    triangles.push_back(Triangle{{right, top, back}, {right, bottom, back}, {left, bottom, back}, colorBack});

    //Left face 
    triangles.push_back(Triangle{{left, top, back}, {left, top, front}, {left, bottom, back}, colorLeft});
    triangles.push_back(Triangle{{left, top, front}, {left, bottom, front}, {left, bottom, back}, colorLeft});

    //Right face
    triangles.push_back(Triangle{{right, top, front}, {right, top, back}, {right, bottom, front}, colorRight});
    triangles.push_back(Triangle{{right, top, back}, {right, bottom, back}, {right, bottom, front}, colorRight});

    //Top face
    triangles.push_back(Triangle{{left, top, back}, {right, top, back}, {left, top, front}, colorTop});
    triangles.push_back(Triangle{{right, top, back}, {right, top, front}, {left, top, front}, colorTop});

    //Bottom face
    triangles.push_back(Triangle{{left, bottom, front}, {right, bottom, front}, {left, bottom, back}, colorBottom});
    triangles.push_back(Triangle{{right, bottom, front}, {right, bottom, back}, {left, bottom, back}, colorBottom});

    //Front face
    triangles.push_back(Triangle{{left, top, front}, {right, top, front}, {left, bottom, front}, colorFront});
    triangles.push_back(Triangle{{right, top, front}, {right, bottom, front}, {left, bottom, front}, colorFront});
}

/*
x′ = xcosθ − ysinθ
y′ = xsinθ + ycosθ
*/

void Cube::rotateX(float dir, float rotation, float ox, float oy, float oz) {
    float angle = (3.14 / 180) * rotation * dir;
    angleX += rotation * dir;
    angleX = fmod(angleX, 360.f);
    if(angleX < 0) angleX += 360.f;
    float y, z;
    for(int i = 0; i < 12; i++) {
        auto t = triangles[i];
        y = t.v1.y - oy;
        z = t.v1.z - oz;
        triangles[i].v1.y = y * cos(angle) - z * sin(angle) + oy;
        triangles[i].v1.z = y * sin(angle) + z * cos(angle) + oz;

        y = t.v2.y - oy;
        z = t.v2.z - oz;
        triangles[i].v2.y = y * cos(angle) - z * sin(angle) + oy;
        triangles[i].v2.z = y * sin(angle) + z * cos(angle) + oz;

        y = t.v3.y - oy;
        z = t.v3.z - oz;
        triangles[i].v3.y = y * cos(angle) - z * sin(angle) + oy;
        triangles[i].v3.z = y * sin(angle) + z * cos(angle) + oz;
    }

    y = cy - oy;
    z = cz - oz;
    cy = y * cos(angle) - z * sin(angle) + oy;
    cz = y * sin(angle) + z * cos(angle) + oz;
}

void Cube::rotateY(float dir, float rotation, float ox, float oy, float oz) {
    float angle = (3.14 / 180) * rotation * dir;
    angleY += rotation * dir;
    angleY = fmod(angleY, 360.f);
    if(angleY < 0) angleY += 360.f;
    float x, z;
    for(int i = 0; i < 12; i++) {
        auto t = triangles[i];
        x = t.v1.x - ox;
        z = t.v1.z - oz;
        triangles[i].v1.x = x * cos(angle) + z * sin(angle) + ox;
        triangles[i].v1.z = -x * sin(angle) + z * cos(angle) + oz;

        x = t.v2.x - ox;
        z = t.v2.z - oz;
        triangles[i].v2.x = x * cos(angle) + z * sin(angle) + ox;
        triangles[i].v2.z = -x * sin(angle) + z * cos(angle) + oz;

        x = t.v3.x - ox;
        z = t.v3.z - oz;
        triangles[i].v3.x = x * cos(angle) + z * sin(angle) + ox;
        triangles[i].v3.z = -x * sin(angle) + z * cos(angle) + oz;
    }

    x = cx - ox;
    z = cz - oz;
    cx = x * cos(angle) + z * sin(angle) + ox;
    cz = -x * sin(angle) + z * cos(angle) + oz;
}

void Cube::rotateZ(float dir, float rotation, float ox, float oy, float oz) {
    float angle = (3.14 / 180) * rotation * dir;
    angleZ += rotation * dir;
    angleZ = fmod(angleZ, 360.f);
    if(angleZ < 0) angleZ += 360.f;
    float x, y;
    for(int i = 0; i < 12; i++) {
        auto t = triangles[i];
        x = t.v1.x - ox;
        y = t.v1.y - oy;
        triangles[i].v1.x = x * cos(angle) - y * sin(angle) + ox;
        triangles[i].v1.y = x * sin(angle) + y * cos(angle) + oy;

        x = t.v2.x - ox;
        y = t.v2.y - oy;
        triangles[i].v2.x = x * cos(angle) - y * sin(angle) + ox;
        triangles[i].v2.y = x * sin(angle) + y * cos(angle) + oy;

        x = t.v3.x - ox;
        y = t.v3.y - oy;
        triangles[i].v3.x = x * cos(angle) - y * sin(angle) + ox;
        triangles[i].v3.y = x * sin(angle) + y * cos(angle) + oy;
    }

    x = cx - ox;
    y = cy - oy;
    cx = x * cos(angle) - y * sin(angle) + ox;
    cy = x * sin(angle) + y * cos(angle) + oy;
}

void Cube::rotateAxis(float dir, float rotation, float ox, float oy, float oz, float axisX, float axisY, float axisZ) {
    float angle = (3.14159f / 180.f) * rotation * dir;

    float len = sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);
    if(len < 1e-6f) return;
    float ux = axisX / len;
    float uy = axisY / len;
    float uz = axisZ / len;

    float cosA = cos(angle);
    float sinA = sin(angle);

    auto rotatePoint = [&](float& px, float& py, float& pz) {
        float x = px - ox;
        float y = py - oy;
        float z = pz - oz;

        float dot = ux * x + uy * y + uz * z;

        float crossX = uy * z - uz * y;
        float crossY = uz * x - ux * z;
        float crossZ = ux * y - uy * x;

        px = x * cosA + crossX * sinA + ux * dot * (1.f - cosA) + ox;
        py = y * cosA + crossY * sinA + uy * dot * (1.f - cosA) + oy;
        pz = z * cosA + crossZ * sinA + uz * dot * (1.f - cosA) + oz;
    };

    for(int i = 0; i < 12; i++) {
        rotatePoint(triangles[i].v1.x, triangles[i].v1.y, triangles[i].v1.z);
        rotatePoint(triangles[i].v2.x, triangles[i].v2.y, triangles[i].v2.z);
        rotatePoint(triangles[i].v3.x, triangles[i].v3.y, triangles[i].v3.z);
    }

    rotatePoint(cx, cy, cz);
}