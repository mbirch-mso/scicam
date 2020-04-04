#if !defined(AFX_EDTIMAGEFRAME_H__3A5BF9FD_108E_11D4_B841_003048100300__INCLUDED_)
#define AFX_EDTIMAGEFRAME_H__3A5BF9FD_108E_11D4_B841_003048100300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdtImageFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdtImageFrame frame

#include "EdtImageWin.h"


class DLLEXPORT CEdtImageFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CEdtImageFrame)

// Attributes
public:
	CEdtImageFrame();           // protected constructor used by dynamic creation
	virtual ~CEdtImageFrame();

	CEdtImageWin *m_pImageWindow;

// Operations
public:
	static CEdtImageFrame * NewImageViewer(const char *label, EdtImage *pImage, CRect *pBox = NULL);

	void CreateImageViewer(const char *label, EdtImage *pImage, CRect *pBox = NULL);

	int m_nX, m_nY;
	CString m_sPixelValue;
	CString m_sStatus;

	double m_dZoom;

	void UpdateXYPixelZoom();

	CStatusBar m_wndStatusBar;
	BOOL m_bStatusBar;
	BOOL m_bValueSet;
	CRect m_LastPosition;
	CRect m_StatusRect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtImageFrame)
	public:
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:

	afx_msg void OnUpdateIndicatorZoom(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorX(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorY(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorPixelValue(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorStatus(CCmdUI * pCmdUI);

	// Generated message map functions
	//{{AFX_MSG(CEdtImageFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTIMAGEFRAME_H__3A5BF9FD_108E_11D4_B841_003048100300__INCLUDED_)
