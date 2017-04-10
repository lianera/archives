// Mill.cpp: implementation of the CMill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Mill.h"
#include "Panel.h"
#include <MMSYSTEM.H>
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HINSTANCE hInst;

CMill::CMill()
{

}

CMill::~CMill()
{

}


void CMill::DoApple()
{
	m_pos.x=rand()%GRIDXNUMBER;
	m_pos.y=rand()%GRIDXNUMBER;
}

void CMill::Draw(HDC hDC,CPanel *pPanel)
{
	TransparentBlt(hDC,pPanel->GetGcx()*m_pos.x,pPanel->GetGcx()*m_pos.y,pPanel->GetGcx(),pPanel->GetGcy(),
					AppleDC,0,0,47,47,RGB(255,0,0));	
}

bool CMill::EatApple()
{
	if(!PlaySound((LPCTSTR)IDR_EAT, NULL, SND_RESOURCE | SND_ASYNC)) OnErr();
	if(m_AppleNum>0) 
	{
		m_AppleNum--;
		DoApple();
		Sleep(50);
		return true;
	}
	return false;
}


void CMill::InitDraw(HDC hDC)
{
	appleBmp=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_APPLE));
	AppleDC=CreateCompatibleDC(hDC);
	SelectObject(AppleDC,appleBmp);
}
