#if !defined(AFX_CHILDDLG_H__704DB83B_1FA9_4CD3_8816_58EDCC3EF55E__INCLUDED_)
#define AFX_CHILDDLG_H__704DB83B_1FA9_4CD3_8816_58EDCC3EF55E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildDlg dialog

class CChildDlg : public CDialog
{
// Construction
public:
	CChildDlg(CWnd* pParent = NULL);   // standard constructor
	void CChildDlg::ArrayList();
//	void CChildDlg::AddListData(int flag,long num1,long num2,CString string);

// Dialog Data
	//{{AFX_DATA(CChildDlg)
	enum { IDD = IDD_CHILD };
	CEdit	m_cPosY;
	CEdit	m_cPosX;
	CButton	m_cMoveMouse;
	CButton	m_bKeyUpState;
	CButton	m_bKeyDownState;
	CListCtrl	m_ActionList;
	long	m_lDefer;
	int		m_nPosX;
	int		m_nPosY;
	BOOL	m_bKeyDown;
	BOOL	m_bKeyUp;
	CString	m_strKey;
	long	m_lRepeat;
	BOOL	m_bMoveMouse;
	BOOL	m_bGetPos;
	BOOL	m_bAlwayRec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChildDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonClean();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckMovemouse();
	afx_msg void OnCheckGetpos();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCheckAlwayrec();
	afx_msg void OnRclickListAction(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDDLG_H__704DB83B_1FA9_4CD3_8816_58EDCC3EF55E__INCLUDED_)
