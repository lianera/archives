// 俄罗斯方块Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "俄罗斯方块.h"
#include "俄罗斯方块Dlg.h"
#include "Manager.h"
#include "Name.h"
#include "Panel.h"
#include <mmsystem.h>
#include "Block.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CManager*	pManager;
extern CPanel* pPanel;
extern CBlock* pBlock;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_COMMAND(IDM_CLOSE, OnClose)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_LOAD, OnLoad)
	ON_COMMAND(IDM_PAUSE, OnPause)
	ON_COMMAND(IDM_RANGE, OnRange)
	ON_COMMAND(IDM_RESTART, OnRestart)
	ON_COMMAND(IDM_SAVE, OnSave)
	ON_COMMAND(IDM_SOUND, OnSound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization her
	pManager=new CManager;
	CName GetName;
	GetName.DoModal();
	pManager->Name=GetName.m_Name;
	pManager->InitGame(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMyDlg::OnTimer(UINT nIDEvent) 
{

	pManager->OnTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}

void CMyDlg::OnClose() 
{
	delete pManager;
	this->CloseWindow();
}

void CMyDlg::OnAbout() 
{
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
}

void CMyDlg::OnFont() 
{
		
}

void CMyDlg::OnLoad() 
{
   CStdioFile file;
   CString str;
   if(file.Open("save.txt",CFile::modeRead,NULL))
   {

		file.ReadString(str);
		int swLevel,swLineNumber;
		long swResult;
		file.Read(&swLevel		,sizeof(pManager->Level		));
		file.Read(&swLineNumber	,sizeof(pManager->LineNumber));
		file.Read(&swResult		,sizeof(pManager->Result	));
		
		pManager->Level		=swLevel;
		pManager->SetFree();
		pManager->InitGame(pManager->Level);
		pManager->Name		=str;
		pManager->LineNumber=swLineNumber;
		pManager->Result	=swResult;

	   for(int iy=0;iy<=pPanel->nVGridNum;iy++)
	   {
		   for(int ix=0;ix<pPanel->nHGridNum;ix++)
		   {
				file.Read(&pBlock->BlockPanel[ix][iy],sizeof(bool));
		   }
	   }
   }
   else	MessageBox("载入失败！",NULL,MB_OK);
	
}

void CMyDlg::OnPause() 
{
	KillTimer(1);
	KillTimer(2);
	MessageBox("已暂停\n要开始请点击确定","暂停",MB_OK);
	SetTimer(1,pManager->Timer1,NULL);
	SetTimer(2,pManager->Timer2,NULL);
}

void CMyDlg::OnRange() 
{
	
}

void CMyDlg::OnRestart() 
{
	pManager->InitGame(1);	
}

void CMyDlg::OnSave() 
{
   CStdioFile file;
   CString str;
   if(file.Open("save.txt",CFile::modeWrite | CFile::modeCreate,NULL))
   {
	   str=pManager->Name+"\n";
	   file.WriteString(str);

	   file.Write(&pManager->Level		,sizeof(pManager->Level		));
	   file.Write(&pManager->LineNumber	,sizeof(pManager->LineNumber));
	   file.Write(&pManager->Result		,sizeof(pManager->Result	));

	   for(int iy=0;iy<=pPanel->nVGridNum;iy++)
	   {
		   for(int ix=0;ix<pPanel->nHGridNum;ix++)
		   {
				file.Write(&pBlock->BlockPanel[ix][iy],sizeof(bool));
		   }
	   }
   }
   else	MessageBox("保存失败！",NULL,MB_OK);

   file.Close();
}

void CMyDlg::OnSetparameter() 
{
}

void CMyDlg::OnSound() 
{
	pManager->Sound=pManager->Sound?0:1;
}

BOOL CMyDlg::PreTranslateMessage(MSG* pMsg) 
{
	 if(pMsg->message==WM_KEYDOWN)
		pManager->OnKeyDown(pMsg->wParam);
	 else if(pMsg->message==WM_KEYUP)
		 pManager->OnKeyUp(pMsg->wParam);
	return CDialog::PreTranslateMessage(pMsg);
}
