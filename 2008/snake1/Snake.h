// Snake.h: interface for the CSnake class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNAKE_H__75E8C5AD_7BD4_4F6C_84F8_B10DAB673DBA__INCLUDED_)
#define AFX_SNAKE_H__75E8C5AD_7BD4_4F6C_84F8_B10DAB673DBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Mill.h"
#include "Node.h"
#include "Panel.h"
enum ETrend{NOTREND=0,LEFT=1,UP=2,RIGHT=3,DOWN=4};

class CSnake  
{
public:
	bool IsEatApple(CMill *pMill);
	bool IsBump(CPanel *pPanel);
	bool SetTrend(ETrend trend);
	void InitDraw(HDC hDC);
	void DrawSnack(HDC hDC,CPanel *pPanel);
	bool GetSnack(int *x,int *y);
	ETrend GetTrend();
	bool MoveSnack(int pace=1);
	bool IsEmpty();
	CSnake(int len);
	bool SetInitLength(int len);
	int GetLength();
	bool DeleteNode(int num=1);
	bool AddNode(int num=1);
	CSnake();
	virtual ~CSnake();
	
private:
	HBITMAP m_shrunken;
	bool m_issk;
	HDC m_bmDC;
	HBITMAP m_nodebmp;
	HBITMAP m_headbmp;
	bool SwitchTrend(ETrend trend,int &x,int &y,int pace=1);
	ETrend NotTrend();
	CNode* m_tail;
	CNode* m_head;
	int m_length;
	ETrend m_headTrend;
};

#endif // !defined(AFX_SNAKE_H__75E8C5AD_7BD4_4F6C_84F8_B10DAB673DBA__INCLUDED_)
