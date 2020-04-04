#if !defined(AFX_DALSACAMERACONTROL_H__C47C9CFE_2CD1_11D4_B86C_003048100300__INCLUDED_)
#define AFX_DALSACAMERACONTROL_H__C47C9CFE_2CD1_11D4_B86C_003048100300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DalsaCameraControl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDalsaCameraControl dialog

class DLLEXPORT CDalsaCameraControl : public CDialog
{
// Construction
public:
	void SetCamera(PdvInput *pCamera);
	double m_dInactiveTime;
	void UpdateSecondaryValues();
	void GetCurrentValues();
	PdvInput * m_pCamera;
	CDalsaCameraControl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDalsaCameraControl)
	enum { IDD = IDD_CAM_CONTROL_DALSA_LS };
	CSliderCtrl	m_wndTotalTimeScroll;
	CSliderCtrl	m_wndExposeScroll;
	int		m_nExposePercent;
	int 	m_nLineRate;
	double	m_dExposeTime;
	double	m_dTotalTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDalsaCameraControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDalsaCameraControl)
	afx_msg void OnApply();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DALSACAMERACONTROL_H__C47C9CFE_2CD1_11D4_B86C_003048100300__INCLUDED_)
