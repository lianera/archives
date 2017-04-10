// Manager.cpp: implementation of the CManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "俄罗斯方块.h"
#include "Manager.h"
#include "Block.h"
#include "Diamond.h"
#include "Mill.h"
#include "Panel.h"
#include <mmsystem.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMyApp theApp;
CBlock*		pBlock;
CDiamond*	pDiamond;
CMill*		pMill;
CPanel*		pPanel;

CManager::CManager()
{
	KeyLeft=false;
	KeyDown=false;
	KeyRight=false;
	KeySpace=false;
}

CManager::~CManager()
{
	SetFree();
}

void CManager::PassLevel()
{
	SetFree();
	if(Sound) PlaySound("sound\\passlevel.wav",NULL,SND_FILENAME | SND_ASYNC);
	Sleep(3000);
	InitGame(Level+1);

}

void CManager::SetFree()
{
	Timer1	=99999;
	Timer2	=99999;
	FallTimer=99999;
	KillTimer(theApp.m_pMainWnd->m_hWnd,1);
	KillTimer(theApp.m_pMainWnd->m_hWnd,2);
	delete	pBlock;
	delete	pDiamond;
	delete	pMill;
	delete	pPanel;
}

void CManager::InitGame(int level)
{
	//初始化
	/////////////////////////////////
	srand( (unsigned)time( NULL ) );	//随机种子
	GetClientRect(theApp.m_pMainWnd->m_hWnd,&ClientRect);
	Sound=true;
	//变量
	Result=0;
	LineNumber=0;
	Level=level;
	//游戏面板
	pPanel=new CPanel;
	pPanel->PanelRect.top	=int(ClientRect.bottom*0.1f);
	pPanel->PanelRect.bottom=int(ClientRect.bottom*0.9f);
	pPanel->PanelRect.left	=int(ClientRect.right *0.1f);
	pPanel->PanelRect.right =int(ClientRect.right *0.6f);
	pPanel->nHGridNum		=15;
	pPanel->nVGridNum		=25;
	pPanel->GridSize.cx		=(pPanel->PanelRect.right-pPanel->PanelRect.left)/pPanel->nHGridNum;
	pPanel->GridSize.cy		=(pPanel->PanelRect.bottom-pPanel->PanelRect.top)/pPanel->nVGridNum;
	pPanel->PanelColor		=RGB(200,200,200);
	pPanel->TextRect.SetRect(int(ClientRect.right *0.7f),int(ClientRect.bottom*0.5f),int(ClientRect.right *0.95f),int(ClientRect.bottom *0.9f));

	//方块工厂
	pMill=new CMill;
	pMill->MillRect.top		=int(ClientRect.bottom*0.1f);
	pMill->MillRect.bottom	=int(ClientRect.bottom*0.2f);
	pMill->MillRect.left	=int(ClientRect.right *0.7f);
	pMill->MillRect.right	=int(ClientRect.right *0.8f);
	pMill->GridSize.cx		=(pMill->MillRect.right-pMill->MillRect.left)/4;
	pMill->GridSize.cy		=(pMill->MillRect.bottom-pMill->MillRect.top)/4;

	pMill->probabilityCake	=25;
	pMill->probabilityHook	=30;
	pMill->probabilitySeven	=30;
	pMill->probabilityHeave	=25;
	pMill->probabilityBar	=23;
	pMill->probabilityTwo	=25;
	pMill->probabilitySpade	=25;

	pMill->MillColor		=RGB(200,200,250);
	pMill->PreColor			=RGB(255,0,0);

	//方块
	pDiamond=new CDiamond;
	pDiamond->DiamondColor=RGB(100,100,255);
	pMill->MadeDiamond();


	//障碍物
	pBlock=new CBlock;
	pBlock->BlockColor=RGB(255,255,100);

	//开始
	Timer1=50;
	Timer2=350-Level*40;
	FallTimer=30;
	SetTimer(theApp.m_pMainWnd->m_hWnd,1,Timer1,NULL);
	SetTimer(theApp.m_pMainWnd->m_hWnd,2,Timer2,NULL);
}

bool CManager::WhetherLoss()
{
	for(int ix=0;ix<=pPanel->nHGridNum;ix++)
	{
		if(pBlock->BlockPanel[ix][0])
		{
			SetFree();
			if(Sound) PlaySound("sound\\gameover.wav",NULL,SND_FILENAME | SND_ASYNC);
			Sleep(3000);
			///////
			InitGame(1);
		}
	}
	return false;
}

void CManager::DrawScene()
{
	CDC* pDC;
	CBitmap MemoryBmp;
	pDC=new CDC;
	CDC *pFontDC;
	pFontDC=new CClientDC(theApp.m_pMainWnd);;
	pDC->CreateCompatibleDC(pFontDC);
	MemoryBmp.CreateCompatibleBitmap(pFontDC,ClientRect.right-ClientRect.left,ClientRect.bottom-ClientRect.top);
	pDC->SelectObject(&MemoryBmp);

	pDC->PatBlt(0,0,ClientRect.Width(),ClientRect.Height(),PATCOPY);

	pPanel->DrawPanel(pDC);
	pBlock->DrawBlock(pDC);
	pMill->DrawPrepare(pDC);
	pDiamond->DrawDiamond(pDC);

	pFontDC->BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),pDC,0,0,SRCCOPY);
	MemoryBmp.DeleteObject();
	pDC->DeleteDC();
	delete pDC;
	delete pFontDC;
}

void CManager::OnTimer(UINT &nIDEvent)
{

	if(nIDEvent==1)
	{
		if(KeyLeft)
		{
			pDiamond->MoveDiamondLeft();
			DrawScene();
		}
		if(KeyRight) 
		{
			pDiamond->MoveDiamondRight();
			DrawScene();
		}
	}
	else if(nIDEvent==2)
	{

		if(!pDiamond->MoveDiamondDown())
		{
			pBlock->AddBlock();
			pDiamond->FullLine();
			pMill->MadeDiamond();
			WhetherLoss();
			if(Result>=Level*50) PassLevel();
		}
		DrawScene();
	}
}

void CManager::OnKeyDown(UINT &nChar)
{
	switch (nChar)
	{
	case VK_LEFT:
		KeyLeft=true;
		break;
	case VK_DOWN:
		SetTimer(theApp.m_pMainWnd->m_hWnd,2,FallTimer,NULL);
		KeyDown=true;
		break;
	case VK_RIGHT:
		KeyRight=true;
		break;
	case VK_UP:
	case VK_SPACE:
		pDiamond->RollDiamond();
		DrawScene();
		break;
	}
}

void CManager::OnKeyUp(UINT &nChar)
{
	switch (nChar)
	{
	case VK_LEFT:
		KeyLeft=false;
		break;
	case VK_DOWN:
		SetTimer(theApp.m_pMainWnd->m_hWnd,2,Timer2,NULL);
		KeyDown=false;
		break;
	case VK_RIGHT:
		KeyRight=false;
		break;
	case VK_UP:
	case VK_SPACE:
		KeySpace=false;
		break;
	}
}