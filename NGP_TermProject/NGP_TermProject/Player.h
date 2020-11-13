#pragma once
class Player
{
public:
	Player();
	Player(float, float);
	~Player();

	void move(float x, float y, float deltaTime);
	void setPos(float x, float y);
	float getX() { return posX; }
	float getY() { return posY; }


	float velocity = 50.f;
	

private:
	float posX;
	float posY;

	int maxHp;
	int maxMp;

	int hp;
	int mp;
};