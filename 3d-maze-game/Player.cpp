#include "Player.hpp"
#include <cmath>

using Utils::Vec3;

#define PI 3.14159265f

Player::Player(Vec3& pos, float angle, float movementSpeed, float rotationSpeed) {
    this->cam = pos;
    this->angle = angle;
    this->movementSpeed = movementSpeed;
    this->rotationSpeed = rotationSpeed;
}

void Player::setPosition(Vec3& pos) {
    this->cam = pos;
}

float Player::getAngle() {
    return angle;
}

Vec3 Player::getCam() {
    return cam;
}

void Player::moveForward(float dt) {
    cam.x += std::sin(angle * PI / 180.f) * movementSpeed * dt;
    cam.z += std::cos(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::moveBackward(float dt) {
    cam.x -= std::sin(angle * PI / 180.f) * movementSpeed * dt;
    cam.z -= std::cos(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::moveLeft(float dt) {
    cam.x -= std::cos(angle * PI / 180.f) * movementSpeed * dt;
    cam.z += std::sin(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::moveRight(float dt) {
    cam.x += std::cos(angle * PI / 180.f) * movementSpeed * dt;
    cam.z -= std::sin(angle * PI / 180.f) * movementSpeed * dt;
}

void Player::rotateLeft(float dt) {
    angle -= rotationSpeed * dt;
}

void Player::rotateRight(float dt) {
    angle += rotationSpeed * dt;
}