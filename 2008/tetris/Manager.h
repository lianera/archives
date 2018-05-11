// Manager.h: interface for the CManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANAGER_H__311D53B9_CF58_4FDA_A1C7_5C534814EC93__INCLUDED_)
#define AFX_MANAGER_H__311D53B9_CF58_4FDA_A1C7_5C534814EC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CManager  
{
public:
	CManager();
	virtual ~CManager();
	void InitGame(int Level);
	void DrawScene();
	void OnTimer(UINT &nIDEvent);
	void OnKeyDown(UINT &nChar);
	void OnKeyUp(UINT &nChar);
	bool WhetherLoss();
	void SetFree();
private:

	void PassLevel();
private:
	bool KeyLeft,KeyDown,KeyRight,KeySpace;


public:
	CRect ClientRect;
	UINT Timer1;
	UINT Timer2;
	UINT FallTimer;
	int Level;
	long Result;
	int	 LineNumber;
	int SeriesLine;
	bool Sound;
	CString Name;
};

const int DIAMONDCAKE	=0;
const int DIAMONDHOOK	=1;
const int DIAMONDSEVEN	=2;
const int DIAMONDHEAVE	=3;
const int DIAMONDBAR	=4;
const int DIAMONDTWO	=5;
const int DIAMONDSPADE	=6;

#endif // !defined(AFX_MANAGER_H__311D53B9_CF58_4FDA_A1C7_5C534814EC93__INCLUDED_)
