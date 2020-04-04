// pdvshowView.h : interface of the CPdvshowView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDVSHOWVIEW_H__7E2B5368_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
#define AFX_PDVSHOWVIEW_H__7E2B5368_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "EdtImageWin.h"

class CPdvshowView : public CView
{
protected: // create from serialization only
	CPdvshowView();
	DECLARE_DYNCREATE(CPdvshowView)

// Attributes
public:
	CPdvshowDoc* GetDocument();

	CEdtImageWin m_wndImageWindow; // to actually draw the image


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdvshowView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(bool bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SizeToImage();
	virtual ~CPdvshowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	LRESULT OnZoomStatusUpdate(WPARAM wParam, LPARAM lparam);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPdvshowView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in pdvshowView.cpp
inline CPdvshowDoc* CPdvshowView::GetDocument()
   { return (CPdvshowDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDVSHOWVIEW_H__7E2B5368_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
