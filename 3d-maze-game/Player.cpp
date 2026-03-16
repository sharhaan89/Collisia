#include "Player.hpp"
#include <cmath>

using Utils::Vec3;

#define PI 3.14159265f

Player::Player(const Vec3& pos, float angle, float movementSpeed, float rotationSpeed) {
    this->cam = pos;
    this->angle = angle;
    this->movementSpeed = movementSpeed;
    this->rotationSpeed = rotationSpeed;
    this->sprintFactor = 5.f;
    this->sprinting = false;
}

void Player::setPosition(const Vec3& pos) {
    this->cam = pos;
}

void Player::setSprintFactor(float val) {
    this->sprintFactor = val;
}

float Player::getAngle() {
    return angle;
}

Vec3 Player::getCam() {
    return cam;
}

void Player::moveForward(float dt) {
    float speed = movementSpeed * (sprinting ? sprintFactor : 1.f);
    cam.x -= std::sin(angle * PI / 180.f) * speed * dt;
    cam.z += std::cos(angle * PI / 180.f) * speed * dt;
}

void Player::moveBackward(float dt) {
    float speed = movementSpeed * (sprinting ? sprintFactor : 1.f);
    cam.x += std::sin(angle * PI / 180.f) * speed * dt;
    cam.z -= std::cos(angle * PI / 180.f) * speed * dt;
}

void Player::moveLeft(float dt) {
    float speed = movementSpeed * (sprinting ? sprintFactor : 1.f);
    cam.x -= std::cos(angle * PI / 180.f) * speed * dt;
    cam.z -= std::sin(angle * PI / 180.f) * speed * dt;
}

void Player::moveRight(float dt) {
    float speed = movementSpeed * (sprinting ? sprintFactor : 1.f);
    cam.x += std::cos(angle * PI / 180.f) * speed * dt;
    cam.z += std::sin(angle * PI / 180.f) * speed * dt;
}

void Player::setSprint(bool val) {
    sprinting = val;
}

bool Player::isSprinting() {
    return sprinting;
}

void Player::rotateLeft(float dt) {
    angle += rotationSpeed * dt;
}

void Player::rotateRight(float dt) {
    angle -= rotationSpeed * dt;
}