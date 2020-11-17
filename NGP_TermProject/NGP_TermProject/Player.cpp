#include "Player.h"
#include "Global.h"

Player::Player() {}

Player::Player(float x, float y) { posX = x; posY = y; }

Player::~Player() {}

void Player::move(int dirx, int diry, float deltaTime) {
	posX += (dirx * velocity * deltaTime);
	posY += (diry * velocity * deltaTime);
}

void Player::shoot(int dstX, int dstY, float deltaTime) {
	vector2D shootDir;
	shootDir.x = dstX - posX;
	shootDir.y = dstY - posY;
}

void Player::setPos(float x, float y) {
	posX = x;
	posY = y;
}