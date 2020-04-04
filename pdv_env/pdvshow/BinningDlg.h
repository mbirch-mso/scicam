#if !defined(AFX_BINNINGDLG_H__2A7FEC13_335F_11D5_96C6_009027D0BF96__INCLUDED_)
#define AFX_BINNINGDLG_H__2A7FEC13_335F_11D5_96C6_009027D0BF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BinningDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBinningDlg dialog

#include "RoiDlg.h"


class DLLEXPORT CBinningDlg : public CDialog
{
// Construction
public:
	CBinningDlg(CWnd* pParent = NULL);   // standard constructor

	PdvInput  *m_pCamera;

	void SetCamera(PdvInput *pCamera)
	{
		m_pCamera = pCamera;
	}

	PdvInput *GetCamera()
	{
		return m_pCamera;
	}

	void UpdateCameraState();

	RoiFireEventFunc m_pFire;
	void *m_pFireTarget;


	void AttachTarget(RoiFireEventFunc pFire, 
		void *pTarget)
	{
		m_pFire = pFire;
		m_pFireTarget = pTarget;
	}

	void UpdateFromCamera(PdvInput * pCamera = NULL);

// Dialog Data
	//{{AFX_DATA(CBinningDlg)
	enum { IDD = IDD_BINNING_DIALOG };
	int		m_nHbin;
	int		m_nVbin;
	int		m_nHeight;
	int		m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBinningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBinningDlg)
	afx_msg void OnApply();
	virtual void OnOK();
	afx_msg void OnHbin1();
	afx_msg void OnHbin2();
	afx_msg void OnHbin4();
	afx_msg void OnHbin8();
	afx_msg void OnVbin1();
	afx_msg void OnVbin2();
	afx_msg void OnVbin4();
	afx_msg void OnVbin8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINNINGDLG_H__2A7FEC13_335F_11D5_96C6_009027D0BF96__INCLUDED_)
