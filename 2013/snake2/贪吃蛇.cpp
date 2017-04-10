//*********************贪吃蛇.cpp ******************************
//窗体的创建、消息的处理
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"
#include "manager.h"

//全局变量
HINSTANCE hInst;
Game* pGame;

//函数声明
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//主函数
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);

	// 程序初始化
	if (!InitInstance (hInstance, nCmdShow))return FALSE;
	//消息转发
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//注册窗口类
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MY);
	wcex.hCursor		= LoadCursor(hInstance, (LPCTSTR)IDC_CUR);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;(LPCSTR)IDC_MY;
	wcex.lpszClassName	= "SnakeWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//初始化程序
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindowEx((unsigned)NULL,"SnakeWindow", "Snake", WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 800+GetSystemMetrics(SM_CXFIXEDFRAME)*2,
	  600+GetSystemMetrics(SM_CXFIXEDFRAME)*2+GetSystemMetrics(SM_CYCAPTION), NULL, NULL, hInstance, NULL);

   if (!hWnd)return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //创建游戏
	pGame=new Game(hInst,hWnd,800,600);
	//设置定时器
	SetTimer(hWnd,0,1000/FPS,NULL);


   return TRUE;
}

//消息处理
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_TIMER:
			pGame->Loop();
			break;
		case WM_DESTROY:
			delete pGame;
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}