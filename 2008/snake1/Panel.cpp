// Panel.cpp: implementation of the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Panel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


extern HWND hWnd;

CPanel::CPanel()
{
	BuildPanel();
}

CPanel::~CPanel()
{
	
}

void CPanel::DrawPanel(HDC hDC)
{

	HPEN oldpen=(HPEN)SelectObject(hDC,groundpen);
	HBRUSH oldbrush=(HBRUSH)SelectObject(hDC,groundbrush);
	Rectangle(hDC,m_groundRect.left,m_groundRect.top,m_groundRect.right,m_groundRect.bottom);

	SelectObject(hDC,infopen);
	SelectObject(hDC,infobrush);
	Rectangle(hDC,m_infoRect.left,m_infoRect.top,m_infoRect.right,m_infoRect.bottom);

	SelectObject(hDC,oldpen);
	SelectObject(hDC,oldbrush);
}

void CPanel::BuildPanel()
{
	m_groundRect.left	=GROUNDLEFT;
	m_groundRect.right	=GROUNDRIGHT;
	m_groundRect.top	=GROUNDTOP;
	m_groundRect.bottom	=GROUNDBUTTOM;

	m_infoRect.left		=INFOLEFT;
	m_infoRect.right	=INFORIGHT;
	m_infoRect.top		=INFOTOP;
	m_infoRect.bottom	=INFOBUTTOM;

	m_gx=GRIDXNUMBER;
	m_gy=GRIDYNUMBER;

	m_gcx=GetWidth()/GRIDXNUMBER;
	m_gcy=GetWidth()/GRIDYNUMBER;
}

int CPanel::GetGcx()
{
	return m_gcx;
}

int CPanel::GetGcy()
{
	return m_gcy;
}

RECT CPanel::GetGround()
{
	return m_groundRect;
}

RECT CPanel::GetInfoRect()
{
	return m_infoRect;
}

int CPanel::GetWidth()
{
	return m_groundRect.right-m_groundRect.left;
}

int CPanel::GetHeight()
{
	return m_groundRect.bottom-m_groundRect.top;
}

void CPanel::InitDraw(HDC hDC)
{
	groundpen=CreatePen(PS_SOLID,GROUNDWIDE,GROUNDBORDERCOLOR);
	infopen=CreatePen(PS_SOLID,INFOWIDE,INFOBORDERCOLOR);

	groundbrush=CreateSolidBrush(GROUNDFACECOLOR);
	infobrush=CreateSolidBrush(INFOFACECOLOR);
}
