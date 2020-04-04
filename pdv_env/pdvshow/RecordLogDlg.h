#if !defined(AFX_RECORDLOGDLG_H__647EA252_6D0E_11D3_8BC6_00104B357776__INCLUDED_)
#define AFX_RECORDLOGDLG_H__647EA252_6D0E_11D3_8BC6_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RecordLogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordLogDlg dialog

#include "edtdefines.h"

class DLLEXPORT CRecordLogDlg : public CDialog
{
// Construction
public:
	BOOL m_bModified;
	FILE * m_LogFile;
	void ClearBuffer();
	void SaveLogFile();
	void OpenLogFile(const char *pFileName);
	void Timestamp(const char *fmt, ...);
	void AddCommentString(const char *fmt, ...);
	CRecordLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordLogDlg)
	enum { IDD = IDD_LOG_DIALOG };
	CString	m_sLogComment;
	CString	m_sLogFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordLogDlg)
	afx_msg void OnAddComment();
	afx_msg void OnAddTimestamped();
	afx_msg void OnClearLogFile();
	afx_msg void OnSaveLogFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDLOGDLG_H__647EA252_6D0E_11D3_8BC6_00104B357776__INCLUDED_)
