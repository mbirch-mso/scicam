#if !defined(AFX_BGSUBTRACTDLG_H__003ADC8F_E494_4B11_B036_81F070D419A3__INCLUDED_)
#define AFX_BGSUBTRACTDLG_H__003ADC8F_E494_4B11_B036_81F070D419A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BGSubtractDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBGSubtractDlg dialog

class CBGSubtractDlg : public CDialog
{
// Construction
public:
	CBGSubtractDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBGSubtractDlg)
	enum { IDD = IDD_BGSUB_DIALOG };
	BOOL	m_bEnableBGImage;
	CString	m_sBackgroundImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBGSubtractDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBGSubtractDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGSUBTRACTDLG_H__003ADC8F_E494_4B11_B036_81F070D419A3__INCLUDED_)
