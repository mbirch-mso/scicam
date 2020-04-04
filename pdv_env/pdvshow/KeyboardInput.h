#if !defined(AFX_KEYBOARDINPUT_H__D125C5A6_2B3D_11D4_B86A_003048100300__INCLUDED_)
#define AFX_KEYBOARDINPUT_H__D125C5A6_2B3D_11D4_B86A_003048100300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyboardInput window

class DLLEXPORT CKeyboardInput : public CWnd
{
// Construction
public:
	CKeyboardInput();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboardInput)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKeyboardInput();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKeyboardInput)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDINPUT_H__D125C5A6_2B3D_11D4_B86A_003048100300__INCLUDED_)
