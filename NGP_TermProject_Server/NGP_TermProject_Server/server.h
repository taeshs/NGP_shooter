#pragma once

#define MAX_Client 2


typedef struct Player_Socket {
	// ������ ���� ���
	int posX=0, posY=0;
	int hp;
	bool isAttack;
	bool live;
}Player_Socket;
