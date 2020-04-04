#if !defined(AFX_CONTRASTDLG_H__B7DF20F4_1B2B_11D2_8DE4_00A0C932D513__INCLUDED_)
#define AFX_CONTRASTDLG_H__B7DF20F4_1B2B_11D2_8DE4_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ContrastDlg.h : header file
//

#include "Edtimage.h"

#include "EdtImageWin.h"

/////////////////////////////////////////////////////////////////////////////
// CContrastDlg dialog

class PdvInput ;
class DLLEXPORT CContrastDlg : public CDialog
{
// Construction
public:
	void SetMapType(int nMapType);
	EdtImage * GetCurrentImage();
	void RefreshImageValues();
	BOOL OnInitDialog();
	void RefreshLowHigh();

	CEdtImageWin  * m_pImageWindow;
	void SetImageWindow(CEdtImageWin *pWindow);

	CContrastDlg(CWnd* pParent = NULL);   // standard constructor

	PdvInput *m_pCamera;

	void SetCamera(PdvInput *pCamera)
	{
		m_pCamera = pCamera;
	}

// Dialog Data
	//{{AFX_DATA(CContrastDlg)
	enum { IDD = IDD_CONTRAST };
	CSliderCtrl	m_wndGammaSlider;
	CButton	m_wndGreenButton;
	CButton	m_wndBlueButton;
	CButton	m_wndRedButton;
	CSliderCtrl	m_wndLowSlider;
	CSliderCtrl	m_wndHighSlider;
	int	m_nHighValue;
	int	m_nLowValue;
	int		m_nMapType;
	int		m_nColor;
	double	m_dGammaValue;
	int		m_nLutMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContrastDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContrastDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnAutoContrast();
	afx_msg void OnRawDisplay();
	afx_msg void OnApplyToLive();
	afx_msg void OnResetLive();
	afx_msg void OnMapMode();
	afx_msg void OnDoHeq();
	afx_msg void OnUserSet();
	afx_msg void OnSqrt();
	afx_msg void OnAutoLinear();
	afx_msg void OnHeq();
	afx_msg void OnRgbBlue();
	afx_msg void OnRgbGreen();
	afx_msg void OnRgbRed();
	afx_msg void OnGamma();
	afx_msg void OnInfrared();
	afx_msg void OnGrayscale();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTRASTDLG_H__B7DF20F4_1B2B_11D2_8DE4_00A0C932D513__INCLUDED_)
