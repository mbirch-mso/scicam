#if !defined(AFX_DVCSETUPDLG_H__C7A02E7E_95EA_4F7A_9DC1_92381D5F242E__INCLUDED_)
#define AFX_DVCSETUPDLG_H__C7A02E7E_95EA_4F7A_9DC1_92381D5F242E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DVCSetupDlg.h : header file
//

#include "CamSelectDialog.h"

#include "RoiDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDVCSetupDlg dialog

class CDVCControlDlg;

class CDVCSetupDlg : public CDialog
{
// Construction
public:

	CDVCControlDlg * m_ControlDlg;

	void SetControlDlg (CDVCControlDlg * pDlg)
	{
		m_ControlDlg = pDlg;
	}

	void SetCamera(PdvInput *pCamera)
	{
		m_pCamera = pCamera;
	}

	PdvInput *GetCamera()
	{
		return m_pCamera;
	}

	void UpdateFromCamera(PdvInput *pCamera = NULL);

	RoiFireEventFunc m_pFire;
	void *m_pFireTarget;

	void AttachTarget(RoiFireEventFunc pFire, 
		void *pTarget)
	{
		m_pFire = pFire;
		m_pFireTarget = pTarget;
	}

	void BuildCameraList();
	void ClearCameraList();

	CPtrArray m_CameraList;

	PdvInput * m_pCamera;
	CDVCSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDVCSetupDlg)
	enum { IDD = IDD_DVC_SETUP_DIALOG };
	CComboBox	m_wndModeCombo;
	CComboBox	m_wndCameraCombo;
	CComboBox	m_wndBinningCombo;
	CString	m_sBinning;
	CString	m_sCamera;
	CString	m_sMode;
	CString	m_nHeight;
	CString	m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVCSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDVCSetupDlg)
	afx_msg void OnSelchangeBinningCombo();
	afx_msg void OnSelchangeCameraCombo();
	afx_msg void OnSelchangeModeCombo();
	afx_msg void OnOpenCamera();
	afx_msg void OnApplyBinning();
	afx_msg void OnApplyCameraMode();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVCSETUPDLG_H__C7A02E7E_95EA_4F7A_9DC1_92381D5F242E__INCLUDED_)
