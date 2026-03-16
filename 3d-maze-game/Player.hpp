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
float sprintFactor;
bool sprinting;

public:

Player(const Vec3& pos, float angle, float movementSpeed, float rotationSpeed);
void setPosition(const Vec3& pos);
Vec3 getCam();
float getAngle();
void setSprintFactor(float val);
void setSprint(bool val);
bool isSprinting();
void moveLeft(float dt);
void moveRight(float dt);
void moveForward(float dt);
void moveBackward(float dt);
void rotateLeft(float dt);
void rotateRight(float dt);

};