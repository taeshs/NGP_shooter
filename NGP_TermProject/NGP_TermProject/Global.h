#pragma once
//#include "profileapi.h"

static int nTime=1;

#define TYPE_BULLET 10000
#define TYPE_PLAYER 10001

struct vector2D {
    float x, y;
};

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(float a, float b, float c, float d, int ty, int Player) { left = a; top = b; right = c; bottom = d; type = ty; bPlayer = Player; }


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
    //BoundingBox bb;

    Bullet(int type, int size, int spd, int dmg) { bType = type; bSize = size; bSpeed = spd; bDamage = dmg; }
    Bullet() {}

    void updateBB() {

    }

    void update(float deltaTime) {
        //面倒贸府 秦拎具窃.
        bPosX += shootDir.x * bSpeed * deltaTime;
        bPosY += shootDir.y * bSpeed * deltaTime;
        updateBB();
    }
};