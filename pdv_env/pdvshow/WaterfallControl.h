#if !defined(AFX_WATERFALLCONTROL_H__0D90F035_14BE_11D4_B843_003048100300__INCLUDED_)
#define AFX_WATERFALLCONTROL_H__0D90F035_14BE_11D4_B843_003048100300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaterfallControl.h : header file
//

#include "PdvInput.h"
//#include "ImagePumpDisplay.h"


/////////////////////////////////////////////////////////////////////////////
// CWaterfallControl dialog

class DLLEXPORT CWaterfallControl : public CDialog
{
// Construction
public:
	EdtImage * m_pSrcImage;
	PdvInput * m_pCamera;
	ImageWindowData * m_pDisplay;
	CWaterfallControl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaterfallControl)
	enum { IDD = IDD_WATERFALL_CONTROL };
	CSpinButtonCtrl	m_wndDecimateSpin;
	CEdit	m_wndRow1;
	CEdit	m_wndRow2;
	CSpinButtonCtrl	m_wndWidthSpin;
	CSpinButtonCtrl	m_wndRow2Spin;
	CSpinButtonCtrl	m_wndRow1Spin;
	CSpinButtonCtrl	m_wndRow0Spin;
	CSpinButtonCtrl	m_wndHeightSpin;
	int		m_nHeight;
	int		m_nRow0;
	int		m_nRow1;
	int		m_nRow2;
	BOOL	m_bUseRgbOut;
	int		m_nWidth;
	BOOL	m_bWidthFromCamera;
	BOOL	m_bActivateWaterfall;
	int		m_nDecimate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterfallControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaterfallControl)
	afx_msg void OnApply();
	virtual void OnOK();
	afx_msg void OnUseRgbOut();
	afx_msg void OnWidthFromCamera();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERFALLCONTROL_H__0D90F035_14BE_11D4_B843_003048100300__INCLUDED_)
