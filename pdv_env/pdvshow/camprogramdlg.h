#if !defined(AFX_DLG_PROG_H__644F3641_3771_11D1_9FEF_444553540000__INCLUDED_)
#define AFX_DLG_PROG_H__644F3641_3771_11D1_9FEF_444553540000__INCLUDED_

//#if _MSC_VER >= 1000
//#pragma once
//#endif

// _MSC_VER >= 1000
// CCamProgramDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamProgramDlg dialog

#include "edtdefines.h"

class PdvInput;


class DLLEXPORT CCamProgramDlg : public CDialog
{
// Construction
public:
	void SetSerialFormatButton(int);
	CCamProgramDlg(CWnd* pParent = NULL);   // standard constructor

	PdvInput *m_pCamera;

	void SetCamera(PdvInput *pCamera)
	{
		m_pCamera = pCamera;
	}

	void AppendText(const char *s);

// Dialog Data
	//{{AFX_DATA(CCamProgramDlg)
	enum { IDD = IDD_CAM_PROGRAM };
	int		m_nIshex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamProgramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamProgramDlg)
	afx_msg void OnChangeCamInput();
	afx_msg void OnKillfocusCamInput();
	afx_msg void OnMaxtextCamInput();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_PROG_H__644F3641_3771_11D1_9FEF_444553540000__INCLUDED_)
