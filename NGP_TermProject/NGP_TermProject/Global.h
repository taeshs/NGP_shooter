#pragma once
//#include "profileapi.h"

static int nTime=1;

#define TYPE_BULLET 10000
#define TYPE_PLAYER 10001

struct vector2D {
    int x, y;
};

class BoundingBox {
public:
    BoundingBox() {}
    BoundingBox(float a, float b, float c, float d, int ty, int Player) { left = a; top = b; right = c; bottom = d; type = ty; bPlayer = Player; }


private:
    float left, top, right, bottom;

    int type;
    int bPlayer;
};

class Bullet {
    bool alive;
    int bType = 0;
    int bSize;
    int bSpeed;
    int bDamage;

    int bPosX = 0, bPosY = 0;

    int bPlayer = 0;
    BoundingBox bb;
public:
    Bullet(int type, int size, int spd, int dmg) { bType = type; bSize = size; bSpeed = spd; bDamage = dmg; }
    Bullet() {}
};