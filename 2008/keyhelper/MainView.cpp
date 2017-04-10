// MainView.cpp : implementation of the CMainView class
//
#include "stdafx.h"
#include "梓囚廁返.h"
#include "ChildDlg.h"
#include "MiniDlg1.h"
#include "MainDoc.h"
#include "MainView.h"
#include "MainFrm.h"
#include "VMKey.h"
#include <afxwin.h>
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
volatile int nCommand;
void CarryKry(bool way,short vKey);
CString vKeyToChar(short vKey);
long nListNum;
short snNewKeyStu=0,snOldKey[120];
long lDerfer=0;

//CMiniDlg *m_dlgMini;
/////////////////////////////////////////////////////////////////////////////
// CMainView

IMPLEMENT_DYNCREATE(CMainView, CView)

BEGIN_MESSAGE_MAP(CMainView, CView)
	//{{AFX_MSG_MAP(CMainView)
	ON_WM_CREATE()
	ON_COMMAND(ID_CTRL_PAUSE, OnCtrlPause)
	ON_COMMAND(ID_CTRL_REC, OnCtrlRec)
	ON_COMMAND(ID_CTRL_RUN, OnCtrlRun)
	ON_COMMAND(ID_CTRL_STOP, OnCtrlStop)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////
long StrToLong(char const *string)
{	
	return atol(string);
}

/////////////////////////////////////////////////////////////////////////////
// CMainView construction/destruction

CMainView::CMainView()
{
	// TODO: add construction code here

}

CMainView::~CMainView()
{
}

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainView drawing

void CMainView::OnDraw(CDC* pDC)
{
	CMainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMainView printing

BOOL CMainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMainDoc* CMainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
}
#endif //_DEBUG

//-----------------------------------------------------------------------------
// CMainView message handlers
CChildDlg *m_dlgChild;
UINT ActionRunAfx(LPVOID pParam);
int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_dlgChild=new CChildDlg();
	m_dlgChild->Create(IDD_CHILD,this);	
	m_dlgChild->MoveWindow(0,0,1280,1024);
	m_dlgChild->ShowWindow(SW_SHOW);
	HWND hWnd = GetSafeHwnd();
	AfxBeginThread(ActionRunAfx,this, THREAD_PRIORITY_NORMAL);
	//////////////////////////////////////////////
	for(int j=0;j<120;j++) {snOldKey[j]=0;}
	///////////////////////////////////////////////
	SetTimer(2,33,NULL);
	return 0;
}

//---------------------------------------------------------------
void CMainView::OnCtrlPause() 
{
	nCommand=COMPAUSE;
	KillTimer(1);
}

//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

void CMainView::OnCtrlRec() 
{
	CWnd* pMain=AfxGetMainWnd();
	pMain->ShowWindow(SW_MINIMIZE);
	m_dlgChild->m_ActionList.DeleteAllItems();
	m_dlgChild->EnableWindow(false);
	nListNum=0;
	SetTimer(1,10,NULL);
	if (m_dlgChild->m_bAlwayRec==1) SetTimer(3,40,NULL);
	for(int j=0;j<120;j++) {snOldKey[j]=0;}
	snNewKeyStu=0;
}
//CMiniDlg *m_dlgMini;
//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
void CMainView::OnCtrlStop() 
{
	nCommand=COMSTOP;
	KillTimer(1);
	KillTimer(3);
	CWnd* pMain=AfxGetMainWnd();
	pMain->ShowWindow(SW_SHOW);
	m_dlgChild->EnableWindow(true);
}
//---------------------------------------------------------------------------

//////////////////村崙泣似強恬//////////////////////
int nOldCom;
void CMainView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		lDerfer+=10;
		for (int i=0;i<120;i++)
		{
   			snNewKeyStu=GetAsyncKeyState(i);
			if(snNewKeyStu==0)
			{
				if(snOldKey[i]==-32768) CarryKry(false,i);
			}
			else if(snNewKeyStu==1)
			{
				CarryKry(true,i);
				CarryKry(false,i);
			}
			else if(snNewKeyStu==-32767)
			{
				if(snOldKey[i]==0) CarryKry(true,i);
			}
			snOldKey[i]=snNewKeyStu;
		}
	}
	if(nIDEvent==2)
	{
			if(GetAsyncKeyState(115)==-32767) this->OnCtrlRec();
			if(GetAsyncKeyState(116)==-32767) this->OnCtrlRun();
			if(GetAsyncKeyState(117)==-32767) this->OnCtrlPause();
			if(GetAsyncKeyState(118)==-32767) this->OnCtrlStop();
			if(nCommand==COMSTOP && nOldCom!=COMSTOP) this->OnCtrlStop();
			nOldCom=nCommand;
	}
	if(nIDEvent==3)
	{
		lDerfer+=10;
		CarryKry(0,0);
	}
	CView::OnTimer(nIDEvent);
}

//----------------------------------------------------------------------------
UINT ActionRunAfx(LPVOID pParam)
{	
	while(true)
	{	Sleep(50);
		while(nCommand==COMSTOP) {Sleep(50);}
		if (nCommand==COMRUN)
		{
			CString Action,strValue;
			for (int i=0;i<=m_dlgChild->m_ActionList.GetItemCount();i++)
			{	if(nCommand==COMSTOP) break;
				while(nCommand==COMPAUSE) {Sleep(50);}
				Action=m_dlgChild->m_ActionList.GetItemText(i,1);
				strValue=m_dlgChild->m_ActionList.GetItemText(i,2);
				if (Action=="决扮")
				{	
					Sleep(StrToLong(strValue));
				}
				if (Action=="卞強報炎")
				{	long PosX,PosY;
					CString PX,PY;
					PX=strValue.Left(strValue.FindOneOf(","));
					PY=strValue.Right(strValue.GetLength()-strValue.FindOneOf(",")-1);
					PosX=StrToLong(PX);
					PosY=StrToLong(PY);
					SetCursorPos(PosX,PosY);
				}
				if (Action=="梓和")
				{	
					int Key;
					if (strValue=="MouseLeft") mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
					else if(strValue=="MouseMid") mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
					else if(strValue=="MouseRight") mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
					else if(Key=KeyList(strValue)) keybd_event(Key,MapVirtualKey(Key,0),0,0);
				}
				if (Action=="箕軟")
				{	int Key;
					if (strValue=="MouseLeft") mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
					else if(strValue=="MouseMid") mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
					else if(strValue=="MouseRight") mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
					else if(Key=KeyList(strValue)) keybd_event(Key,MapVirtualKey(Key,0),KEYEVENTF_KEYUP,0);
				}
			}

			nCommand=COMSTOP;
		}
	}
	return 0;
}


void CMainView::OnCtrlRun() 
{	CWnd* pMain=AfxGetMainWnd();
	//CWnd *pMain=AfxGetMainWnd();
	//::ShowWindow(*pMain,SW_HIDE);
	//this->ShowWindow(SW_HIDE);
	//m_dlgMini=new CMiniDlg;
	//m_dlgMini->Create(IDD_DIALOGBAR,this);
	//m_dlgMini->ShowWindow(SW_SHOW);
	//CWnd* pMain=AfxGetMainWnd();
	pMain->ShowWindow(SW_MINIMIZE);
	//this->ShowWindow(SW_MINIMIZE);
	nCommand=COMRUN;

}

//////////////way 梓和葎true 箕軟葎false///////////

POINT OldPos,NewPos;
void CarryKry(bool way,short vKey)
{
	CString str1,str2;
	str1.Format("%d",nListNum);
	m_dlgChild->m_ActionList.InsertItem(nListNum,str1);
	m_dlgChild->m_ActionList.SetItemText(nListNum,1,"决扮");
	str1.Format("%d",lDerfer);
	m_dlgChild->m_ActionList.SetItemText(nListNum,2,str1);
	nListNum++;
	lDerfer=0;
	GetCursorPos(&NewPos);
	str1.Format("%d",nListNum);
	m_dlgChild->m_ActionList.InsertItem(nListNum,str1);
	m_dlgChild->m_ActionList.SetItemText(nListNum,1,"卞強報炎");
	str1.Format("%d",NewPos.x);
	str1+=',';
	str2.Format("%d",NewPos.y);
	str2=str1+str2;
	m_dlgChild->m_ActionList.SetItemText(nListNum,2,str2);
	nListNum++;
	if(vKey!=0)
	{
		if (way==true)
		{
			str1.Format("%d",nListNum);
			m_dlgChild->m_ActionList.InsertItem(nListNum,str1);
			m_dlgChild->m_ActionList.SetItemText(nListNum,1,"梓和");
			m_dlgChild->m_ActionList.SetItemText(nListNum,2,vKeyToChar(vKey));
			nListNum++;
		}
		else
		{
			str1.Format("%d",nListNum);
			m_dlgChild->m_ActionList.InsertItem(nListNum,str1);
			m_dlgChild->m_ActionList.SetItemText(nListNum,1,"箕軟");
			m_dlgChild->m_ActionList.SetItemText(nListNum,2,vKeyToChar(vKey));
			nListNum++;
		}
	}
}

CString vKeyToChar(short vKey)
{	CString ch;
	if ((vKey>=48 && vKey<=57) || (vKey>=65 && vKey<=90)) 
		return char(vKey);
	else
	{
	  switch (vKey)
	  {
	  case 13:
		  ch = "ENTER";
			break;
		case 9:
		  ch = "TAB";
			break;
		case 38:
		  ch = "UPARROW";
			break;
		case 40:
		  ch = "DOWNARROW";
			break;
		case 37:
		  ch = "LEFTARROW";
			break;
		case 39:
		  ch = "RIGHTARROW";
			break;
		case 112:
		  ch = "F1";
			break;
		case 113:
		  ch = "F2";
			break;
		case 114:
		  ch = "F3";
			break;
		case 115:
		  ch = "F4";
			break;
		case 116:
		  ch = "F5";
			break;
		case 117:
		  ch = "F6";
			break;
		case 118:
		  ch = "F7";
			break;
		case 119:
		  ch = "F8";
			break;
		case 120:
		  ch = "F9";
			break;
		case 121:
		  ch = "F10";
			break;
		case 122:
		  ch = "F11";
			break;
		case 123:
		  ch = "F12";
			break;
		case 32:
		  ch = "SPACEBAR";
			break;
		case 17:
		  ch = "Ctrl";
			break;
		case 8:
		  ch = "SPACEBAR";
			break;
		case 18:
		  ch = "ALT";
			break;
		case 91:
		  ch = "LeftWindows";
			break;
		case 92:
		  ch = "RightWindows";
			break;
		case 93:
		  ch = "menu";
			break;
		case 16:
		  ch = "SHIFT";
			break;
		case 1:
		  ch = "MouseLeft";
			break;
		case 97:
		  ch = "NUM1";
			break;
		case 98:
		  ch = "NUM2";
			break;
		case 99:
		  ch = "NUM3";
			break;
		case 100:
		  ch = "NUM4";
			break;
		case 101:
		  ch = "NUM5";
			break;
		case 102:
		  ch = "NUM6";
			break;
		case 103:
		  ch = "NUM7";
			break;
		case 104:
		  ch = "NUM8";
			break;
		case 105:
		  ch = "NUM9";
			break;
		case 46:
		  ch = "Delete";
			break;
		case 2:
		  ch = "MouseRight";
			break;
		case 96:
		  ch = "NUM0";
			break;
		case 110:
		  ch = ".";
			break;
		case 27:
		  ch = "ESC";
			break;

		}
		return ch;
	}

}
