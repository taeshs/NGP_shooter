#pragma once
class Player
{
public:
	Player();
	~Player();

	void move();

private:
	int posX;
	int posY;

	int hp;
	int mp;
};

