#if !defined(AFX_SOFTWARERGBDLG_H__D35352C8_14AB_4585_961A_4A6BABBE79CD__INCLUDED_)
#define AFX_SOFTWARERGBDLG_H__D35352C8_14AB_4585_961A_4A6BABBE79CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoftwareRGBDlg.h : header file
//

#include "libpdv.h"
#include "edtimage.h"
#include "PdvInput.h"



/////////////////////////////////////////////////////////////////////////////
// CSoftwareRGBDlg dialog

class CWhiteBalanceSetup;

class DLLEXPORT CSoftwareRGBDlg : public CDialog
{
// Construction
public:
	void * m_pUpdateTestTarget;
	void (*m_UpdateTestImage) (void *pTarget);

	int m_nSourceDepth;
	int ComputeWB(EdtImage *pImage);
	CSoftwareRGBDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoftwareRGBDlg();

	void SetCameraValues(int src_depth, int red_row_first, int green_pixel_first);

	int (*m_RequestImage)(void *pTarget, int (*pUpdateImage)(void *, EdtImage *),
			void *pSrc, int once);

	void *m_pRequestTarget;

	EdtImage *m_pSrcTest,
			*m_pDestTest;

	PdvInput *m_pCamera; 

	void SetCamera(PdvInput *pCamera);

	CWhiteBalanceSetup *m_WBDlg;

static 	int UpdateImage(void *pObj, EdtImage *pImage);


// Dialog Data
	//{{AFX_DATA(CSoftwareRGBDlg)
	enum { IDD = IDD_SOFTWARE_RGB };
	CSpinButtonCtrl	m_wndBiasSpin;
	CSliderCtrl	m_wndBiasSlider;
	CSliderCtrl	m_wndEvenRowSlider;
	CSliderCtrl	m_wndOddRowSlider;
	CSliderCtrl	m_wndBlackOffsetSlider;
	CSliderCtrl	m_wndScaleRedSlider;
	CSliderCtrl	m_wndScaleGreenSlider;
	CSliderCtrl	m_wndScaleBlueSlider;
	CSliderCtrl	m_wndGammaSlider;
	BOOL	m_bGreenPixelFirst;
	BOOL	m_bRedRowFirst;
	double	m_dRGBGamma;
	double	m_dScaleBlue;
	double	m_dScaleGreen;
	double	m_dScaleRed;
	int		m_nBlackOffset;
	double	m_dSpeedValue;
	int		m_nQuality;
	double	m_dEvenRowScale;
	double	m_dOddRowScale;
	BOOL	m_bUseGamma;
	int		m_nBias;
	BOOL	m_bGradientColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoftwareRGBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetUseBiasState();
	void SetUseGammaState();
	void EnableTesting(bool on);
	bool m_bShowTest;

	// Generated message map functions
	//{{AFX_MSG(CSoftwareRGBDlg)
	afx_msg void OnApply();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSampleWb();
	afx_msg void OnTestButton();
	afx_msg void OnUseGamma();
	afx_msg void OnQuality1();
	afx_msg void OnQuality0();
	afx_msg void OnQuality2();
	afx_msg void OnWbDetails();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedRedRowFirst();
public:
    afx_msg void OnBnClickedGreenPixelFirst();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOFTWARERGBDLG_H__D35352C8_14AB_4585_961A_4A6BABBE79CD__INCLUDED_)
