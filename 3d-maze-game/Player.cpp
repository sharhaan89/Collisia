#include "Player.hpp"
#include <cmath>

#define PI 3.14159265f

Player::Player(float x, float y, float z, float angle, float movementSpeed, float rotationSpeed) {
    this->camX = x;
    this->camY = y;
    this->camZ = z;
    this->angle = angle;
    this->movementSpeed = movementSpeed;
    this->rotationSpeed = rotationSpeed;
}

void Player::setPosition(float x, float y, float z) {
    camX = x;
    camY = y;
    camZ = z;
}

float Player::getAngle() {
    return angle;
}

float Player::getCamX() {
    return camX;
}

float Player::getCamY() {
    return camY;
}

float Player::getCamZ() {
    return camZ;
}

void Player::moveForward(float dt) {
    camX += std::sin(angle * PI / 180.f) * movementSpeed * dt;
    camZ += std::cos(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::moveBackward(float dt) {
    camX -= std::sin(angle * PI / 180.f) * movementSpeed * dt;
    camZ -= std::cos(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::moveLeft(float dt) {
    camX -= std::cos(angle * PI / 180.f) * movementSpeed * dt;
    camZ += std::sin(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::moveRight(float dt) {
    camX += std::cos(angle * PI / 180.f) * movementSpeed * dt;
    camZ -= std::sin(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::rotateLeft(float dt) {
    angle -= rotationSpeed * dt;
}

void Player::rotateRight(float dt) {
    angle += rotationSpeed * dt;
}