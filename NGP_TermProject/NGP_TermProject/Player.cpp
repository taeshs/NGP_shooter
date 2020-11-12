#include "Player.h"

Player::Player() {}

Player::Player(float x, float y) { posX = x; posY = y; }

Player::~Player() {}

void Player::move(float x, float y, float deltaTime) {
	posX += (x * deltaTime);
	posY += (y * deltaTime);
}

void Player::setPos(float x, float y) {
	posX = x;
	posY = y;
}