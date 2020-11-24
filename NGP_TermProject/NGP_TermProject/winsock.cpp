#include "winsock.h"

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
// 소켓 함수 오류 출력
void err_display(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

SOCKET init_socket() {
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	}
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");


	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	//printf("[알림] %s:%d 정상적으로 연결 되었습니다..!\n", IPaddres, SERVERPORT);
	return sock;
}


void send_Player(SOCKET sock, Player_Socket player) {
	int retval;

	// 데이터 보내기( 구조체 크기를 먼저 보낸다. )
	int buf = sizeof(player);
	retval = send(sock, (char*)&buf, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

	// 데이터 보내기( 구조체 데이터를 보낸다. )
	retval = send(sock, (char*)&player, sizeof(Player_Socket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

}

Server_Player recv_Player(SOCKET sock) {
	int retval;
	int buf;
	int GetSize;

	retval = recvn(sock, (char*)&buf, sizeof(int), 0); // 데이터 받기(고정 길이)
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	else if (retval == 0) {
		
	}

	char Buffer[BUFSIZE];
	Server_Player* player;
	GetSize = recv(sock, Buffer, buf, 0);
	if (GetSize == SOCKET_ERROR) {
		MessageBox(NULL,"error", "연결이 끊어졌습니다", 0);
		exit(1);
	}

	Buffer[GetSize] = '\0'; // 마지막 버퍼 비워줌
	player = (Server_Player*)Buffer;

	return *player;
}



