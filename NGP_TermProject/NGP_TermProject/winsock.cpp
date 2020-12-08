#include "winsock.h"
#include "Resource.h"
#include <CommCtrl.h>


DWORD address;
// ���� �Լ� ���� ��� �� ����
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
// ���� �Լ� ���� ���
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

SOCKET init_socket(HINSTANCE hinst) {
	int retval;

	DialogBox(hinst, MAKEINTRESOURCE(IDD_DIALOG1),NULL,Dlg_Proc);
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	}
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	//Nagle off
	BOOL optval = TRUE;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);  //htonl(address); //inet_addr(SERVERIP); //htonl(address);//inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	//printf("[�˸�] %s:%d ���������� ���� �Ǿ����ϴ�..!\n", IPaddres, SERVERPORT);
	return sock;
}


void send_Player(SOCKET sock, Player_Socket player) {
	int retval;

	// ������ ������( ����ü ũ�⸦ ���� ������. )

	// ������ ������( ����ü �����͸� ������. )
	retval = send(sock, (char*)&player, sizeof(Player_Socket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

}
char Buffer[BUFSIZE];
Player_Socket recv_Player(SOCKET sock) {
	int retval;
	int buf;
	int GetSize;

	Player_Socket* player;
	GetSize = recv(sock, Buffer, sizeof(Player_Socket), 0);
	if (GetSize == SOCKET_ERROR) {
		err_quit("RECV()");
		//MessageBox(NULL,"error", "�����̤��� ���������ϴ�", 0);
		exit(1);
	}

	Buffer[GetSize] = '\0'; // ������ ���� �����
	player = (Player_Socket*)Buffer;

	return *player;
}


void send_Bullet(SOCKET sock, Bullet_Arr bullet) {
	int retval;

	// ������ ������( ����ü �����͸� ������. )
	retval = send(sock, (char*)&bullet, sizeof(Bullet_Arr), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}
}
//char Buffer2[BUFSIZE];

Bullet_Arr recv_Bullet(SOCKET sock) {
	int retval;
	int GetSize;

	Bullet_Arr bullet;
	GetSize = recv(sock, (char*)&bullet, sizeof(Bullet_Arr), 0);
	if (GetSize == SOCKET_ERROR) {
		MessageBox(NULL, "error", "������ ���������ϴ�", 0);
		exit(1);
	}

	return bullet;
}


void send_Bullet_Alive(SOCKET sock, Bullet_Alive_Arr bullet_al) {
	int retval;

	retval = send(sock, (char*)&bullet_al, sizeof(Bullet_Alive_Arr), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}
}

Bullet_Alive_Arr recv_Bullet_Alive(SOCKET sock) {
	int retval;
	int GetSize;

	Bullet_Alive_Arr bullet_ar;
	GetSize = recv(sock, (char*)&bullet_ar, sizeof(Bullet_Alive_Arr), 0);
	if (GetSize == SOCKET_ERROR) {
		MessageBox(NULL, "error", "������ ���������ϴ�", 0);
		exit(1);
	}

	return bullet_ar;
}


BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (wParam) {
		case IDOK:
			SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_GETADDRESS, 0, (LPARAM)&address);
			EndDialog(hDlg, IDOK);
			return TRUE;;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;;
		}
		return FALSE;
	}
	return FALSE;
}