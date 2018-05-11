#if !defined(AFX_PARAMETER_H__4B27CAB0_992F_43BB_BCCC_B37D604C8B55__INCLUDED_)
#define AFX_PARAMETER_H__4B27CAB0_992F_43BB_BCCC_B37D604C8B55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Parameter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParameter dialog

class CParameter : public CDialog
{
// Construction
public:
	CParameter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParameter)
	enum { IDD = IDD_PARAMETER };
	CSliderCtrl	m_PanelTop;
	CSliderCtrl	m_PanelRight;
	CSliderCtrl	m_PanelLeft;
	CSliderCtrl	m_PanelBottom;
	int		m_vnPB;
	int		m_vnPL;
	int		m_vnPR;
	int		m_vnPT;
	int		m_hNumber;
	int		m_vNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParameter)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParameter)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETER_H__4B27CAB0_992F_43BB_BCCC_B37D604C8B55__INCLUDED_)
