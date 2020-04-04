// CCamSaveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamSaveDlg dialog

#ifndef dlg_auto_h
#define dlg_auto_h

#include "edtdefines.h"

class DLLEXPORT CCamSaveDlg : public CDialog
{
// Construction
public:
	CCamSaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	BOOL	m_bInitialized;

	//{{AFX_DATA(CCamSaveDlg)
	enum { IDD = IDD_CAM_CAPTURE };
	CEdit	m_ctlCaptureInterval;
	CButton	m_ctlCaptureMode;
	CEdit	m_ctlCaptureCount;
	CEdit	m_ctlCaptureIndex;
	CEdit	m_ctlCaptureFile;
	CEdit	m_ctlCaptureDirectory;
	CString	m_strCaptureDirectory;
	CString	m_strCaptureFile;
	int		m_nCaptureIndex;
	BOOL	m_bCaptureMode;
	int		m_nCaptureCount;
	int		m_nCaptureInterval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamSaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL dirstatUNC(CString&);
	BOOL ValidateDirectory(BOOL, CString&);

	// Generated message map functions
	//{{AFX_MSG(CCamSaveDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBrowseDirectories();
	afx_msg void OnChangeCaptureCount();
	afx_msg void OnChangeCaptureDirectory();
	afx_msg void OnChangeCaptureFile();
	afx_msg void OnChangeCaptureIndex();
	afx_msg void OnCaptureMode();
	afx_msg void OnChangeCaptureInterval();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
