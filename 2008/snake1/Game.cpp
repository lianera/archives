// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Game.h"
#include <time.h>
#include <fstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HWND hWnd;

CGame::CGame()
{
	m_Level=1;
	m_life=3;
	m_isbegin=true;
}

CGame::~CGame()
{
	ReleaseDC(hWnd,theDC);
	ReleaseDC(hWnd,swapDC);

	delete pPanel;
	delete pSnake;
	delete pMill;
}

void CGame::OnStart()
{	
	srand(time(0));

	pPanel=new CPanel;
	pPanel->BuildPanel();

	pSnake=new CSnake;
	pSnake->AddNode(m_Level*2-1);
	
	pMill=new CMill;
	pMill->SetAppleNum(5+m_Level*3);
	pMill->DoApple();

	InitDraw();
	if(m_isbegin) BeginCart();
	if(m_Level<=3) PlayMid("back.MID");
	else if(m_Level<=5) PlayMid("back1.MID");
	else PlayMid("back2.MID");

	////
	////
	SetTimer(hWnd,1,200-m_Level*20,NULL);
	lastKey=LEFT;
}

void CGame::DrawScene()
{
	SelectObject(swapDC,swapBmp);
//	SetBkColor(backDC,RGB(0,0,0));
//	PatBlt(backDC,0,0,WINWIDTH,WINHEIGHT,PATCOPY);
	//Draw
	pPanel->DrawPanel(swapDC);
	ShowInfo();
	pSnake->DrawSnack(swapDC,pPanel);
	pMill->Draw(swapDC,pPanel);

	if(!BitBlt(theDC,0,0,WINWIDTH,WINHEIGHT,swapDC,0,0,SRCCOPY)) OnErr();
}

void CGame::OnKeyDown(int key)
{
	switch(key)
	{
	case VK_LEFT:
		lastKey=LEFT;
		break;

	case VK_UP:
		lastKey=UP;
		break;

	case VK_RIGHT:
		lastKey=RIGHT;
		break;

	case VK_DOWN:
		lastKey=DOWN;
		break;

	default:
		break;
	}
}

void CGame::OnKeyUp(int key)
{
/*	switch(key)
	{
	case VK_LEFT:
		LeftKey=false;
		break;

	case VK_UP:
		UpKey=false;
		break;

	case VK_RIGHT:
		RightKey=false;
		break;

	case VK_DOWN:
		DownKey=false;
		break;

	default:
		break;
	}*/
}

void CGame::OnTimer(int ID)
{
	CheckKey();
	pSnake->MoveSnack();
	if(pSnake->IsBump(pPanel)) OnLose();
	if(pSnake->IsEatApple(pMill)) 
	{
		if(pMill->EatApple()) pSnake->AddNode();
		else OnPassLevel();
	}
	DrawScene();
}

void CGame::InitDraw()
{
	theDC=GetDC(hWnd);
	swapDC=CreateCompatibleDC(theDC);
	swapBmp=CreateCompatibleBitmap(theDC,WINWIDTH,WINHEIGHT);
	SetBkColor(swapDC,RGB(200,200,200));
	SetTextColor(swapDC,RGB(50,50,255));
	pSnake->InitDraw(swapDC);
	pPanel->InitDraw(swapDC);
	pMill->InitDraw(swapDC);
}


void CGame::OnLose()
{
	KillTimer(hWnd,1);
	Destroy();
	if(!PlaySound((LPCTSTR)IDR_LOSE, NULL, SND_RESOURCE | SND_ASYNC)) OnErr();
	Sleep(1000);
	m_life--;
	m_isbegin=false;
	if(m_life>0)
	{
		MeamCart();
		OnStart();
		Sleep(1000);
	}
	else OnEnd();
}

CPanel* CGame::GetPanel()
{
	return pPanel;
}

void CGame::OnPassLevel()
{
	KillTimer(hWnd,1);
	Destroy();
	if(!PlaySound((LPCTSTR)IDR_PASSLEVEL, NULL, SND_RESOURCE | SND_ASYNC)) OnErr();
	Sleep(1000);
	m_Level++;
	m_life++;
	MeamCart();
	Sleep(1000);
	OnStart();
}

void CGame::Destroy()
{
	mciSendCommand(MCI_ALL_DEVICE_ID,MCI_CLOSE,0,NULL);
	delete pPanel;
	delete pSnake;
	delete pMill;	
}

void CGame::PlayMid(LPTSTR name)
{
	MCI_DGV_OPEN_PARMS mciOpen;
	MCIERROR mciError;
	mciOpen.lpstrDeviceType = "sequencer"; //设备名
	mciOpen.lpstrElementName = name; //设备元素
	mciError=mciSendCommand(0, MCI_OPEN, 
	MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)&mciOpen);
	if(mciError)
	{
		char s[80];
		mciGetErrorString(mciError,s,80);
		MessageBox(hWnd,s,NULL,NULL);
		return ;
	}
	wDeviceID=mciOpen.wDeviceID; //保存设备ID
	MCI_DGV_PLAY_PARMS mciPlay;
	mciError=mciSendCommand(wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)&mciPlay);
}

void CGame::ShowInfo()
{
	RECT InfoRect=pPanel->GetInfoRect();
	char level[10];
	memset(level,0,sizeof(level));
	wsprintf(level,"等级:%d",m_Level);
	TextOut(swapDC,InfoRect.left+10,InfoRect.top+50,level,strlen(level));
	
	char life[10];
	memset(life,0,sizeof(life));
	wsprintf(life,"生命:%d",m_life);
	TextOut(swapDC,InfoRect.left+10,InfoRect.top+100,life,strlen(life));

	char apple[10];
	memset(apple,0,sizeof(apple));
	wsprintf(apple,"苹果:%d",pMill->GetApple()+1);
	TextOut(swapDC,InfoRect.left+10,InfoRect.top+150,apple,strlen(apple));
}

void CGame::OnEnd()
{
	KillTimer(hWnd,1);
	HDC dc=CreateCompatibleDC(theDC);
	HBITMAP endBmp=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_THEEND));
	SelectObject(dc,endBmp);
	BitBlt(theDC,0,0,500,450,dc,0,0,SRCCOPY);
	ReleaseDC(hWnd,dc);

	if(!PlaySound((LPCTSTR)IDR_THEEND, NULL, SND_RESOURCE | SND_ASYNC)) OnErr();
	Sleep(13000);

	m_Level=1;
	m_life=3;
	m_isbegin=true;
	OnStart();
}

void CGame::BeginCart()
{
	HDC dc=CreateCompatibleDC(theDC);
	HBITMAP logoBmp=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LOGO));
	SelectObject(dc,logoBmp);
	BitBlt(theDC,0,0,500,450,dc,0,0,SRCCOPY);
	ReleaseDC(hWnd,dc);
	Sleep(1000);
	m_isbegin=false;
}

void CGame::MeamCart()
{
	COLORREF oldtc=SetTextColor(theDC,RGB(255,255,255));
	COLORREF oldbk=SetBkColor(theDC,RGB(0,0,0));
	HBRUSH brush=CreateSolidBrush(RGB(0,0,0));
	SelectObject(theDC,brush);
	Rectangle(theDC,0,0,500,450);

	char level[10];
	memset(level,0,sizeof(level));
	wsprintf(level,"LEVEL:%d",m_Level);
	char life[10];
	memset(life,0,sizeof(life));
	wsprintf(life,"LIFE :%d",m_life);

	TextOut(theDC,200,100,level,strlen(level));
	TextOut(theDC,200,200,life,strlen(life));

	SetTextColor(theDC,oldtc);
	SetBkColor(theDC,oldbk);
}

void CGame::CheckKey()
{
	pSnake->SetTrend(lastKey);
}

void CGame::Pause()
{
	KillTimer(hWnd,1);
}

void CGame::NotPause()
{
	SetTimer(hWnd,1,200-m_Level*20,NULL);
}

void CGame::Save(LPTSTR name)
{
	ofstream out;
	strcat(name,".txt");
	out.open(name,ios::trunc|ios::out);
	if(!out) OnErr();
	int level;
	int life;
	level=m_Level;
	life=m_life;
	out<<level<<"\n";
	out<<life;
	out.close();
}

void CGame::Load(LPTSTR name)
{
	ifstream in;
	strcat(name,".txt");
	in.open(name,ios::in);
	if(!in) OnErr();
	int level;
	int life;
	in>>level;
	in>>life;
	if(level<=0 || life<=0 || level>15 || life>19) OnErr("载入错误！");
	m_Level=level;
	m_life=life;

	KillTimer(hWnd,1);
	Destroy();
	m_isbegin=true;
	OnStart();
	in.close();
}
