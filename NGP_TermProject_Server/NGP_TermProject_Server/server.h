#pragma once

#define MAX_Client 2

struct vector2D {
    float x, y;
};

typedef struct Player_Socket {
	// 전송을 위해 사용
	int posX=0, posY=0;
	int hp;
	bool isAttack;
	bool live;
}Player_Socket;

struct Bullet {
    vector2D shootDir;
    bool alive = false;
    int bType = 0;
    int bSize;
    int bSpeed;
    int bDamage;

    float bPosX = 0, bPosY = 0;

    int bPlayer = 0;
    //BoundingBox bb;

    Bullet(int type, int size, int spd, int dmg) { bType = type; bSize = size; bSpeed = spd; bDamage = dmg; }
    Bullet() {}

    void updateBB() {

    }

    void update(float deltaTime, RECT gameGround) {
        //충돌처리 해줘야함.
        bPosX += shootDir.x * bSpeed * deltaTime;
        if (bPosX > gameGround.right + 40 || bPosX < gameGround.left) {
            alive = false;
        }
        bPosY += shootDir.y * bSpeed * deltaTime;
        if (bPosY > gameGround.bottom + 40 || bPosY < gameGround.top) {
            alive = false;
        }
        updateBB();
    }
};