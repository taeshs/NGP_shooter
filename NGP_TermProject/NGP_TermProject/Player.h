#pragma once
#include "Global.h"

class Player
{
public:
	Player();
	Player(float, float);
	~Player();

	void move(int x, int y, float deltaTime);
	void shoot(int dstX, int dstY, float deltaTime);

	void setPos(float x, float y);

	float getX() { return posX; }
	float getY() { return posY; }

	int getMp() { return mp; }

	void addMp() { if (mp < maxMp) { mp++; } }


private:
	float posX;
	float posY;

	int maxHp;
	int maxMp;

	int hp;
	int mp;

	float velocity = 100.f;

	Bullet bullet1;
	Bullet bullet2;

	BoundingBox bb;
};