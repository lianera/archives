// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__4AA9A689_CFB1_456F_8C6D_EEFF69125C54__INCLUDED_)
#define AFX_GAME_H__4AA9A689_CFB1_456F_8C6D_EEFF69125C54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mill.h"
#include "Snake.h"
#include "resource.h"
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"winmm.lib")

extern HINSTANCE hInst;

class CGame  
{
public:
	void Load(LPTSTR name);
	void Save(LPTSTR name);
	void NotPause();
	void Pause();
	void CheckKey();
	void MeamCart();
	void BeginCart();
	void OnEnd();
	void ShowInfo();
	void PlayMid(LPTSTR name);
	void Destroy();
	void OnPassLevel();
	CPanel* GetPanel();
	void OnLose();
	void InitDraw();
	void OnTimer(int ID);
	void OnKeyUp(int key);
	void OnKeyDown(int key);
	void DrawScene();
	void OnStart();
	CGame();
	virtual ~CGame();

private:
	/*bool DownKey;
	bool RightKey;
	bool UpKey;
	bool LeftKey;*/
	bool m_isbegin;
	int m_life;
	UINT wDeviceID;
	int m_Level;
	HBITMAP swapBmp;
	HDC swapDC;
	HDC theDC;
	CSnake* pSnake;
	CPanel* pPanel;
	CMill *pMill;
	ETrend lastKey;
};

#endif // !defined(AFX_GAME_H__4AA9A689_CFB1_456F_8C6D_EEFF69125C54__INCLUDED_)
