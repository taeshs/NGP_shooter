#pragma once
#include "Global.h"
#include <math.h>

class Player
{
public:
	Player();
	Player(float, float);
	~Player();

	void move(int x, int y, float deltaTime);
	bool shoot(int stX, int stY, int dstX, int dstY, float deltaTime);

	void setPos(float x, float y);

	float getX() { return posX; }
	float getY() { return posY; }

	int getMp() { return mp; }

	void addMp() { if (mp < maxMp) { mp++; } }
	void subMp(int val);

	Bullet bullet1 = {1, 10, 200, 10};	// size spd dmg
	Bullet bullet2 = {2, 20, 200, 20};
	Bullet bullet3 = {3, 10, 400, 20};

	int maxBulletCnt = 10;
	Bullet bullets[10];
	int bulletCount = 0;

private:
	float playerNo = 0;
	float posX;
	float posY;

	int maxHp;
	int maxMp;

	int hp;
	int mp;

	float velocity = 100.f;

	Bullet nowBullet = bullet1;


	//BoundingBox bb;
};