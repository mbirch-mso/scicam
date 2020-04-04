#if !defined(AFX_GAMMADIALOG_H__CCD934A4_473A_11D2_9813_00104B357776__INCLUDED_)
#define AFX_GAMMADIALOG_H__CCD934A4_473A_11D2_9813_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GammaDialog.h : header file
//
#include "Edtimage.h"

/////////////////////////////////////////////////////////////////////////////
// CGammaDialog dialog

class DLLEXPORT CGammaDialog : public CDialog
{
// Construction
public:
	CGammaDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGammaDialog)
	enum { IDD = IDD_GAMMA_DIALOG };
	CSliderCtrl	m_wndGammaSlider;
	double	m_dGammaValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGammaDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGammaDialog)
	afx_msg void OnSetGamma();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMMADIALOG_H__CCD934A4_473A_11D2_9813_00104B357776__INCLUDED_)
