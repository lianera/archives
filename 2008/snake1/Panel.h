// Panel.h: interface for the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANEL_H__4937E4A4_9A4F_412B_B965_4F7649CADB3F__INCLUDED_)
#define AFX_PANEL_H__4937E4A4_9A4F_412B_B965_4F7649CADB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPanel  
{
public:
	void InitDraw(HDC hDC);
	int GetHeight();
	int GetWidth();
	RECT GetInfoRect();
	RECT GetGround();
	int GetGcy();
	int GetGcx();
	void BuildPanel();
	void DrawPanel(HDC hDC);
	CPanel();
	virtual ~CPanel();
	int GetGx(){return m_gx;}
	int GetGy(){return m_gy;}
private:
	int m_gy;
	int m_gx;
	RECT m_groundRect;
	RECT m_infoRect;
	int  m_gcx;
	int  m_gcy;
	HPEN infopen;
	HPEN groundpen;
	HBRUSH groundbrush;
	HBRUSH infobrush;
};

const int GROUNDLEFT	=0;
const int GROUNDRIGHT	=WINWIDTH-100;
const int GROUNDTOP		=0;
const int GROUNDBUTTOM	=WINHEIGHT;

const int INFOLEFT	=GROUNDRIGHT;
const int INFORIGHT	=WINWIDTH;
const int INFOTOP	=0;
const int INFOBUTTOM=WINHEIGHT;

const int GRIDXNUMBER=20;
const int GRIDYNUMBER=20;

const COLORREF GROUNDBORDERCOLOR	=RGB(255,255,0);
const COLORREF GROUNDFACECOLOR		=RGB(0,0,0);
const int GROUNDWIDE=2;

const DWORD INFOBORDERCOLOR	=RGB(100,100,255);
const DWORD INFOFACECOLOR	=RGB(200,200,200);
const int INFOWIDE=2;

extern HWND hWnd;

#endif // !defined(AFX_PANEL_H__4937E4A4_9A4F_412B_B965_4F7649CADB3F__INCLUDED_)
