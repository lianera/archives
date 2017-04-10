// MiniDlg.cpp : implementation file
//

#include "stdafx.h"
#include "°´¼üÖúÊÖ.h"
#include "MiniDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MiniDlg dialog


MiniDlg::MiniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MiniDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MiniDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MiniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MiniDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MiniDlg, CDialog)
	//{{AFX_MSG_MAP(MiniDlg)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MiniDlg message handlers

BOOL MiniDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MiniDlg::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
