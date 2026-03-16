#pragma once

#include <SFML/Graphics.hpp>
#include "Utils.hpp"

using Utils::Vec3;

class Player {

private:

//these are basically the coordinates of the player (first person)
Vec3 cam;
float angle;
float movementSpeed;
float rotationSpeed;

public:

Player(Vec3& pos, float angle, float movementSpeed, float rotationSpeed);
void setPosition(Vec3& pos);
Vec3 getCam();
float getAngle();
void moveLeft(float dt);
void moveRight(float dt);
void moveForward(float dt);
void moveBackward(float dt);
void rotateLeft(float dt);
void rotateRight(float dt);

};