#if !defined(AFX_DVCCONTROLDLG_H__3F4590A7_74BF_44BB_8541_81E3979C4C2E__INCLUDED_)
#define AFX_DVCCONTROLDLG_H__3F4590A7_74BF_44BB_8541_81E3979C4C2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DVCControlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDVCControlDlg dialog

#include "SoftwareRGBDlg.h"

#include "PdvInput.h"

//#include "DVCSetupDlg.h"
#include "CamSelectDialog.h"

#include "RoiDlg.h"

class DLLEXPORT CDVCControlDlg : public CDialog
{
// Construction
public:
	BOOL m_bGoing;
	void StopIfGoing();
	void SetSpeed();
	void UpdateBinValue();

	//CDVCSetupDlg m_SetupDlg;

	void SetOffset();
	void SetExposure();
	void SetGain();
	void SetCamera(PdvInput *pCamera);
	void SetRGBDlg(CSoftwareRGBDlg *pRGB)
	{
		m_pRGBDlg = pRGB;
	}

	double ScreenGain(const int nGain);
	double ScreenOffset(const int nOffset);
	double ScreenExposure(const int nExposure);

	int CameraGain(const double dGain);
	int CameraOffset(const double dOffset);
	int CameraExposure(const double dExposure);


	RoiFireEventFunc m_pFire;
	RoiFireEventFunc m_pLive;
	RoiFireEventFunc m_pGrab;
	RoiFireEventFunc m_pIsLive;

	void *m_pFireTarget;

	void AttachTarget(RoiFireEventFunc pFire, 
		RoiFireEventFunc pLive, 
		RoiFireEventFunc pGrab, 		
		RoiFireEventFunc pIsLive, 		
		void *pTarget)
	{
		m_pFire = pFire;
		m_pLive = pLive;
		m_pGrab = pGrab;
		m_pFireTarget = pTarget;
		m_pIsLive = pIsLive;
	}
	
	// These allow stopping and starting 

	void StartIfGoing();

	void BuildCameraList();
	void ClearCameraList();

	CPtrArray m_CameraList;

	PdvInput * m_pCamera;
	int m_nExposure;
	int m_nOffset;
	int m_nGain;
	CSoftwareRGBDlg *m_pRGBDlg;

	void UpdateFromCamera();

	CDVCControlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDVCControlDlg)
	enum { IDD = IDD_DVC_CONTROL };
	CSliderCtrl	m_wndExposureSlider;
	CSliderCtrl	m_wndOffsetSlider;
	CSliderCtrl	m_wndGainSlider;
	CSpinButtonCtrl	m_wndOffsetSpin;
	CSpinButtonCtrl	m_wndGainSpin;
	CSpinButtonCtrl	m_wndExposureSpin;
	CString	m_sGainValue;
	CString	m_sOffsetValue;
	CString	m_sExposureValue;
	BOOL	m_bConnect;
	CComboBox	m_wndModeCombo;
	CComboBox	m_wndCameraCombo;
	CComboBox	m_wndBinningCombo;
	CString	m_sBinning;
	CString	m_sCamera;
	CString	m_sMode;
	BOOL	m_bRoiEnabled;
	int		m_nRoiBottom;
	int		m_nRoiTop;
	int		m_nSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVCControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDVCControlDlg)
	afx_msg void OnSetupButton();
	afx_msg void OnWbButton();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposOffsetSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposGainSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposExposeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeBinningCombo();
	afx_msg void OnSelchangeCameraCombo();
	afx_msg void OnSelchangeModeCombo();
	afx_msg void OnOpenCamera();
	afx_msg void OnApplyBinning();
	afx_msg void OnApplyCameraMode();
	afx_msg void OnRefreshButton();
	afx_msg void OnEnableRoi();
	afx_msg void OnSpeed1();
	afx_msg void OnSpeed2();
	afx_msg void OnSpeed4();
	afx_msg void OnSpeed8();
	afx_msg void OnGrabButton();
	afx_msg void OnLiveButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVCCONTROLDLG_H__3F4590A7_74BF_44BB_8541_81E3979C4C2E__INCLUDED_)
