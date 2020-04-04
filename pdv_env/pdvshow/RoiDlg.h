#if !defined(AFX_ROIDLG_H__C2F38F41_E0A2_11D3_80F7_009027D0BF96__INCLUDED_)
#define AFX_ROIDLG_H__C2F38F41_E0A2_11D3_80F7_009027D0BF96__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RoiDlg.h : header file
//

#include "PdvInput.h"

/////////////////////////////////////////////////////////////////////////////
// CRoiDlg dialog

typedef int (*RoiFireEventFunc)(void *pTarget);

class DLLEXPORT CRoiDlg : public CDialog
{
// Construction
public:
	void ApplyToCamera();
	int m_nHeight;
	int m_nWidth;
	void UpdateFromCamera(PdvInput * pCamera = NULL);
	CRoiDlg(CWnd* pParent = NULL);   // standard constructor

	PdvInput *m_pCamera;

	RoiFireEventFunc m_pFire;
	void *m_pFireTarget;


	void AttachTarget(RoiFireEventFunc pFire, 
		void *pTarget)
	{
		m_pFire = pFire;
		m_pFireTarget = pTarget;
	}

		
	// Dialog Data
	//{{AFX_DATA(CRoiDlg)
	enum { IDD = IDD_ROI_DIALOG };
	CSpinButtonCtrl	m_wndVSkipSpin;
	CSpinButtonCtrl	m_wndVActvSpin;
	CSpinButtonCtrl	m_wndHActvSpin;
	CSpinButtonCtrl	m_wndHSkipSpin;
	CSliderCtrl	m_wndHActvSlider;
	CSliderCtrl	m_wndHSkipSlider;
	CSliderCtrl	m_wndVActvSlider;
	CSliderCtrl	m_wndVSkipSlider;
	CString	m_sHActvMax;
	int		m_nHActv;
	int		m_nHSkip;
	CString	m_sHSkipMax;
	int		m_nVActv;
	CString	m_sVActvMax;
	int		m_nVSkip;
	CString	m_sVSkipMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoiDlg)
	afx_msg void OnApply();
	afx_msg void OnClose();
	afx_msg void OnKillfocusHactvEdit();
	afx_msg void OnKillfocusHskipEdit();
	afx_msg void OnKillfocusVactvEdit();
	afx_msg void OnKillfocusVskipEdit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposHactvSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposHskipSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposVactvSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposVskipSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROIDLG_H__C2F38F41_E0A2_11D3_80F7_009027D0BF96__INCLUDED_)
