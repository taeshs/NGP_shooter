// NGP_TermProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "framework.h"
#include "NGP_TermProject.h"
#include "Global.h"
#include "winsock.h"
#include "Player.h"
#pragma comment(lib, "Msimg32.lib")



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


//서버관련
SOCKET sock;

Player_Socket Player_socket;
Player_Socket Other_socket;


DWORD WINAPI ClientMain(LPVOID);

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HBITMAP DrawBackground(HWND, int, int, int, int, HDC, HDC, HBITMAP);
HBITMAP DrawSkill(HWND, int, int, int, int, HDC, HDC, HBITMAP);
//void DrawCharater(HWND, POINT, HDC, HDC, int);
HBITMAP DrawCharater(HWND, Player, HDC, HDC, HBITMAP);
void Run(HWND);


Bullet_Arr arr_to_struct(Bullet* arr);


LARGE_INTEGER g_tSecond;   // 초당 클록수    ex) 360  (고정값)

LARGE_INTEGER g_tTime;      // 이전 클록수    

float		  g_fDeltaTime;

int maxhp = 10;
float minhp = 0;
float minhp2 = 0;
int clientid,person,gameState;
int person1 = 0;
bool start = false;

Bullet_Arr Other_Bullet;
Bullet_Arr Player_Bullet;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NGPTERMPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 소켓 통신 스레드 생성
    CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NGPTERMPROJECT));

    MSG msg;

    QueryPerformanceFrequency(&g_tSecond); QueryPerformanceCounter(&g_tTime);
    // 기본 메시지 루프입니다:
    while (1)
    {
        //Sleep(10);
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (person > 1) {
            Run(msg.hwnd);
        }
            

    }
    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        
        Run(msg.hwnd);
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            
        }
        
    }*/

    // closesocket()
    //closesocket(sock);

    // 윈속 종료
    //WSACleanup();

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NGPTERMPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NGPTERMPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

static POINT P1, P2;

static Player player(80.0f, 200.0f), Other_Player(550.0f, 200.0f);            // P1 P2 -> PLAYER   OTHERPLAYER 변수이름 변경
// 내번호가 0번이면 PLAYER.SETPOS((80.0f, 200.0f)), OTHERPLAYER.SETPOS(550.0f, 200.0f); player bitmap -> p1bitmap ,  otherplayer bitmap -> p2bitmap
// 내번호가 1번이면 PLAYER.SETPOS((550.0f, 200.0f)), OTHERPLAYER.SETPOS(80.0f, 200.0f); player bitmap -> p1bitmap ,  otherplayer bitmap -> p1bitmap


HBITMAP BGBitmap, P1Bitmap, P2Bitmap, S1Bitmap, S2Bitmap, S3Bitmap,LodBitmap, p1winBitmap, p2winBitmap, drawBitmap;
RECT gameGround;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    //BGBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
    //P1Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
    //P2Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));


    switch (message)
    {
    case WM_CREATE:
    {
        BGBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        P1Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        P2Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        S1Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        S2Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        S3Bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
        LodBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
        p1winBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
        p2winBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP9));
        drawBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP10));


        //p1.setPos(60.0f, 200.0f);
        //p2.setPos(550.0f, 200.0f);
        P1.x = 60, P1.y = 200, P2.x = 550, P2.y = 200;
        //static int nTime = 0;
        SetTimer(hWnd, 0, 1000, NULL);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 0x31:
            player.setBullet(1);
            player.setHp(1);    ////////////////////////////////////////////////
            break;

        case 0x32:
            player.setBullet(2);
            player.setHp(5);    ////////////////////////////////////////////////
            break;

        case 0x33:
            player.setBullet(3);
            player.setHp(10);    ////////////////////////////////////////////////
            break;
        }

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    case WM_LBUTTONDOWN:
        int posX, posY;
        posX = LOWORD(lParam);
        posY = HIWORD(lParam);

        if (player.getMp() >= player.getBulletCost()) {
            if (player.shoot(player.getX() + 20, player.getY() + 20, posX, posY, g_fDeltaTime)) {
                player.subMp(player.getBulletCost());
            }
        }
        //          


        // if posx,y가 스킬1범위에 있을때
        //            player.subMp(1) 로 마나 소모, 스킬실행

         // if posx,y가 스킬2범위에 있을때
        //            player.subMp(1) 로 마나 소모, 스킬실행

         // if posx,y가 스킬3범위에 있을때
        //            player.subMp(1) 로 마나 소모, 스킬실행

        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc, memDC, backDC;
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        HBITMAP BackBit, oldBackBit;

        RECT bufferRT;

        hdc = BeginPaint(hWnd, &ps);
        memDC = CreateCompatibleDC(hdc);
        backDC = CreateCompatibleDC(hdc);

        //if(person1 <= 1)
           
        

            //계속진행

        if (clientid == 0 && start == false)
        {
            player.setPos(80.0f, 200.0f);
            Other_Player.setPos(550.0f, 200.0f);
            player.bitmap = P1Bitmap;
            Other_Player.bitmap = P2Bitmap;
            player.setHp(10);
            Other_Player.setHp(10);

            start = true;
        }
        else if (clientid == 1 && start == false)
        {
            player.setPos(550.0f, 200.0f);
            Other_Player.setPos(80.0f, 200.0f);
            player.bitmap = P2Bitmap;
            Other_Player.bitmap = P1Bitmap;
            player.setHp(10);
            Other_Player.setHp(10);

            start = true;
        }

        GetClientRect(hWnd, &bufferRT);


        int mp_gage = (bufferRT.right - 100) / 10;

        gameGround = { 50,50,bufferRT.right - 190, bufferRT.bottom - 140 };
        

        BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
        oldBackBit = (HBITMAP)SelectObject(backDC, BackBit);
        DeleteObject(BackBit);
        FillRect(backDC, &bufferRT, (HBRUSH)GetStockObject(WHITE_BRUSH));

        if (gameState == 1) {
            DrawBackground(hWnd, bufferRT.left, bufferRT.top, bufferRT.right, bufferRT.bottom, memDC, backDC, p1winBitmap);
        }
        else if (gameState == 2) {
            DrawBackground(hWnd, bufferRT.left, bufferRT.top, bufferRT.right, bufferRT.bottom, memDC, backDC, p2winBitmap);
        }
        else if (gameState == 3) {
            DrawBackground(hWnd, bufferRT.left, bufferRT.top, bufferRT.right, bufferRT.bottom, memDC, backDC, drawBitmap);
        }
        
        // rect로 배경 범위 만들기. -> 범위 내 이동
        if(person < 2)
            DrawBackground(hWnd, bufferRT.left, bufferRT.top, bufferRT.right, bufferRT.bottom, memDC, backDC, LodBitmap);
        if(person == 2 && gameState == 0)
        {
            DrawBackground(hWnd, gameGround.left, gameGround.top, gameGround.right, gameGround.bottom, memDC, backDC, BGBitmap);

            DrawSkill(hWnd, bufferRT.right - 150, 50, bufferRT.right - 50, bufferRT.bottom - 360, memDC, backDC, S1Bitmap);
            DrawSkill(hWnd, bufferRT.right - 150, bufferRT.bottom - 360, bufferRT.right - 50, bufferRT.bottom - 230, memDC, backDC, S2Bitmap);
            DrawSkill(hWnd, bufferRT.right - 150, bufferRT.bottom - 230, bufferRT.right - 50, bufferRT.bottom - 100, memDC, backDC, S3Bitmap);

            // left, top, right, bottom,

            DrawCharater(hWnd, player, memDC, backDC, player.bitmap); // PLAYER 1
            //Rectangle(backDC, player.GetBBRect().left, player.GetBBRect().top, player.GetBBRect().right, player.GetBBRect().bottom); //boundingbox

            DrawCharater(hWnd, Other_Player, memDC, backDC, Other_Player.bitmap); // PLAYER 2
            //Rectangle(backDC, Other_Player.GetBBRect().left, Other_Player.GetBBRect().top, Other_Player.GetBBRect().right, Other_Player.GetBBRect().bottom);//boundingbox

            Rectangle(backDC, 50, 30, ((bufferRT.right - 100) / 2), 50);
            Rectangle(backDC, ((bufferRT.right - 100) / 2), 30, (bufferRT.right - 100) - 50, 50);


           /*
           if (BGBitmap == NULL) {
               LPCWSTR word = TEXT("BG LOAD FAILED");
               TextOut(backDC, 200, 300, word, lstrlen(word));
           }
           */

           //test/////////////////////////////

           //empty mp bar
            Rectangle(backDC, 50, bufferRT.bottom - 100, bufferRT.right - 54, bufferRT.bottom - 50);

            HPEN myPen, oldPen;
            HBRUSH myBrush, oldBrush;

            myPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            oldPen = (HPEN)SelectObject(backDC, myPen);

            myBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(backDC, myBrush);

            //if (공격받을때) 
            //player.subHp();

            float hpBlock = (((bufferRT.right - 200) / 2)) / 10;

            Rectangle(backDC, 50, 30, 50 + (hpBlock * player.getHp()), 50);
            
            SelectObject(backDC, oldPen);
            DeleteObject(myPen);
            SelectObject(backDC, oldBrush);
            DeleteObject(myBrush);

            myBrush = CreateSolidBrush(RGB(0, 0, 255));
            oldBrush = (HBRUSH)SelectObject(backDC, myBrush);

            Rectangle(backDC, ((bufferRT.right - 100) / 2), 30, ((bufferRT.right - 100) / 2) + (hpBlock * Other_Player.getHp()), 50);

            SelectObject(backDC, oldPen);
            DeleteObject(myPen);
            SelectObject(backDC, oldBrush);
            DeleteObject(myBrush);

            ///hp bar
            //////////

            //Rectangle(backDC, bufferRT.right - 150, 50, bufferRT.right - 50, bufferRT.bottom - 100);// 440);// 
            // skill bar

            myBrush = CreateSolidBrush(RGB(0, 255, 0));
            oldBrush = (HBRUSH)SelectObject(backDC, myBrush);

            // 플레이어의 mp 수치 받아오기
            int mp = player.getMp();

            RECT mpBar;
            mpBar.left = 50;
            mpBar.top = bufferRT.bottom - 100;
            mpBar.right = (50 + mp_gage * mp < bufferRT.right - 50) ? 50 + mp_gage * mp : bufferRT.right - 50;
            mpBar.bottom = bufferRT.bottom - 50;

            Rectangle(backDC, mpBar.left, mpBar.top, mpBar.right, mpBar.bottom);
            // mp Bar

            for (int i = 0; i < player.maxBulletCnt; i++) {
                if (player.bullets[i].alive) {
                    Rectangle(backDC, player.bullets[i].bPosX - player.bullets[i].bSize / 2, player.bullets[i].bPosY + player.bullets[i].bSize / 2
                        , player.bullets[i].bPosX + player.bullets[i].bSize / 2, player.bullets[i].bPosY - player.bullets[i].bSize / 2);
                    /*Rectangle(backDC, player.bullets[i].bb.getBB().left, player.bullets[i].bb.getBB().top,
                        player.bullets[i].bb.getBB().right, player.bullets[i].bb.getBB().bottom);*/
                }
            }

            for (int i = 0; i < player.maxBulletCnt; i++) {
                if (Other_Bullet.arr[i].alive) {
                    Rectangle(backDC, Other_Bullet.arr[i].bPosX - Other_Bullet.arr[i].bSize / 2, Other_Bullet.arr[i].bPosY + Other_Bullet.arr[i].bSize / 2
                        , Other_Bullet.arr[i].bPosX + Other_Bullet.arr[i].bSize / 2, Other_Bullet.arr[i].bPosY - Other_Bullet.arr[i].bSize / 2);
                    /*Rectangle(backDC, Other_Bullet.arr[i].bb.getBB().left, Other_Bullet.arr[i].bb.getBB().top,
                        Other_Bullet.arr[i].bb.getBB().right, Other_Bullet.arr[i].bb.getBB().bottom);*/
                }

            }
            
            // 임시 총알 


            SelectObject(backDC, oldPen);
            DeleteObject(myPen);
            SelectObject(backDC, oldBrush);
            DeleteObject(myBrush);


            //쓰고난 펜을 삭제해준다.
            DeleteObject(myPen);
            DeleteObject(myBrush);

            //test//////////////////////////////// 
        }

        BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, backDC, 0, 0, SRCCOPY);

        SelectObject(hdc, oldBackBit);


        
        
        
        DeleteDC(hdc);
        DeleteDC(backDC);
        DeleteDC(memDC);
        EndPaint(hWnd, &ps);


        //DeleteObject(BGBitmap);
        //DeleteObject(P1Bitmap);
        //DeleteObject(P2Bitmap);
        
    }
    break;
    case WM_DESTROY:

        PostQuitMessage(0);
        KillTimer(hWnd, 0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

float timer = 0.f;

void Run(HWND hWnd) {
    LARGE_INTEGER tTIme;
    QueryPerformanceCounter(&tTIme);




    g_fDeltaTime = (tTIme.QuadPart - g_tTime.QuadPart) / (float)g_tSecond.QuadPart;
    timer += g_fDeltaTime;
    if (timer > 1.0f) {
        timer = 0.f;
        player.addMp();
    }

    g_tTime = tTIme;


    /*if (GetAsyncKeyState(VK_RIGHT) < 0 || GetAsyncKeyState(0x44) < 0) {
        if (gameGround.right > player.getX())
            player.move(1, 0, g_fDeltaTime);
    }
    else if (GetAsyncKeyState(VK_LEFT) < 0 || GetAsyncKeyState(0x41) < 0) {
        if (gameGround.left < player.getX())
            player.move(-1, 0, g_fDeltaTime);
    }
    if (GetAsyncKeyState(VK_UP) < 0 || GetAsyncKeyState(0x57) < 0) {
        if (gameGround.top < player.getY())
            player.move(0, -1, g_fDeltaTime);
    }
    else if (GetAsyncKeyState(VK_DOWN) < 0 || GetAsyncKeyState(0x53) < 0) {
        if (gameGround.bottom > player.getY())
            player.move(0, 1, g_fDeltaTime);
    }*/ // 기존 키 조작
    
    // 키 1p 2p 분리 - 혼자 테스트 용
    /**/
    if (clientid == 0) {
        if (GetAsyncKeyState(VK_RIGHT) < 0) {
            if (gameGround.right > player.getX())
                player.move(1, 0, g_fDeltaTime);
        }
        else if (GetAsyncKeyState(VK_LEFT) < 0) {
            if (gameGround.left < player.getX())
                player.move(-1, 0, g_fDeltaTime);
        }
        if (GetAsyncKeyState(VK_UP) < 0) {
            if (gameGround.top < player.getY())
                player.move(0, -1, g_fDeltaTime);
        }
        else if (GetAsyncKeyState(VK_DOWN) < 0) {
            if (gameGround.bottom > player.getY())
                player.move(0, 1, g_fDeltaTime);
        }
    }
    else {
        if (GetAsyncKeyState(0x44) < 0) {
            if (gameGround.right > player.getX())
                player.move(1, 0, g_fDeltaTime);
        }
        else if (GetAsyncKeyState(0x41) < 0) {
            if (gameGround.left < player.getX())
                player.move(-1, 0, g_fDeltaTime);
        }
        if (GetAsyncKeyState(0x57) < 0) {
            if (gameGround.top < player.getY())
                player.move(0, -1, g_fDeltaTime);
        }
        else if (GetAsyncKeyState(0x53) < 0) {
            if (gameGround.bottom > player.getY())
                player.move(0, 1, g_fDeltaTime);
        }
    }

    player.UpdateBB(player.getX(),player.getY(),40);

    for (int i = 0; i < player.maxBulletCnt; i++) {
        if (player.bullets[i].alive)
            player.bullets[i].update(g_fDeltaTime, gameGround);
    }
    //일단 받아서 그리는걸로 하고, 이동이 어색하면 경로 받아서 같이 업데이트 시키는 방식으로.

    //Player_socket.posX = player.getX();
    //Player_socket.posX = player.getY();

    InvalidateRect(hWnd, NULL, FALSE);
}

HBITMAP DrawBackground(HWND hWnd, int left, int top, int right, int bottom, HDC hdc, HDC dest, HBITMAP bitmap) {
    SelectObject(hdc, bitmap);

    BitBlt(dest, left, top, right, bottom, hdc, 0, 0, SRCCOPY);
    return bitmap;
}

HBITMAP DrawCharater(HWND hWnd, Player player, HDC hdc, HDC dest, HBITMAP BITMAP) {
    SelectObject(hdc, BITMAP);

    TransparentBlt(dest, (int)player.getX(), (int)player.getY(), 40, 40, hdc, 0, 0, 80, 80, RGB(255, 255, 255));
    return BITMAP;
}

HBITMAP DrawSkill(HWND hWnd, int left, int top, int right, int bottom, HDC hdc, HDC dest, HBITMAP bitmap) {
    SelectObject(hdc, bitmap);

    BitBlt(dest, left, top, right, bottom, hdc, 0, 0, SRCCOPY);
    return bitmap;
}


DWORD WINAPI ProcessClient(LPVOID arg) {

    int retval;
    int len;

    retval = recvn(sock, (char*)&clientid, sizeof(clientid), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        closesocket(sock);
    }
    printf("-> 클라 아이디 (번호): %d\n", clientid);
    //PLAYER.SETPOS((80.0f, 200.0f)), OTHERPLAYER.SETPOS(550.0f, 200.0f); player bitmap -> p1bitmap ,  otherplayer bitmap -> p2bitmap
    // 내번호가 1번이면 PLAYER.SETPOS((550.0f, 200.0f)), OTHERPLAYER.SETPOS(80.0f, 200.0f); player bitmap -> p1bitmap ,  otherplayer bitmap -> p1bitmap
    int playerNo = -1;
    if (clientid == 0) {
        playerNo = PLAYER_NUMBER_1;
    }
    else if (clientid == 1) {
        playerNo = PLAYER_NUMBER_2;
    }
    player.setPlayerNo(playerNo);

    retval = recvn(sock, (char*)&person, sizeof(person), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        closesocket(sock);
    }
    printf("-> 클라 아이디 (번호): %d\n", person);
    while (person < 2) {
        retval = recvn(sock, (char*)&person, sizeof(person), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(sock);
        }
        printf("-> 클라 아이디 (번호): %d\n", person);
    }
    /*if (person == 2) {
        retval = recv(sock, (char*)&person, sizeof(person), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(sock);
        }
        printf("-> 클라 아이디 (번호): %d\n", person);
    }*/
    


    while (1) {
        Player_socket.posX = player.getX();
        Player_socket.posY = player.getY();
        Player_socket.hp = player.getHp();
        Player_socket.bb = player.GetBB();
        

        send_Player(sock, Player_socket);

        Other_socket = recv_Player(sock);


        Other_Player.setPos(Other_socket.posX, Other_socket.posY);
        Other_Player.setHp(Other_socket.hp);
        Other_Player.UpdateBB(Other_socket.posX, Other_socket.posY, 40);

        //player.setHp(Player_socket.hp);

        send_Bullet(sock, arr_to_struct(player.bullets));
        Other_Bullet = recv_Bullet(sock);
        
        Bullet_Alive_Arr aArr;
        aArr = recv_Bullet_Alive(sock);

        /*for (int i = 0; i < 10; i++) {
            player.bullets[i].alive = aArr.arr[i];
        }*/
       
        
        

        

        //player.setHp(5);


        //Other_Player = recv_Player(sock);          // 받은 정보로 otherplayer set.

        //p2.setPos(server_Player.Players->posX, server_Player.Players->posY);

        retval = recvn(sock, (char*)&gameState, sizeof(gameState), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            closesocket(sock);
        }

    }
    return 0;
}

// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
    HANDLE hThread;
    sock = init_socket();

    hThread = CreateThread(NULL, 0, ProcessClient, 0, 0, NULL);

    return 0;
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
// 만약에 받은 플레이어 번호가 0번이면 1P -> 