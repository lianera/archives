#if !defined(AFX_MINIDLG1_H__A08A4451_ADE3_4458_9E06_85F636E96146__INCLUDED_)
#define AFX_MINIDLG1_H__A08A4451_ADE3_4458_9E06_85F636E96146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiniDlg dialog

class CMiniDlg : public CDialog
{
// Construction
public:
	CMiniDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMiniDlg)
	enum { IDD = IDD_DIALOGBAR };
	CButton	m_Stop;
	CButton	m_Pause;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMiniDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCtrlPause();
	afx_msg void OnCtrlStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIDLG1_H__A08A4451_ADE3_4458_9E06_85F636E96146__INCLUDED_)
