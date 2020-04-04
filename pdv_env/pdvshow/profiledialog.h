#if !defined(AFX_PROFILEDIALOG_H__D5C9EC54_2BB2_11D2_97D9_00104B357776__INCLUDED_)
#define AFX_PROFILEDIALOG_H__D5C9EC54_2BB2_11D2_97D9_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProfileDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProfileDialog dialog

#include "EdtImgProfile.h"
#include "EdtPlotWindow.h"


class DLLEXPORT CProfileDialog : public CDialog
{
// Construction
public:
	LRESULT OnPlotButton(WPARAM wparam, LPARAM lparam);
	BOOL m_bVisible;
	void Compute(EdtImage *pImage, CPoint pt1, CPoint pt2);
	EdtImgProfile m_Profile;
	void SetPoints(CPoint point1, CPoint point2);
	void Compute(EdtImage *pImage = NULL);
	EdtImage * m_pImage;
	CProfileDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProfileDialog)
	enum { IDD = IDD_PROFILE_DIALOG };
	CStatic	m_wndCursorValues;
	CEdit	m_wndY2;
	CEdit	m_wndY1;
	CEdit	m_wndX2;
	CEdit	m_wndX1;
	CEdtPlotWindow	m_wndProfileCntl;
	int		m_nX1;
	int		m_nX2;
	int		m_nY1;
	int		m_nY2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfileDialog)
	afx_msg void OnComputeButton();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEDIALOG_H__D5C9EC54_2BB2_11D2_97D9_00104B357776__INCLUDED_)
