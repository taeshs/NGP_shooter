#define _WINSOCK_DEPRECATED_NO_WARNINGS 
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

void send_Bullet(SOCKET sock, Bullet_Arr bullet);

Bullet_Arr recv_Bullet(SOCKET sock);

void send_Bullet_Alive(SOCKET sock, Bullet_Alive_Arr bullet);

Bullet_Alive_Arr recv_Bullet_Alive(SOCKET sock);

DWORD WINAPI ProcessClient(LPVOID);


Bullet_Arr arr_to_struct(Bullet* arr);
Bullet* struct_to_arr(Bullet_Arr str);

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
            cout << "전송 완료\n";
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

Bullet_Arr Bullets[2];

Bullet_Alive_Arr aArr[2];

char Buffer[2][BUFSIZE];


DWORD WINAPI ProcessClient(LPVOID arg) {

    char* buf = new char[BUFSIZE + 1];

    int m_no=no++;
    int enemy_no = m_no == 0 ? 1 : 0;

    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;



    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    printf("%d번째 클라이언트 입니다", m_no + 1);

    if (no >= 3) {
        closesocket(client_sock);
        printf("클라이언트 종료: IP 주소=%s, 포트 번호=%d [인원 초과]\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        return 0;
    }

    retval = send(client_sock, (char*)&m_no, sizeof(m_no), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        closesocket(client_sock);
    }


    retval = send(client_sock, (char*)&no, sizeof(no), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        closesocket(client_sock);
    }
    printf("-> 클라 아이디 (번호): %d\n", no);

    while (no < 2) {
        retval = send(client_sock, (char*)&no, sizeof(no), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(client_sock);
        }
        printf("-> 클라 아이디 (번호): %d\n", no);

    }
    if (no == 2) {
        retval = send(client_sock, (char*)&no, sizeof(no), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(client_sock);
        }
        printf("-> 클라 아이디 (번호): %d\n", no);
    }
    

    while (1) {
        int retval;
        int buf;
        int GetSize;

        
        
        
        Player_Socket* player;
        GetSize = recv(client_sock, Buffer[m_no], sizeof(Player_Socket), 0);
        if (GetSize == SOCKET_ERROR) {
            MessageBox(NULL, "error", "연결이 끊어졌습니다", 0);
            exit(1);
        }

        Buffer[m_no][GetSize] = '\0'; 
        player = (Player_Socket*)Buffer[m_no];




        Player[m_no].posX = player->posX;
        Player[m_no].posY = player->posY;
        Player[m_no].hp = player->hp;
        Player[m_no].bb = player->bb;



        Bullets[m_no] = recv_Bullet(client_sock);
        
        for (int i = 0; i < 10; i++) {
            aArr[enemy_no].arr[i] = 0;
        }

        int asd;
        for (int i = 0; i < 10; i++) {
            if (Bullets[enemy_no].arr[i].alive) {
                if (collisionCheck(Player[m_no].bb, Bullets[enemy_no].arr[i].GetBB())) {
                    Bullets[enemy_no].arr[i].alive = false;     
                    aArr[enemy_no].arr[i] = 1;
                }
            }
        }

        send_Player(client_sock, Player[enemy_no]);
        send_Bullet(client_sock, Bullets[enemy_no]);

        send_Player(client_sock, Player[m_no]);

        send_Bullet_Alive(client_sock, aArr[enemy_no]);
        send_Bullet_Alive(client_sock, aArr[m_no]);


        int gameState;
        if (Player[0].hp <= 0) {
            gameState = 1;
        }
        else if (Player[1].hp <= 0) {
            gameState = 2;
        }
        else if (Player[0].hp <= 0 && Player[1].hp <= 0) {
            gameState = 3;
        }
        else {
            gameState = 0;
        }

        retval = send(client_sock, (char*)&gameState, sizeof(gameState), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(client_sock);
        }

    }


    closesocket(client_sock);
    printf("스레드 종료 : %d\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n\n", m_no,
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    delete[] buf;
    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    //Nagle 알고리즘 off
    BOOL optval = TRUE;
    setsockopt(listen_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

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

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();

    return 0;
}

void send_Player(SOCKET sock, Player_Socket player) {
    int retval;


    // 데이터 보내기( 구조체 데이터를 보낸다. )
    retval = send(sock, (char*)&player, sizeof(Player_Socket), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        exit(1);
    }
}

void send_Bullet(SOCKET sock, Bullet_Arr bullet) {
    int retval;

    // 데이터 보내기( 구조체 데이터를 보낸다. )
    retval = send(sock, (char*)&bullet, sizeof(Bullet_Arr), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        exit(1);
    }
}


Bullet_Arr recv_Bullet(SOCKET sock) {
    int retval;
    int GetSize;
    
    Bullet_Arr bullet;
    GetSize = recv(sock, (char*)&bullet, sizeof(Bullet_Arr), 0);
    if (GetSize == SOCKET_ERROR) {
        MessageBox(NULL, "error", "연결이 끊어졌습니다", 0);
        exit(1);
    }

    return bullet;
}

void send_Bullet_Alive(SOCKET sock, Bullet_Alive_Arr bullet_al) {
    int retval;

    // 데이터 보내기( 구조체 데이터를 보낸다. )
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
        MessageBox(NULL, "error", "연결이 끊어졌습니다", 0);
        exit(1);
    }

    return bullet_ar;
}

/*
Client_Player recv_Player(SOCKET sock) {
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
    Client_Player* player;
    GetSize = recv(sock, Buffer, buf, 0);
    if (GetSize == SOCKET_ERROR) {
        MessageBox(NULL, "error", "연결이 끊어졌습니다", 0);
        exit(1);
    }

    Buffer[GetSize] = '\0'; // 마지막 버퍼 비워줌
    player = (Client_Player*)Buffer;

    return *player;
}
*/





bool collisionCheck(BoundingBox a, BoundingBox b) {
    if (a.getBB().left > b.getBB().right) return false;
    if (a.getBB().right < b.getBB().left) return false;
    if (b.getBB().bottom < a.getBB().top) return false;
    if (a.getBB().bottom < b.getBB().top) return false;
    //printf("collide!\n");
    return true;
}

Bullet_Arr arr_to_struct(Bullet* arr) {
    Bullet_Arr str;
    str.arr[0] = arr[0];
    str.arr[1] = arr[1];
    str.arr[2] = arr[2];
    str.arr[3] = arr[3];
    str.arr[4] = arr[4];
    str.arr[5] = arr[5];
    str.arr[6] = arr[6];
    str.arr[7] = arr[7];
    str.arr[8] = arr[8];
    str.arr[9] = arr[9];
    return str;
}

Bullet* struct_to_arr(Bullet_Arr str) {
    Bullet arr[10];
    arr[0] = str.arr[0];
    arr[1] = str.arr[1];
    arr[2] = str.arr[2];
    arr[3] = str.arr[3];
    arr[4] = str.arr[4];
    arr[5] = str.arr[5];
    arr[6] = str.arr[6];
    arr[7] = str.arr[7];
    arr[8] = str.arr[8];
    arr[9] = str.arr[9];

    return arr;
}