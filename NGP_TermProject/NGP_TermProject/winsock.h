#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include "framework.h"
#include "Global.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    5000
#define MAX_Client 2


typedef struct Player_Socket
{
	int posX, posY;
	int hp;
	bool isAttack;
	bool live;
}Player_Socket;


//Server_Player server_Player;

//서버관련
void err_quit(const char* msg);
void err_display(const char* msg);
SOCKET init_socket();
void send_Player(SOCKET sock, Player_Socket player);
Player_Socket recv_Player(SOCKET sock);

//extern char IPaddres[MAX_PATH];