#if !defined(AFX_DISPLAYSKIPDLG_H__C0ABEF51_6EF0_4536_82BF_8471CA0B0D82__INCLUDED_)
#define AFX_DISPLAYSKIPDLG_H__C0ABEF51_6EF0_4536_82BF_8471CA0B0D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplaySkipDlg.h : header file
//

#include "edtdefines.h"

typedef void (*DisplayFireEventFunc)(void *pTarget, int value);

/////////////////////////////////////////////////////////////////////////////
// CDisplaySkipDlg dialog

class DLLEXPORT CDisplaySkipDlg : public CDialog
{
// Construction
public:
	void UpdateOwner();
	void * m_pNotifyTarget;
	DisplayFireEventFunc m_pNotifier;

	void SetOwnerNotification(void *target, DisplayFireEventFunc notify)
	{
		m_pNotifyTarget = target;
		m_pNotifier = notify;
	}

	CDisplaySkipDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDisplaySkipDlg)
	enum { IDD = IDD_DISPLAY_SKIP_DLG };
	CSliderCtrl	m_wndDisplaySkipSlider;
	CSpinButtonCtrl	m_wndDisplaySkipSpin;
	int		m_nDisplaySkip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplaySkipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplaySkipDlg)
	afx_msg void OnUpdateDisplaySkipValue();
	afx_msg void OnKillfocusDisplaySkipValue();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDisplaySkipSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYSKIPDLG_H__C0ABEF51_6EF0_4536_82BF_8471CA0B0D82__INCLUDED_)
