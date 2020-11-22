#include "Player.h"
#include "Global.h"

Player::Player() {}

Player::Player(float x, float y) { posX = x; posY = y; maxMp = 10; mp = 0; maxHp = 10; hp = maxHp; }

Player::~Player() {}

void Player::move(int dirx, int diry, float deltaTime) {
	posX += (dirx * velocity * deltaTime);
	posY += (diry * velocity * deltaTime);
}

void Player::shoot(int dstX, int dstY, float deltaTime) {
	vector2D shootDir;			// 벡터 노멀라이즈 해서 델타타임으로 발사.
	shootDir.x = dstX - posX;
	shootDir.y = dstY - posY;
}

void Player::setPos(float x, float y) {
	posX = x;
	posY = y;
}