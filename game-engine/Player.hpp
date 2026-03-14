#pragma once

#include <SFML/Graphics.hpp>

class Player {

private:

//these are basically the coordinates of the player (first person)
float camX;
float camY;
float camZ;
float angle;
float movementSpeed;
float rotationSpeed;

public:

Player(float x, float y, float z, float angle, float movementSpeed, float rotationSpeed);
void setPosition(float x, float y, float z);
float getAngle();
float getCamX();
float getCamY();
float getCamZ();
void moveLeft(float dt);
void moveRight(float dt);
void moveForward(float dt);
void moveBackward(float dt);
void rotateLeft(float dt);
void rotateRight(float dt);

};