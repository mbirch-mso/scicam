#if !defined(AFX_EDTIMAGEWINDOW_H__2AA2C883_22B2_11D2_A920_CE16AA0B2C07__INCLUDED_)
#define AFX_EDTIMAGEWINDOW_H__2AA2C883_22B2_11D2_A920_CE16AA0B2C07__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EdtImageWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdtImageWindow window

#include "WinViewImage.h"

enum TrackingOptions {
	NoTracking,
	TrackRect,
	TrackLine
} ;

class DLLEXPORT CEdtImageWindow : public CStatic
{
// Construction
public:
	CEdtImageWindow();

protected:

	EdtImage *	m_pImage;

	EdtImage *	m_pActiveImage; // Subimage

	CRect		m_SubRegion;

	CPoint		m_Point1;
	CPoint		m_Point2;

	BOOL		m_bRectTracking;
	BOOL		m_bLineTracking;

	BOOL		m_bLineActive;
	BOOL		m_bRectActive;
	
// Attributes
public:
	
	BOOL GetEndPoints(CPoint &pt1, CPoint &pt2);

	void SetEndPoints(const CPoint pt1, const CPoint pt2);


	BOOL GetRegion(CRect &region);

	void SetRegion(const CRect region);

	EdtImage *GetImage();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtImageWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	CPoint m_Corner2;
	CPoint m_Corner1;

	BOOL IsRectTracking();
	BOOL IsLineTracking();

	void SetRectTracking(BOOL bState);
	void SetLineTracking(BOOL bState);

	BOOL IsRectActive();
	BOOL IsLineActive();

	void SetRectActive(BOOL bState);
	void SetLineActive(BOOL bState);

	void NotifyMouseAction(UINT message, UINT nFlags, CPoint point);
	BOOL m_bDragging;
	BOOL PtInImage(const CPoint pt);
	EdtImage * GetBaseImage();
	EdtImage * GetSubImage();
	BOOL m_bNotifyParentMouseMove;
	void SetNotifyParentMouseMove(BOOL bState);
	int m_nFrameNumber;
	int m_nSleepValue;
	void UpdateDisplay();
	void SetImageHandle(EdtImage *pImage, LPCTSTR ImageName);
	CWnd * m_wndStatus;
	virtual ~CEdtImageWindow();


protected:
	//{{AFX_MSG(CEdtImageWindow)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTIMAGEWINDOW_H__2AA2C883_22B2_11D2_A920_CE16AA0B2C07__INCLUDED_)
