// Parameter.cpp : implementation file
//

#include "stdafx.h"
#include "¶íÂŞË¹·½¿é.h"
#include "Parameter.h"
#include "Manager.h"
#include "Panel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParameter dialog
extern CManager* pManager;
extern CPanel* pPanel;

CParameter::CParameter(CWnd* pParent /*=NULL*/)
	: CDialog(CParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParameter)
	m_vnPB = 0;
	m_vnPL = 0;
	m_vnPR = 0;
	m_vnPT = 0;
	m_hNumber = 0;
	m_vNumber = 0;
	//}}AFX_DATA_INIT
}


void CParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParameter)
	DDX_Control(pDX, IDC_PANELTOP, m_PanelTop);
	DDX_Control(pDX, IDC_PANELRIGHT, m_PanelRight);
	DDX_Control(pDX, IDC_PANELLEFT, m_PanelLeft);
	DDX_Control(pDX, IDC_PANELBOTTOM, m_PanelBottom);
	DDX_Slider(pDX, IDC_PANELBOTTOM, m_vnPB);
	DDX_Slider(pDX, IDC_PANELLEFT, m_vnPL);
	DDX_Slider(pDX, IDC_PANELRIGHT, m_vnPR);
	DDX_Slider(pDX, IDC_PANELTOP, m_vnPT);
	DDX_Text(pDX, IDC_HNUMBER, m_hNumber);
	DDV_MinMaxInt(pDX, m_hNumber, 1, 90);
	DDX_Text(pDX, IDC_VNUMBER, m_vNumber);
	DDV_MinMaxInt(pDX, m_vNumber, 1, 90);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParameter, CDialog)
	//{{AFX_MSG_MAP(CParameter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameter message handlers

BOOL CParameter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_PanelTop		.SetRange(pManager->ClientRect.top	,pManager->ClientRect.bottom);
	m_PanelBottom	.SetRange(pManager->ClientRect.top	,pManager->ClientRect.bottom);
	m_PanelLeft		.SetRange(pManager->ClientRect.left	,pManager->ClientRect.right);
	m_PanelRight	.SetRange(pManager->ClientRect.left	,pManager->ClientRect.right);
	m_PanelBottom	.SetPos(pPanel->PanelRect.bottom);
	m_PanelTop		.SetPos(pPanel->PanelRect.top);
	m_PanelLeft		.SetPos(pPanel->PanelRect.left);
	m_PanelRight	.SetPos(pPanel->PanelRect.right);

	m_hNumber		=pPanel->nHGridNum;
	m_vNumber		=pPanel->nVGridNum;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CParameter::OnClose() 
{

	CDialog::OnClose();
}

void CParameter::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	pPanel->PanelRect.top	=m_vnPT;
	pPanel->PanelRect.bottom=m_vnPB;
	pPanel->PanelRect.left	=m_vnPL;
	pPanel->PanelRect.right	=m_vnPR;
	pPanel->nHGridNum		=m_hNumber;
	pPanel->nVGridNum		=m_vNumber;
	pPanel->GridSize.cx		=(m_vnPR-m_vnPL)/m_hNumber;
	pPanel->GridSize.cy		=(m_vnPB-m_vnPT)/m_vNumber;
	UpdateData(false);	
	CDialog::OnOK();
}

void CParameter::OnSound() 
{
	pManager->Sound=-pManager->Sound;
}
