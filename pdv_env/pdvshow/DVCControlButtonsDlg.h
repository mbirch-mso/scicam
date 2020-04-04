#if !defined(AFX_DVCCONTROLBUTTONSDLG_H__D6CA8598_1526_4956_B5BC_22A4A914B12F__INCLUDED_)
#define AFX_DVCCONTROLBUTTONSDLG_H__D6CA8598_1526_4956_B5BC_22A4A914B12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DVCControlButtonsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDVCControlButtonsDlg dialog

class CDVCControlButtonsDlg : public CDialog
{
// Construction
public:
	CDVCControlButtonsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDVCControlButtonsDlg)
	enum { IDD = IDD_DVC_BUTTONS_DLG };
	CString	m_sLabel1;
	CString	m_sLabel2;
	CString	m_sLabel3;
	CString	m_sLabel4;
	CString	m_sLabel5;
	CString	m_sLabel6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVCControlButtonsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDVCControlButtonsDlg)
	afx_msg void OnExamine1();
	afx_msg void OnExamine2();
	afx_msg void OnExamine3();
	afx_msg void OnExamine4();
	afx_msg void OnExamine5();
	afx_msg void OnExamine6();
	afx_msg void OnLoadButton1();
	afx_msg void OnLoadButton2();
	afx_msg void OnLoadButton3();
	afx_msg void OnLoadButton4();
	afx_msg void OnLoadButton5();
	afx_msg void OnLoadButton6();
	afx_msg void OnSaveCurrent1();
	afx_msg void OnSaveCurrent2();
	afx_msg void OnSaveCurrent3();
	afx_msg void OnSaveCurrent4();
	afx_msg void OnSaveCurrent5();
	afx_msg void OnSaveCurrent6();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVCCONTROLBUTTONSDLG_H__D6CA8598_1526_4956_B5BC_22A4A914B12F__INCLUDED_)
