#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "server.h"

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512000


#define MIN(a,b) a>b?b:a

void send_Player(SOCKET, Player_Socket);

void send_Bullet(SOCKET sock, Bullet* bullet);

Bullet* recv_Bullet(SOCKET sock);

DWORD WINAPI ProcessClient(LPVOID);
//Client_Player recv_Player(SOCKET sock);

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
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
void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;
    int sum = 0;
    float size = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0) {
            cout << "���� �Ϸ�\n";
            break;
        }
        left -= received;
        ptr += received;
        sum += received;
    }

    return (len - left);
}


int no = 0;
int val[10] = { 0, };   //

Player_Socket Player[2];
Bullet* Bullets[2];

char Buffer[BUFSIZE];

DWORD WINAPI ProcessClient(LPVOID arg) {

    char* buf = new char[BUFSIZE + 1];

    int m_no=no++;
    //int threadId = GetCurrentThreadId();
    //printf("������ ���� : %d\n", threadId);

    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;


    // Ŭ���̾�Ʈ ����
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    printf("%d��° Ŭ���̾�Ʈ �Դϴ�", m_no + 1);

    // �ο�üũ
    if (no >= 3) {
        closesocket(client_sock);
        printf("Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d [�ο� �ʰ�]\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        return 0;
    }

    retval = send(client_sock, (char*)&m_no, sizeof(m_no), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        closesocket(client_sock);
    }
    //printf("-> Ŭ�� ���̵� (��ȣ): %d\n", m_no);

    retval = send(client_sock, (char*)&no, sizeof(no), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        closesocket(client_sock);
    }
    printf("-> Ŭ�� ���̵� (��ȣ): %d\n", no);

    while (no < 2) {
        retval = send(client_sock, (char*)&no, sizeof(no), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(client_sock);
        }
        printf("-> Ŭ�� ���̵� (��ȣ): %d\n", no);
    }
    if (no == 2) {
        retval = send(client_sock, (char*)&no, sizeof(no), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(client_sock);
        }
        printf("-> Ŭ�� ���̵� (��ȣ): %d\n", no);
    }
    

    while (1) {
        printf("%d�� ���� ����\n", m_no);
        int retval;
        int buf;
        int GetSize;

        
 
        retval = recvn(client_sock, (char*)&buf, sizeof(int), 0); // ������ �ޱ�(���� ����)
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
        }
        else if (retval == 0) {

        } 
        
        Player_Socket* player;
        GetSize = recv(client_sock, Buffer, buf, 0);
        if (GetSize == SOCKET_ERROR) {
            MessageBox(NULL, "error", "������ ���������ϴ�", 0);
            exit(1);
        }

        Buffer[GetSize] = '\0'; // ������ ���� �����
        player = (Player_Socket*)Buffer;
        
        Player[m_no];

        /// <summary>
        ///  Player[0]; Player[1];
        /// Player[0] - Player[1]send ;
        /// 
        /// </summary>

        Player[m_no].posX = player->posX;
        Player[m_no].posY = player->posY;
        Player[m_no].hp = player->hp;
        Player[m_no].isAttack = player->isAttack;
        Player[m_no].live = player->live;
      


        //int retval;
        // ������ ������( ����ü ũ�⸦ ���� ������. )

        if (m_no == 0) {
            send_Player(client_sock, Player[1]);
        }
        else {
            send_Player(client_sock, Player[0]);
        }
        /*Bullets[m_no] = recv_Bullet(client_sock);

        if (m_no == 0) {
            send_Bullet(client_sock, Bullets[1]);
        }
        else {
            send_Bullet(client_sock, Bullets[0]);
        }*/
        

    }

    // closesocket()
    closesocket(client_sock);
    printf("������ ���� : %d\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n\n", m_no,
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    delete[] buf;
    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");


    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();

    return 0;
}

void send_Player(SOCKET sock, Player_Socket player) {
    int retval;

    // ������ ������( ����ü ũ�⸦ ���� ������. )
    int buf = sizeof(player);
    retval = send(sock, (char*)&buf, sizeof(int), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        exit(1);
    }

    // ������ ������( ����ü �����͸� ������. )
    retval = send(sock, (char*)&player, sizeof(Player_Socket), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        exit(1);
    }
}

void send_Bullet(SOCKET sock, Bullet* bullet) {
    int retval;

    // ������ ������( ����ü �����͸� ������. )
    retval = send(sock, (char*)&bullet, sizeof(Bullet*), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        exit(1);
    }
}char Buffer2[BUFSIZE];

Bullet* recv_Bullet(SOCKET sock) {
    int retval;
    int GetSize;

    
    Bullet* bullet;
    GetSize = recv(sock, Buffer2, sizeof(Bullet*), 0);
    if (GetSize == SOCKET_ERROR) {
        MessageBox(NULL, "error", "������ ���������ϴ�", 0);
        exit(1);
    }

    Buffer2[GetSize] = '\0'; // ������ ���� �����
    bullet = (Bullet*)Buffer2;

    return bullet;
}


/*
Client_Player recv_Player(SOCKET sock) {
    int retval;
    int buf;
    int GetSize;

    retval = recvn(sock, (char*)&buf, sizeof(int), 0); // ������ �ޱ�(���� ����)
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
    }
    else if (retval == 0) {

    }

    char Buffer[BUFSIZE];
    Client_Player* player;
    GetSize = recv(sock, Buffer, buf, 0);
    if (GetSize == SOCKET_ERROR) {
        MessageBox(NULL, "error", "������ ���������ϴ�", 0);
        exit(1);
    }

    Buffer[GetSize] = '\0'; // ������ ���� �����
    player = (Client_Player*)Buffer;

    return *player;
}
*/



/*
���������� ������ ������ ���� ����, �� ������ �ε����� �迭�� �÷��̾� ����ü ����, 
����� ����ü ��ȯ�ؼ� ���� 


���� �浹üũ?

*/