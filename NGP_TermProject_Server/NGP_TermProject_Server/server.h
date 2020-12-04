#pragma once

#define MAX_Client 2

#define TYPE_BULLET 10000
#define TYPE_PLAYER 10001

struct vector2D {
    float x, y;
};
bool collisionCheck(BoundingBox a, BoundingBox b);

typedef struct Player_Socket {
	// 전송을 위해 사용
	int posX=0, posY=0;
	int hp;
	bool live;
    BoundingBox bb;
}Player_Socket;

class BoundingBox {
public:
    BoundingBox() {}
    BoundingBox(float a, float b, float c, float d, int ty, int Player) { left = a; top = b; right = c; bottom = d; type = ty; bPlayer = Player; }

    void SetBB(float a, float b, float c, float d, int ty, int Player) { left = a; top = b; right = c; bottom = d; type = ty; bPlayer = Player; }
    void Update(float a, float b, float c, float d) { left = a; top = b; right = c; bottom = d; }

    RECT getBB() { RECT a; a.left = left; a.top = top; a.right = right; a.bottom = bottom; return a; }
private:
    float left, top, right, bottom;

    int type;
    int bPlayer;
};

struct Bullet {
    vector2D shootDir;
    bool alive = false;
    int bType = 0;
    int bSize;
    int bSpeed;
    int bDamage;

    float bPosX = 0, bPosY = 0;

    int bPlayer = 0;
    BoundingBox bb;

    Bullet(int type, int size, int spd, int dmg)
    {
        bType = type; bSize = size; bSpeed = spd; bDamage = dmg;
        bb.SetBB(bPosX - (bSize / 2), bPosY - (bSize / 2), bPosX + (bSize / 2), bPosY + (bSize / 2), TYPE_BULLET, bPlayer);
    }
    Bullet() {}

    void updateBB() {
        bb.Update(bPosX - (bSize / 2), bPosY - (bSize / 2), bPosX + (bSize / 2), bPosY + (bSize / 2));
    }

    BoundingBox GetBB() { return bb; }

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

struct Bullet_Arr {
    Bullet arr[10];
};



///
/// gamestate 전송해주기
/// 충돌처리 해주기
/// 
/// 
/// 
/// 
/// 