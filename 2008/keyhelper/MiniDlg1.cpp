// MiniDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "°´¼üÖúÊÖ.h"
#include "MiniDlg1.h"
#include "ChildDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniDlg dialog


CMiniDlg::CMiniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiniDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMiniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniDlg)
	DDX_Control(pDX, ID_CTRL_STOP, m_Stop);
	DDX_Control(pDX, ID_CTRL_PAUSE, m_Pause);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMiniDlg, CDialog)
	//{{AFX_MSG_MAP(CMiniDlg)
	ON_BN_CLICKED(ID_CTRL_PAUSE, OnCtrlPause)
	ON_BN_CLICKED(ID_CTRL_STOP, OnCtrlStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniDlg message handlers

BOOL CMiniDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	HICON hicoPause,hicoStop;
	hicoPause=AfxGetApp()->LoadIcon(IDI_PAUSE);
	hicoStop=AfxGetApp()->LoadIcon(IDI_STOP);

	m_Pause.SetIcon(hicoPause);
	m_Stop.SetIcon(hicoStop);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMiniDlg::OnCtrlPause() 
{	
}

CChildDlg *pChild;
void CMiniDlg::OnCtrlStop() 
{

	
	//pMain=AfxGetMainWnd();
	//pMain->ShowWindow(SW_SHOW);
	//this->DestroyWindow();
}
