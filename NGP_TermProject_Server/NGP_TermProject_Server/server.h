#pragma once

#define MAX_Client 2


typedef struct Player_Socket {
	// ������ ���� ���
	int posX=0, posY=0;
	int hp;
	bool isAttack;
	bool live;
}Player_Socket;

typedef struct Client_Player {
	//Ŭ�� �������� ���
	// Ŭ�󿡼� �޴� �÷��̾�
	Player_Socket player_socket;
}Player;

typedef struct Server_Player {
	//�÷��̾� �����͸� ���������
	//�ٽ� ����
	Player_Socket player[MAX_Client];
}Server_Player;