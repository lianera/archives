// Mill.h: interface for the CMill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MILL_H__6218AF55_FD23_4733_841C_402936BD0A6F__INCLUDED_)
#define AFX_MILL_H__6218AF55_FD23_4733_841C_402936BD0A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Panel.h"
class CMill  
{
public:
	POINT GetPos(){return m_pos;}
	void InitDraw(HDC hDC);
	bool EatApple();
	void Draw(HDC hDC,CPanel *pPanel);
	void DoApple();
	CMill();
	virtual ~CMill();
	void SetAppleNum(int num){m_AppleNum=num;}
	int GetApple(){return m_AppleNum;}
private:
	HDC AppleDC;
	HBITMAP appleBmp;
	POINT m_pos;
	int m_AppleNum;
};

#endif // !defined(AFX_MILL_H__6218AF55_FD23_4733_841C_402936BD0A6F__INCLUDED_)
