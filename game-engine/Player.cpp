#include "Player.hpp"

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

void Player::moveLeft(float dt) {
    camX -= movementSpeed * dt;
}

void Player::moveRight(float dt) {
    camX += movementSpeed * dt;
}

void Player::moveForward(float dt) {
    camZ += movementSpeed * dt;
}

void Player::moveBackward(float dt) {
    camZ -= movementSpeed * dt;
}

void Player::rotateLeft(float dt) {
    angle -= rotationSpeed * dt;
}

void Player::rotateRight(float dt) {
    angle += rotationSpeed * dt;
}