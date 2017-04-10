// Mill.h: interface for the CMill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MILL_H__BD3FF7BC_BB6B_4576_A51C_1A688A98D9E1__INCLUDED_)
#define AFX_MILL_H__BD3FF7BC_BB6B_4576_A51C_1A688A98D9E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMill  
{
public:
	CMill();
	virtual ~CMill();
	void MadeDiamond();
	void DrawPrepare(CDC* pDC);
public:
	int PreDiamondType;
	int PreDiamondState;
	RECT MillRect;
	CSize GridSize;
	COLORREF MillColor;
	COLORREF PreColor;
	int probabilityCake;
	int probabilityHook;
	int probabilitySeven;
	int probabilityHeave;
	int probabilityBar;
	int probabilityTwo;
	int probabilitySpade;
};

#endif // !defined(AFX_MILL_H__BD3FF7BC_BB6B_4576_A51C_1A688A98D9E1__INCLUDED_)
