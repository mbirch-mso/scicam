#if !defined(AFX_EDTIMAGETEXT_H__7E2B5355_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
#define AFX_EDTIMAGETEXT_H__7E2B5355_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EdtImageText.h : header file
//

#include "EdtImage.h"

/////////////////////////////////////////////////////////////////////////////
// CEdtImageText window

class DLLEXPORT CEdtImageText : public CWnd
{
// Construction
public:
	CEdtImageText();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtImageText)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_borderWidth;
	int m_nColumns;
	int m_nRows;
	EdtImage * m_pImage;
	void RefreshData();
	void SetCenterPoint(CPoint point);
	void SetImageHandle(EdtImage *pImage);
	void SetDecimal(bool state);
	long m_nCenterY;
	long m_nCenterX;
    bool m_bDecimal;
	virtual ~CEdtImageText();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEdtImageText)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CEdtImageText)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTIMAGETEXT_H__7E2B5355_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
