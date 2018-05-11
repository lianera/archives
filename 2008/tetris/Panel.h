// Panel.h: interface for the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANEL_H__0FF0F9CA_A17E_4BD1_8172_CCEDCE1CE79D__INCLUDED_)
#define AFX_PANEL_H__0FF0F9CA_A17E_4BD1_8172_CCEDCE1CE79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPanel  
{
public:
	CPanel();
	virtual ~CPanel();
	void DrawPanel(CDC* pDC);
	void PanelPosToPos(POINT &PanelPos);
	void PosToPanelPos(POINT &Pos);
public:
	RECT PanelRect;
	COLORREF PanelColor;
	int nHGridNum;
	int nVGridNum;
	CSize GridSize;
	CRect TextRect;
};

#endif // !defined(AFX_PANEL_H__0FF0F9CA_A17E_4BD1_8172_CCEDCE1CE79D__INCLUDED_)
