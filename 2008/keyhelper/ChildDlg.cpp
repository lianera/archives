// ChildDlg.cpp : implementation file
//

#include "stdafx.h"
#include "按键助手.h"
#include "ChildDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildDlg dialog


CChildDlg::CChildDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChildDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChildDlg)
	m_lDefer = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	m_bKeyDown = FALSE;
	m_bKeyUp = FALSE;
	m_strKey = _T("");
	m_lRepeat = 0;
	m_bMoveMouse = FALSE;
	m_bGetPos = FALSE;
	m_bAlwayRec = FALSE;
	//}}AFX_DATA_INIT
}


void CChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildDlg)
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Control(pDX, IDC_CHECK_MOVEMOUSE, m_cMoveMouse);
	DDX_Control(pDX, IDC_CHECK_KEYUP, m_bKeyUpState);
	DDX_Control(pDX, IDC_CHECK_KEYDOWN, m_bKeyDownState);
	DDX_Control(pDX, IDC_LIST_ACTION, m_ActionList);
	DDX_Text(pDX, IDC_EDIT_DEFER, m_lDefer);
	DDV_MinMaxLong(pDX, m_lDefer, 0, 2147483640);
	DDX_Text(pDX, IDC_EDIT_POSX, m_nPosX);
	DDV_MinMaxInt(pDX, m_nPosX, 0, 32760);
	DDX_Text(pDX, IDC_EDIT_POSY, m_nPosY);
	DDV_MinMaxInt(pDX, m_nPosY, 0, 32760);
	DDX_Check(pDX, IDC_CHECK_KEYDOWN, m_bKeyDown);
	DDX_Check(pDX, IDC_CHECK_KEYUP, m_bKeyUp);
	DDX_CBString(pDX, IDC_COMBO_KEY, m_strKey);
	DDV_MaxChars(pDX, m_strKey, 15);
	DDX_Text(pDX, IDC_EDITREPEAT, m_lRepeat);
	DDV_MinMaxLong(pDX, m_lRepeat, 0, 2147483640);
	DDX_Check(pDX, IDC_CHECK_MOVEMOUSE, m_bMoveMouse);
	DDX_Check(pDX, IDC_CHECK_GETPOS, m_bGetPos);
	DDX_Check(pDX, IDC_CHECK_ALWAYREC, m_bAlwayRec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildDlg, CDialog)
	//{{AFX_MSG_MAP(CChildDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_MOVEMOUSE, OnCheckMovemouse)
	ON_BN_CLICKED(IDC_CHECK_GETPOS, OnCheckGetpos)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK_ALWAYREC, OnCheckAlwayrec)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ACTION, OnRclickListAction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
void CChildDlg::ArrayList()
{
	//重新排列索引
	long nItemCount;
	CString str;
	nItemCount=m_ActionList.GetItemCount();
	for(long i=0;i<=nItemCount;i++)
	{
		str.Format("%d",i);
		m_ActionList.SetItemText(i,0,str);
	}
}

/*void CChildDlg::AddListData(int flag,long num1,long num2,CString string)
{
	int nItemCount;
	CString str,str2;
	nItemCount=m_ActionList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)==-1?m_ActionList.GetItemCount():m_ActionList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);	
	str.Format("%d",nItemCount);
	m_ActionList.InsertItem(nItemCount,str);			//No.1 索引
	if(flag==1) 
	{
		m_ActionList.SetItemText(nItemCount,1,"延时");
		str.Format("%d",num1);
		m_ActionList.SetItemText(nItemCount,2,str);	
	}		//No.2 动作
	else if(flag==2) 
	{
		m_ActionList.SetItemText(nItemCount,1,"移动鼠标");
		str.Format("%d",num1);str+=',';str2.Format("%d",num2);str+=str2;
		m_ActionList.SetItemText(nItemCount,2,str);	
		}
	else if(flag==3) 
	{
		m_ActionList.SetItemText(nItemCount,1,"按下");
		m_ActionList.SetItemText(nItemCount,2,string);	
	}
	else if(flag==4) 
	{
		m_ActionList.SetItemText(nItemCount,1,"抬起");
		m_ActionList.SetItemText(nItemCount,2,string);	
	}
		//No.3 值
}*/
/////////////////////////////////////////////////////////////////////////////
// 添加
void CChildDlg::OnButtonAdd() 
{
	UpdateData();
	long nItemCount,nCurSel;
	CString str,str2;
	nCurSel=m_ActionList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	nItemCount=(nCurSel==-1)?m_ActionList.GetItemCount():m_ActionList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//重复
	for(int i=0;i<=m_lRepeat;i++)
	{
		//延时
		if(m_lDefer)
		{	str.Format("%d",nItemCount);
			m_ActionList.InsertItem(nItemCount,str);
			m_ActionList.SetItemText(nItemCount,1,"延时");
			str.Format("%d",m_lDefer);
			m_ActionList.SetItemText(nItemCount,2,str);
			nItemCount++;
			//this->ArrayList();
		}
		//鼠标坐标
		if(m_bMoveMouse)
		{	str.Format("%d",nItemCount);
			m_ActionList.InsertItem(nItemCount,str);
			m_ActionList.SetItemText(nItemCount,1,"移动鼠标");
			str.Format("%d",m_nPosX);
			str+=',';
			str2.Format("%d",m_nPosY);
			str+=str2;
			m_ActionList.SetItemText(nItemCount,2,str);	
			nItemCount++;
			//this->ArrayList();
		}
		//按键
		if(m_strKey!="")
		{
			if(m_bKeyDown)
			{	str.Format("%d",nItemCount);
				m_ActionList.InsertItem(nItemCount,str);
				m_ActionList.SetItemText(nItemCount,1,"按下");
				m_ActionList.SetItemText(nItemCount,2,m_strKey);
				nItemCount++;
				//this->ArrayList();
			}
			if(m_bKeyUp)
			{	str.Format("%d",nItemCount);
				m_ActionList.InsertItem(nItemCount,str);
				m_ActionList.SetItemText(nItemCount,1,"抬起");
				m_ActionList.SetItemText(nItemCount,2,m_strKey);
				nItemCount++;
				//this->ArrayList();
			}
		}
	}

}

void CChildDlg::OnButtonDelete() 
{
	while (m_ActionList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)!=-1)
	{
		m_ActionList.DeleteItem(m_ActionList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED));
	}
	this->ArrayList();
}

void CChildDlg::OnButtonClean() 
{
	m_ActionList.DeleteAllItems();	
	//m_lDefer=0;
	//m_lRepeat=0;
	//m_nPosX=0;
	//m_nPosY=0;
	//m_strKey="";
	UpdateData(false);
}

int CChildDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CChildDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_ActionList.InsertColumn(0,"索引",LVCFMT_LEFT,50,0);
	m_ActionList.InsertColumn(1,"动作",LVCFMT_LEFT,90,0);
	m_ActionList.InsertColumn(2,"值",LVCFMT_LEFT,102,0);
	m_bKeyDownState.SetCheck(1);
	m_bKeyDown=1;
	m_bKeyUpState.SetCheck(1);
	m_bKeyUp=1;
	m_ActionList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cMoveMouse.SetCheck(1);
	m_bMoveMouse=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChildDlg::OnCheckMovemouse() 
{
	UpdateData();
	if (m_bMoveMouse)
	{
		m_cPosX.EnableWindow(true);
		m_cPosY.EnableWindow(true);
	}
	else
	{
		m_cPosX.EnableWindow(false);
		m_cPosY.EnableWindow(false);
	}
}

void CChildDlg::OnCheckGetpos() 
{
	UpdateData();
	if(m_bGetPos)
	{
		SetTimer(1,33,NULL);
	}
	else
	{
		KillTimer(1);
	}
}

void CChildDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	POINT lpPoint;	
	GetCursorPos(&lpPoint);
	m_nPosX=lpPoint.x;
	m_nPosY=lpPoint.y;
	UpdateData(false);
	CDialog::OnTimer(nIDEvent);
}

void CChildDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildDlg::OnCheckAlwayrec() 
{
	UpdateData();	
}

void CChildDlg::OnRclickListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu *pActionList;
	pActionList=new CMenu;
	pActionList->CreatePopupMenu();
	*pResult = 0;
}
