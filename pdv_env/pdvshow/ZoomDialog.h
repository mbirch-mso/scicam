#if !defined(AFX_ZOOMDIALOG_H__4D809462_7C1A_11D3_8BD3_00104B357776__INCLUDED_)
#define AFX_ZOOMDIALOG_H__4D809462_7C1A_11D3_8BD3_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ZoomDialog.h : header file
//

#include "EdtImageWin.h"



/////////////////////////////////////////////////////////////////////////////
// CZoomDialog dialog

class DLLEXPORT CZoomDialog : public CDialog
{
// Construction
public:
	void UpdateToWindow();
	void UpdateFromWindow();
	CEdtImageWin * m_pAttachedWindow;
	void SetWindow(CEdtImageWin *pwindow);
	CEdtImageWin * GetWindow()
	{
		return m_pAttachedWindow;
	}

	CZoomDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CZoomDialog)
	enum { IDD = IDD_ZOOM_DIALOG };
	CSpinButtonCtrl	m_wndSpinZoomScale;
	CSpinButtonCtrl	m_wndSpinCornerY;
	CSpinButtonCtrl	m_wndSpinXCorner;
	int		m_nXCorner;
	int		m_nYCorner;
	double	m_dXScale;
	double	m_dYScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoomDialog)
	afx_msg void OnUpdateXCorner();
	afx_msg void OnKillfocusXCorner();
	afx_msg void OnKillfocusYCorner();
	afx_msg void OnUpdateYCorner();
	afx_msg void OnKillfocusYScale();
	afx_msg void OnUpdateYScale();
	afx_msg void OnKillfocusXScale();
	afx_msg void OnUpdateXScale();
	afx_msg void OnClose();
	afx_msg void OnDeltaposSpinZoomScale(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMDIALOG_H__4D809462_7C1A_11D3_8BD3_00104B357776__INCLUDED_)
