#pragma once

#define MAX_Client 2


typedef struct Player_Socket {
	// 전송을 위해 사용
	int posX=0, posY=0;
	int hp;
	bool isAttack;
	bool live;
}Player_Socket;

typedef struct Client_Player {
	//클라 내에서만 사용
	// 클라에서 받는 플레이어
	Player_Socket player_socket;
}Player;

typedef struct Server_Player {
	//플레이어 데이터를 묶어놓은것
	//다시 보냄
	Player_Socket player[MAX_Client];
}Server_Player;