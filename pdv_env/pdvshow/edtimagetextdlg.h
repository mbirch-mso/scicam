
#if !defined(AFX_DLGPIXELTEXT_H__4F3431F6_2AE9_11D2_8DEF_00A0C932D513__INCLUDED_)
#define AFX_DLGPIXELTEXT_H__4F3431F6_2AE9_11D2_8DEF_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPixelText.h : header file
//


#include "EdtImageText.h"

/////////////////////////////////////////////////////////////////////////////
// CEdtImageTextDlg dialog

class DLLEXPORT CEdtImageTextDlg : public CDialog
{
// Construction
public:
	EdtImage * m_pImage;
	CEdtImageTextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEdtImageTextDlg)
	enum { IDD = IDD_PIXELTEXT };
	CEdtImageText	m_wndImageText;
	int		m_nStartX;
	int		m_nStartY;
	BOOL	m_bFollowCursor;
    BOOL    m_nFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtImageTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEdtImageTextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFollowCursor();
	afx_msg void OnGotoxy();
	afx_msg void OnDecimal();
	afx_msg void OnHexidecimal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPIXELTEXT_H__4F3431F6_2AE9_11D2_8DEF_00A0C932D513__INCLUDED_)
