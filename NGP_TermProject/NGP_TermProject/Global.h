#pragma once
//#include "profileapi.h"

static int nTime=1;

#define TYPE_BULLET 10000
#define TYPE_PLAYER 10001

struct vector2D {
    int x, y;
};

struct Bullet {
    int bSize;
    int bSpeed;
    int bDamage;

    int bPosX, bPosY;

    int bPlayer;
    //BoundingBox bb; 재정의 지정자 오류남

    Bullet() {}
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

