// EdtImageWindow.cpp : implementation file
//

#include "stdafx.h"

#include "EdtImageWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdtImageWindow

CEdtImageWindow::CEdtImageWindow()
{
	m_wndStatus = NULL;
	m_pImage = NULL;
	m_pActiveImage = NULL;

	m_nSleepValue = 1;
	m_nFrameNumber = 0;

	m_bLineTracking = FALSE;
	m_bLineActive = FALSE;

	m_bRectTracking = FALSE;
	m_bRectActive = FALSE;

	m_SubRegion = CRect(0,0,0,0);

	m_Point1 = CPoint(0,0);
	m_Point2 = CPoint(0,0);

	m_Corner1 = CPoint(0,0);
	m_Corner2 = CPoint(0,0);

	m_bDragging = FALSE;

	m_bNotifyParentMouseMove = FALSE;
}

CEdtImageWindow::~CEdtImageWindow()
{
	if (m_pActiveImage)
	{
		delete m_pActiveImage;
		m_pActiveImage = NULL;
	}
}

//	ON_WM_MOUSEMOVE()

BEGIN_MESSAGE_MAP(CEdtImageWindow, CStatic)
	//{{AFX_MSG_MAP(CEdtImageWindow)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtImageWindow message handlers

void CEdtImageWindow::OnMouseMove(UINT nFlags, CPoint rawpoint) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPoint point = rawpoint;

	CClientDC dc(this);
	dc.DPtoLP(&point);

	if (PtInImage(point))
	{
		if (m_bLineTracking && m_bDragging)
		{
			
		// Draw a line inverting

			dc.SetROP2(R2_NOT);
			
			if (m_Point1 !=m_Point2)
			{

				dc.MoveTo(m_Point1);
				dc.LineTo(m_Point2);
			}

			m_Point2 = point;

			dc.MoveTo(m_Point1);
			dc.LineTo(m_Point2);

		} 
		if (m_bRectTracking && m_bDragging)
		{
			CRect lastrect = m_SubRegion;

			m_Corner2 = point;

			m_SubRegion = CRect(m_Corner1, m_Corner2);
			m_SubRegion.NormalizeRect();

			dc.DrawDragRect(&m_SubRegion,CSize(2,2),&lastrect,CSize(2,2),NULL);

		}

		if (m_wndStatus)
		{
			CString s;

			if (m_pImage && point.x <= m_pImage->GetWidth() &&
				point.y <= m_pImage->GetHeight()) 
			{
				long Value = 0;
				m_pImage->GetPixel(point.x, point.y, Value);

				if (m_pImage->GetNColors() == 1)
					s.Format("(%d,%d) = %d", point.x, point.y, Value);
				else
					s.Format("(%d,%d) = (%d,%d,%d)", point.x, point.y, 
						GetRValue(Value), GetGValue(Value), GetBValue(Value));

			}
			else
			{
				s.Format("(%d,%d)",point.x, point.y);
			}

			m_wndStatus->SetWindowText(s);

		}

		if (m_bNotifyParentMouseMove)
		{
			NotifyMouseAction(WM_MOUSEMOVE, nFlags, rawpoint);
		}

	}
	CStatic::OnMouseMove(nFlags, point);
}

void CEdtImageWindow::OnLButtonDown(UINT nFlags, CPoint rawpoint) 
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here and/or call default
	
	CPoint point = rawpoint;

	CClientDC dc(this);
	dc.DPtoLP(&point);

	if (PtInImage(point))
	{
		// Draw a line inverting
		
		dc.SetROP2(R2_NOT);

		if (m_bLineTracking)
		{
			
		// Draw a line inverting

			
			if (m_Point1 !=m_Point2)
			{

				dc.MoveTo(m_Point1);
				dc.LineTo(m_Point2);
			}

			m_Point1 = m_Point2 = point;

		} 
		if (m_bRectTracking)
		{
			
			if (!m_SubRegion.IsRectEmpty())
			{
				dc.DrawDragRect(&m_SubRegion,CSize(2,2),NULL,CSize(2,2),NULL);
			}
			// Create the subimage
			m_SubRegion = CRect(point,point);
			m_Corner1 = m_Corner2 = point;

		}

		m_bDragging = TRUE;

	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CEdtImageWindow::OnLButtonUp(UINT nFlags, CPoint rawpoint) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPoint point = rawpoint;

	CClientDC dc(this);
	dc.DPtoLP(&point);

	if (PtInImage(point) && m_bDragging)
	{
		// Draw a line inverting
		
		dc.SetROP2(R2_NOT);

		if (m_bLineTracking)
		{
			
		// Draw a line inverting

			
			if (m_Point1 !=m_Point2)
			{

				dc.MoveTo(m_Point1);
				dc.LineTo(m_Point2);
			}

			m_Point2 = point;

			dc.MoveTo(m_Point1);
			dc.LineTo(m_Point2);

		} 
		if (m_bRectTracking)
		{
			
			CRect lastrect = m_SubRegion;

			m_Corner2 = point;

			m_SubRegion = CRect(m_Corner1, m_Corner2);
			m_SubRegion.NormalizeRect();

			dc.DrawDragRect(&m_SubRegion,CSize(2,2),&lastrect,CSize(2,2),NULL);

			// Create the subimage

			if (!m_SubRegion.IsRectEmpty())
			{
				if (!m_pActiveImage)
					m_pActiveImage =  MainWindowFactory->NewDisplayableImage(m_pImage->GetType());

				m_pActiveImage->AttachToParent(m_pImage,
					m_SubRegion.left, m_SubRegion.top,
					m_SubRegion.Width(), m_SubRegion.Height());

			}
		}
		

		UpdateDisplay();

		if (m_bNotifyParentMouseMove)
		{
			NotifyMouseAction(WM_LBUTTONUP, nFlags, rawpoint);
		}

	}
	m_bDragging = false;
	
	CStatic::OnLButtonUp(nFlags, point);
}

void CEdtImageWindow::SetImageHandle(CEdtImage * pImage, LPCSTR ImageName)
{
	if (pImage != m_pImage)
	{
		m_pImage = pImage;
		m_pImage->SetupDisplay();
	
		if (m_pActiveImage)
		{
			m_pActiveImage->AttachToParent(m_pImage,
					m_SubRegion.left, m_SubRegion.top,
					m_SubRegion.Width(), m_SubRegion.Height());
		}
	}

}

void CEdtImageWindow::UpdateDisplay()
{
	Invalidate(FALSE);
	UpdateWindow();
}


void CEdtImageWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect wndRect;
	CRect clientRect;

	GetWindowRect(&wndRect);
	GetClientRect(&clientRect);

	// TODO: Add your message handler code here

	if (m_pImage)
	{
		m_pImage->Draw(m_hWnd, dc.m_hDC, NULL);
	}

	if (m_bRectActive)
	{
		if (!m_SubRegion.IsRectEmpty())
		{
			CBrush brush(RGB(255,00,0));
			
			dc.FrameRect(&m_SubRegion,&brush);
		}


	} 
	if (m_bLineActive)
	{
		CPen pen(PS_SOLID, 1, RGB(255,00,0));
			
		dc.SelectObject(&pen);

		dc.MoveTo(m_Point1);
		dc.LineTo(m_Point2);
		
	}
	
	// Do not call CStatic::OnPaint() for painting messages

}


void CEdtImageWindow::SetNotifyParentMouseMove(BOOL bState)
{
	m_bNotifyParentMouseMove = bState;
}


BOOL CEdtImageWindow::GetEndPoints(CPoint &pt1, CPoint &pt2)
{
	if (m_bLineActive)
	{
		pt1 = m_Point1;
		pt2 = m_Point2;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CEdtImageWindow::SetEndPoints(const CPoint pt1, const CPoint pt2)
{
	m_Point1 = pt1;
	m_Point2 = pt2;
}

BOOL CEdtImageWindow::GetRegion(CRect &region)
{
	region = m_SubRegion;
	return TRUE;
}

void CEdtImageWindow::SetRegion(const CRect region)
{
	m_SubRegion = region;
}

CEdtImage *CEdtImageWindow::GetImage()
{
	if (m_pActiveImage)
		return m_pActiveImage;
	else
		return m_pImage;
}

CEdtImage * CEdtImageWindow::GetSubImage()
{
	return m_pActiveImage;

}

CEdtImage * CEdtImageWindow::GetBaseImage()
{
	return m_pImage;
}


BOOL CEdtImageWindow::PtInImage(const CPoint pt)
{
	CEdtImage *pImage = GetBaseImage();

	if (!pImage)
		return FALSE;

	if (pt.x < 0 || pt.x >= pImage->GetWidth() ||
		pt.y < 0 || pt.y >= pImage->GetHeight())
		return FALSE;

	return TRUE;
}

void CEdtImageWindow::NotifyMouseAction(UINT message, UINT nFlags, CPoint point)
{
	CWnd *wndParent = GetParent();

	if (wndParent)
	{
		CRect wrect;
		GetWindowRect(&wrect);
		CRect drect;
		wndParent->GetWindowRect(&drect);
		
		point.x += wrect.left - drect.left;
		point.y += wrect.top - drect.top;

		DWORD pt = MAKELPARAM(point.x, point.y);

		wndParent->SendMessage(message, nFlags, pt);
	}
}

void CEdtImageWindow::SetLineTracking(BOOL bState)
{
	m_bLineTracking = bState;
	if (bState)
	{
		m_bLineActive = TRUE;
	}

}

void CEdtImageWindow::SetRectTracking(BOOL bState)
{
	m_bRectTracking = bState;
	if (bState)
	{
		SetRectActive(TRUE);
	}

}

BOOL CEdtImageWindow::IsLineTracking()
{
	return m_bLineTracking;
}

BOOL CEdtImageWindow::IsRectTracking()
{
	return m_bRectTracking;
}


void CEdtImageWindow::SetLineActive(BOOL bState)
{
	m_bLineActive = bState;
}

void CEdtImageWindow::SetRectActive(BOOL bState)
{
	m_bRectActive = bState;

	if (!bState)
	{
		if (m_pActiveImage)
		{
			delete m_pActiveImage;
			m_pActiveImage = NULL;
		}
	}
	else
	{
		if (!m_SubRegion.IsRectEmpty())
		{
			if (!m_pActiveImage)
				m_pActiveImage =  MainWindowFactory->NewDisplayableImage(m_pImage->GetType());

			m_pActiveImage->AttachToParent(m_pImage,
				m_SubRegion.left, m_SubRegion.top,
				m_SubRegion.Width(), m_SubRegion.Height());

		}		
	}
}

BOOL CEdtImageWindow::IsLineActive()
{
	return m_bLineActive;
}

BOOL CEdtImageWindow::IsRectActive()
{
	return m_bRectActive;
}
