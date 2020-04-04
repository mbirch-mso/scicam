// dlg_set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamSetupDlg dialog

#include "edtdefines.h"


class DLLEXPORT CCamSetupDlg : public CDialog
{
// Construction
public:
	CPtrArray * m_pCamList;
	CCamSetupDlg(CWnd* pParent = NULL, LPCSTR szInitStr = NULL, CPtrArray *pList = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamSetupDlg)
	enum { IDD = IDD_CAM_SETUP };
	CListBox	m_ctlCamList;
	CString	m_strCamType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusCamlist();
	afx_msg void OnSelchangeCamlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
