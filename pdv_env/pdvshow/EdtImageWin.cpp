//
// EdtImageWin.cpp : implementation file
//

#include "stdafx.h"
#include "EdtImage.h"
#include "edtdefines.h"
#include "EdtImageWin.h"

extern "C" {
#include "edtinc.h"
}

#include "MsWindowPaint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGripper message map
BEGIN_MESSAGE_MAP(CGripper, CScrollBar)
    //{{AFX_MSG_MAP(CGripper)
    ON_WM_NCHITTEST()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGripper message handlers
HITTEST_RET CGripper::OnNcHitTest(CPoint point) 
{
    HITTEST_RET ht = CScrollBar::OnNcHitTest(point);
    if(ht==HTCLIENT)
    {
	ht = HTBOTTOMRIGHT;
    }
    return ht;
}



/////////////////////////////////////////////////////////////////////////////
// CEdtImageWin

CEdtImageWin::CEdtImageWin()
{
    m_bNotifyParentMouseMove = FALSE;
    m_wndStatus = NULL;

    m_bScrollBarsActive = TRUE;
    m_nScrollBorder = 16;

    m_nMargin = 0;

    m_nMouseZoomMode = ZOOM_MODE_NORMAL;
    m_dDeltaZoom = 1.0;


    m_bResizeParent = FALSE;

    CMsWindowPaint *pPainter = new CMsWindowPaint;

    m_wData.SetPainter(pPainter);
}

CEdtImageWin::~CEdtImageWin()
{
    if (m_wData.m_pPainter)
	delete m_wData.m_pPainter;
}


BEGIN_MESSAGE_MAP(CEdtImageWin, CStatic)
    //{{AFX_MSG_MAP(CEdtImageWin)
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_VSCROLL_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CEdtImageWin::OnMouseMove(UINT nFlags, CPoint rawpoint) 
{

    CPoint point = rawpoint;

    CClientDC dc(this);
    //	dc.DPtoLP(&point);

    // convert point to image coordinates

    ConvertToImageCoordinates(point);

    if (PtInImage(point))
    {
	if (IsLineTracking() && IsDragging())
	{

	    // Draw a line inverting

	    dc.SetROP2(R2_NOT);

	    if (m_wData.m_RawPoint1 != m_wData.m_RawPoint2)
	    {

		dc.MoveTo(m_wData.m_RawPoint1);
		dc.LineTo(m_wData.m_RawPoint2);
	    }

	    m_wData.m_Point2 = point;
	    m_wData.m_RawPoint2 = rawpoint;

	    dc.MoveTo(m_wData.m_RawPoint1);
	    dc.LineTo(m_wData.m_RawPoint2);

	} 
	if (IsRectTracking() && IsDragging())
	{
	    CRect lastrect = m_wData.m_RawSubRegion;

	    m_wData.m_Corner2 = point;
	    m_wData.m_RawCorner2 = rawpoint;

	    m_wData.m_SubRegion = CRect(m_wData.m_Corner1, m_wData.m_Corner2);
	    m_wData.m_SubRegion.NormalizeRect();
	    m_wData.m_RawSubRegion = CRect(m_wData.m_RawCorner1, m_wData.m_RawCorner2);
	    m_wData.m_RawSubRegion.NormalizeRect();


	    dc.DrawDragRect(&m_wData.m_RawSubRegion,CSize(2,2),&lastrect,CSize(2,2),NULL);

	}

	if (m_wndStatus)
	{
	    CString s;

	    EdtImage *pImage = GetBaseImage();

	    if (pImage && point.x <= pImage->GetWidth() &&
		point.y <= pImage->GetHeight()) 
	    {
		int Value = 0;
		pImage->GetPixel(point.x, point.y, Value);

		if (pImage->GetNColors() == 1)
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
    //CStatic::OnMouseMove(nFlags, rawpoint);
}

void CEdtImageWin::OnLButtonDown(UINT nFlags, CPoint rawpoint) 
{

    CPoint point = rawpoint;


    CClientDC dc(this);
    dc.DPtoLP(&point);

    ConvertToImageCoordinates(point);

    if (PtInImage(point))
    {
	// Draw a line inverting

	dc.SetROP2(R2_NOT);

	if (IsLineTracking())
	{

	    // Draw a line inverting


	    if (m_wData.m_RawPoint1 !=m_wData.m_RawPoint2)
	    {

		dc.MoveTo(m_wData.m_RawPoint1);
		dc.LineTo(m_wData.m_RawPoint2);
	    }

	    m_wData.m_Point1 = m_wData.m_Point2 = point;
	    m_wData.m_RawPoint1 = m_wData.m_RawPoint2 = rawpoint;

	} 
	if (IsRectTracking())
	{

	    if (!m_wData.m_RawSubRegion.IsRectEmpty())
	    {
		dc.DrawDragRect(&m_wData.m_RawSubRegion,CSize(2,2),NULL,CSize(2,2),NULL);
	    }
	    // Create the subimage
	    m_wData.m_SubRegion = CRect(point,point);
	    m_wData.m_Corner1 = m_wData.m_Corner2 = point;

	    m_wData.m_RawSubRegion = CRect(rawpoint,rawpoint);
	    m_wData.m_RawCorner1 = m_wData.m_RawCorner2 = rawpoint;

	}

	SetDragging(TRUE);

    }

    CStatic::OnLButtonDown(nFlags, point);
}

void CEdtImageWin::OnLButtonUp(UINT nFlags, CPoint rawpoint) 
{
    bool bUpdate = FALSE;
    CPoint point = rawpoint;

    CClientDC dc(this);
    dc.DPtoLP(&point);

    ConvertToImageCoordinates(point);

    if (PtInImage(point))
    {
	if (m_nMouseZoomMode)
	{

	    switch (m_nMouseZoomMode)
	    {
	    case ZOOM_MODE_INCREASE:
		ChangeZoomUpdate(m_dDeltaZoom,point);
		break;
	    case ZOOM_MODE_DECREASE:
		ChangeZoomUpdate(-m_dDeltaZoom,point);
		break;
	    case ZOOM_MODE_CENTER:
		ChangeZoomUpdate(0,point);
		break;

	    case ZOOM_MODE_LINE_TRACK:
		if (IsDragging())
		{
		    // Draw a line inverting

		    dc.SetROP2(R2_NOT);


		    if (m_wData.m_RawPoint1 !=m_wData.m_RawPoint2)
		    {

			dc.MoveTo(m_wData.m_RawPoint1);
			dc.LineTo(m_wData.m_RawPoint2);
		    }

		    m_wData.m_Point2 = point;
		    m_wData.m_RawPoint2 = rawpoint;

		    dc.MoveTo(m_wData.m_RawPoint1);
		    dc.LineTo(m_wData.m_RawPoint2);

		    bUpdate = TRUE;

		} 
		break;
	    case ZOOM_MODE_RECT_TRACK:
		if (IsDragging())
		{

		    dc.SetROP2(R2_NOT);

		    CRect lastrect = m_wData.m_RawSubRegion;

		    m_wData.m_Corner2 = point;

		    m_wData.m_SubRegion = CRect(m_wData.m_Corner1, m_wData.m_Corner2);
		    m_wData.m_SubRegion.NormalizeRect();

		    m_wData.m_RawCorner2 = rawpoint;

		    m_wData.m_RawSubRegion = CRect(m_wData.m_RawCorner1, m_wData.m_RawCorner2);
		    m_wData.m_RawSubRegion.NormalizeRect();


		    dc.DrawDragRect(&m_wData.m_RawSubRegion,CSize(2,2),&lastrect,CSize(2,2),NULL);

		    // Create the subimage

		    if (!m_wData.m_SubRegion.IsRectEmpty())
		    {
			m_wData.SetRectActive(TRUE);

		    }

		    bUpdate = TRUE;	

		}
		break;
	    }
	}

	if (bUpdate)
	    UpdateDisplay();

	if (m_bNotifyParentMouseMove)
	{
	    NotifyMouseAction(WM_LBUTTONUP, nFlags, rawpoint);
	}

	SetDragging(FALSE);
    }

    CStatic::OnLButtonUp(nFlags, point);
}

void CEdtImageWin::UpdateDisplay()
{
    Invalidate(FALSE);
    UpdateWindow();
}

void CEdtImageWin::RedrawImage(CDC &dc, CRect updaterect, EdtImage *pImage)

{
#if  defined(_DEBUG) && defined(DEBUG_ACQUIRE)
    char s[128];
    sprintf(s,"Paint:     %d %10f delay = %10f ms stamp = %10f Started = %d\n",
	pImage->GetSequenceNumber(), edt_elapsed(FALSE), (edt_timestamp() - pImage->GetTimeStamp()) * 1000.0,
	pImage->GetTimeStamp(), m_wData.m_nPumpStarted);
    OutputDebugString(s);
#endif

    if (m_wData.m_nPumpStarted)
    {
	m_wData.AddDelayValue( edt_timestamp() - pImage->GetTimeStamp());

	m_wData.m_nPumpFinished = pImage->GetSequenceNumber();
	m_wData.m_nPumpStarted = 0;
    }

    COLORREF background = RGB(128,128,128);

    int fillbottom = FALSE , 
	fillright = FALSE;


    CRect wndRect;
    CRect clientRect;

    GetWindowRect(&wndRect);
    GetClientRect(&clientRect);

    CRect rcClipBox;
    dc.GetClipBox(&rcClipBox);


    if (m_vScroll.IsWindowVisible())
    {
	CRect rect;
	m_vScroll.GetWindowRect(&rect);
	rect -= wndRect.TopLeft();

	if (rect.left < updaterect.right)	
	    updaterect.right = rect.left;
    }

    if (m_hScroll.IsWindowVisible())
    {
	CRect rect;
	m_hScroll.GetWindowRect(&rect);

	rect -= wndRect.TopLeft();
	if (rect.top < updaterect.bottom)	
	    updaterect.bottom = rect.top;
    }



    if (updaterect.right > GetDisplayWidth(pImage))
    {
	fillright = TRUE;
    }
    if (updaterect.bottom > GetDisplayHeight(pImage))
    {
	fillbottom = TRUE;
    }

    // Fill in blank spaces

    if (fillright && fillbottom)
    {

	int l,t,r,b;

	l = edtmax(GetDisplayWidth(pImage),updaterect.left);
	t = updaterect.top;
	r = updaterect.right;
	b = updaterect.bottom;

	CRect fillbox(l,t,r,b);

	dc.FillSolidRect(&fillbox,background);

	l = updaterect.left;
	t = edtmax(GetDisplayHeight(pImage),updaterect.top);
	r = updaterect.right;
	b = updaterect.bottom;
	fillbox.SetRect(l,t,r,b);

	dc.FillSolidRect(&fillbox,background);

    }
    else if (fillright)
    {
	int l,t,r,b;

	l = edtmax(GetDisplayWidth(pImage),updaterect.left);
	t = updaterect.top;
	r = updaterect.right;
	b = updaterect.bottom;

	CRect fillbox(l,t,r,b);

	dc.FillSolidRect(&fillbox,background);

    }
    else if (fillbottom)
    {
	int l,t,r,b;
	l = updaterect.left;
	t = edtmax(GetDisplayHeight(pImage),updaterect.top);
	r = updaterect.right;
	b = updaterect.bottom;
	CRect fillbox(l,t,r,b);

	dc.FillSolidRect(&fillbox,background);

    }

    LPRECT pRect;

    if (updaterect.Width() <= 1 || updaterect.Height() <= 1)
    {
	pRect = NULL;
    }
    else
    {
	pRect = &updaterect;
    }


    //TRACE("UpdateRect %x %x -> %x %x\n",
    //		updaterect.left,updaterect.top, updaterect.right, updaterect.bottom);

    m_wData.Draw(pImage, dc.m_hDC, &updaterect);



    if (IsRectActive())
    {
	if (!m_wData.m_SubRegion.IsRectEmpty())
	{
	    CBrush brush(RGB(255,00,0));

	    CRect rect = m_wData.m_SubRegion;
	    ConvertToDisplayCoordinates(rect);
	    dc.FrameRect(&rect,&brush);
	}


    } 
    if (IsLineActive())
    {
	CPen pen(PS_SOLID, 1, RGB(255,00,0));
	CPoint pt;

	dc.SelectObject(&pen);

	pt = m_wData.m_Point1;
	ConvertToDisplayCoordinates(pt);

	dc.MoveTo(pt);
	pt = m_wData.m_Point2;
	ConvertToDisplayCoordinates(pt);

	dc.LineTo(pt);

    }

}


void CEdtImageWin::OnPaint() 
{

    EdtImage *pImage;

    if (pImage = GetBaseImage())
    {
	CPaintDC dc(this); // device context for painting
	CRect updaterect = dc.m_ps.rcPaint;
	RedrawImage(dc, updaterect, pImage);
	// Do not call CStatic::OnPaint() for painting messages
    }
    else
    {
	CStatic::OnPaint();
    }

}


void CEdtImageWin::SetNotifyParentMouseMove(bool bState)
{
    m_bNotifyParentMouseMove = bState;
}

void CEdtImageWin::NotifyMouseAction(UINT message, UINT nFlags, CPoint point)
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

	DWORD pt = (DWORD) MAKELPARAM(point.x, point.y);

	wndParent->SendMessage(message, nFlags, pt);
    }
}

void CEdtImageWin::SizeToImage()
{
    EdtImage *pImage;

    if (pImage = GetBaseImage())
    {

	int Width = GetWidth();
	int Height = GetHeight();

	CRect OldRect;
	CFrameWnd *parent;

	// Get pos in parent if child
	GetWindowRect (&OldRect);
	CRect StartRect = OldRect;

	CRect clientrect;

	GetClientRect(&OldRect);



	CRect ParentRect(0,0,0,0);
	CPoint TopLeft = OldRect.TopLeft();

	OldRect.SetRect(TopLeft.x,TopLeft.y, TopLeft.x + Width, TopLeft.y + Height);


	if (parent = (CFrameWnd *) GetParent())
	{
	    parent->GetClientRect(&ParentRect);

	    // get the right margin ...

	    CPoint pt(2,2);

	    int StartWidth = OldRect.Width();
	    int StartHeight = OldRect.Height();

	    CWnd *statusbar;
	    if (statusbar = parent->GetMessageBar())
	    {
		CRect statusrect;

		statusbar->GetWindowRect(&statusrect);

		ParentRect.bottom -= statusrect.Height();

	    }

	    // CLip against size of window 
	    OldRect.IntersectRect(OldRect,ParentRect);


	    OldRect -= ParentRect.TopLeft();

	    // Adjust for the right margin

	    OldRect -= pt;


	    if (OldRect.Width() != StartWidth)
		OldRect.right -= m_nMargin;

	    if (OldRect.Height() != StartHeight)
		OldRect.bottom -= m_nMargin;



	}

	MoveWindow(&OldRect);

    }
}

void CEdtImageWin::RedrawCallback(void *obj, EdtImage *pImage)

{
    CEdtImageWin *pWin = (CEdtImageWin *) obj;
    CWindowDC dc(pWin);
    CRect updaterect;
    pWin->GetClientRect(&updaterect);
    pWin->RedrawImage(dc, updaterect, pImage);
}

void CEdtImageWin::Update()
{



    m_wData.SetWindow(m_hWnd);
    m_wData.SetRedrawCB(RedrawCallback, this);

}

void CEdtImageWin::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    SCROLLINFO sinfo;
    m_vScroll.GetScrollInfo(&sinfo, SIF_ALL);

    int realmax = sinfo.nMax - sinfo.nPage;

    int sx, sy;
    double xscale, yscale;

    GetZoomValues(sx,sy,xscale, yscale);


    int delta = (int) yscale;
    if (delta < 1) delta = 1;

    int newpos = -1;

    switch (nSBCode)
    {
    case SB_BOTTOM:
	//	Scroll to bottom.

	newpos = realmax;


	break;

    case SB_ENDSCROLL:
	//	   End scroll.
	return;


    case SB_LINEDOWN:
	//	   Scroll one line down.
	newpos = sinfo.nPos + delta;
	if (newpos > realmax)
	    newpos = realmax;

	break;

    case SB_LINEUP:
	//	   Scroll one line up.
	newpos = sinfo.nPos - delta;
	if (newpos < sinfo.nMin)
	    newpos = sinfo.nMin;

	break;

    case SB_PAGEDOWN:
	//	   Scroll one page down.
	newpos = sinfo.nPos + sinfo.nPage;
	if (newpos > realmax)
	    newpos = realmax;

	break;

    case SB_PAGEUP:
	//	   Scroll one page up.
	newpos = sinfo.nPos - sinfo.nPage;
	if (newpos < sinfo.nMin)
	    newpos = sinfo.nMin;

	break;

    case SB_THUMBPOSITION:
	//	   Scroll to the absolute position. The current position is provided in nPos.
	newpos = nPos;

	break;

    case SB_THUMBTRACK:
	//	   Drag scroll box to specified position. The current position is provided in nPos.
	newpos = nPos;

	break;

    case SB_TOP:
	//	   Scroll to top
	newpos = sinfo.nMin;

	break;
    }


    if (newpos > -1)
    {
	/* adjust to zoom */
	if (yscale > 1)
	{
	    newpos = (int) (((int) (newpos / yscale) ) * yscale);
	}

	GotoScroll(m_hScroll.GetScrollPos(), newpos);

    }


}

void CEdtImageWin::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    SCROLLINFO sinfo;
    m_hScroll.GetScrollInfo(&sinfo, SIF_ALL);
    int realmax = sinfo.nMax - sinfo.nPage;
    int sx, sy;
    double xscale, yscale;

    GetZoomValues(sx,sy,xscale, yscale);

    int delta = (int) xscale;
    if (delta < 1) delta = 1;

    int newpos = -1;
    switch (nSBCode)
    {
    case SB_LEFT:
	//	Scroll to left
	newpos = 0;
	break;

    case SB_ENDSCROLL:
	//	   End scroll.

	break;

    case SB_LINELEFT:
	//	   Scroll one line down.
	newpos = sinfo.nPos - delta;
	if (newpos < sinfo.nMin)
	    newpos = sinfo.nMin;

	break;

    case SB_LINERIGHT:
	//	   Scroll one line up.
	newpos = sinfo.nPos + delta;
	if (newpos > realmax)
	    newpos = realmax;

	break;

    case SB_PAGELEFT:
	//	   Scroll one page down.
	newpos = sinfo.nPos - sinfo.nPage;
	if (newpos < sinfo.nMin)
	    newpos = sinfo.nMin;

	break;

    case SB_PAGERIGHT:
	//	   Scroll one page up.
	newpos = sinfo.nPos + sinfo.nPage;
	if (newpos > realmax)
	    newpos = realmax;

	break;

    case SB_RIGHT:
	//	   Scroll to top
	newpos = realmax;


	break;

    case SB_THUMBPOSITION:
	//	   Scroll to the absolute position. The current position is provided in nPos.
	newpos = nPos;

	break;

    case SB_THUMBTRACK:
	//	   Drag scroll box to specified position. The current position is provided in nPos.
	newpos = nPos;


	break;

    }

    if (newpos > -1)
    {
	/* adjust to zoom */


	if (xscale > 1)
	{
	    newpos = (int) (((int) (newpos / xscale) ) * xscale);
	}


	GotoScroll(newpos, m_vScroll.GetScrollPos());
    }

}


void CEdtImageWin::SizeFrame(bool bResizeParent)
{


    // Get pos in parent if child
    if (GetBaseImage())
    {

	int nWidth = GetWidth();
	int nHeight = GetHeight();

	if (bResizeParent)
	{
	    CFrameWnd * parent;

	    if (parent = (CFrameWnd *) GetParent())
	    {
		CRect newrect(0,0,nWidth, nHeight);

		parent->CalcWindowRect(&newrect,CWnd::adjustOutside);

		nWidth = newrect.Width();
		nHeight = newrect.Height();


		parent->SetWindowPos(NULL,0,0,nWidth,nHeight,
		    SWP_NOMOVE | SWP_NOZORDER);

	    }
	}

	SizeToImage();

    }

    AdjustScrollbars();

}

int CEdtImageWin::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CStatic::OnCreate(lpCreateStruct) == -1)
	return -1;


#ifdef USE_SCROLL_CTL

    CRect rect(lpCreateStruct->x, lpCreateStruct->y,
	lpCreateStruct->x + lpCreateStruct->cx, lpCreateStruct->y + lpCreateStruct->cy);

    m_vScroll.Create(WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN, rect, this, 0);
    m_hScroll.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN, rect, this, 0);
#endif

    Update();

    return 0;
}

void CEdtImageWin::OnSize(UINT nType, int cx, int cy) 
{

    CStatic::OnSize(nType, cx, cy);
    CRect rect;
    GetWindowRect(&rect);

#ifdef USE_SCROLL_CTL
    CRect scrollrect;


    // TODO: Add your message handler code here
    m_vScroll.GetWindowRect(&scrollrect);

    scrollrect -= rect.TopLeft();

    int width = scrollrect.Width();

    scrollrect.bottom = scrollrect.top + cy- m_nScrollBorder;
    scrollrect.left = rect.Width() - width;
    scrollrect.right = rect.Width();

    m_vScroll.MoveWindow(&scrollrect);

    m_hScroll.GetWindowRect(&scrollrect);

    scrollrect -= rect.TopLeft();

    int height = scrollrect.Height();


    scrollrect.right = scrollrect.left + cx - m_nScrollBorder;
    scrollrect.top = rect.Height() - height;
    scrollrect.bottom = rect.Height();

    m_hScroll.MoveWindow(&scrollrect);

#endif

    AdjustScrollbars();

}

void CEdtImageWin::AddScrollbars()
{

#ifdef USE_SCROLL_CTL
    CRect rect;

    GetClientRect(&rect);


    //	rect.SetRect(0,0,rect.Width() - m_nScrollBorder, rect.Height() - m_nScrollBorder);

    m_vScroll.Create(WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN, rect, this, 0xffff);
    m_hScroll.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN, rect, this, 0xffff);

    //	m_hScroll.BringWindowToTop();
    //	m_vScroll.BringWindowToTop();

#endif

}

void CEdtImageWin::AdjustScrollbars()
{

    EdtImage *pImage = GetBaseImage();
    if (pImage && pImage->GetWidth() && pImage->GetHeight())
    {
	SCROLLINFO sinfo;
	CRect clientrect;
	CRect wrect;

	GetClientRect(&clientrect);
	GetWindowRect(&wrect);

	int sx, sy;
	int worksx, worksy;
	double xscale, yscale;

	GetZoomValues(sx,sy,xscale, yscale);


	worksx = sx;
	worksy = sy;

	if (xscale > 1)
	{
	    sx = (int) (((int) (sx / xscale) ) * xscale);	
	}

	if (yscale > 1)
	{
	    sy = (int) (((int) (sy / yscale) ) * yscale);	
	}

	sinfo.cbSize = sizeof(sinfo);
	sinfo.fMask = SIF_ALL ;

	//GetScrollInfo(SB_HORZ, &sinfo);



	if (GetDisplayWidth(pImage) <= clientrect.Width())
	{
	    sx = 0;
	    m_hScroll.SetScrollRange(0,0);
	    m_hScroll.ShowScrollBar(FALSE);
	} 
	else
	{
	    if (clientrect.Width() > GetDisplayWidth(pImage) - xscale * sx)
	    {
		sx = (int) ( (GetDisplayWidth(pImage) - clientrect.Width()) / xscale);
		if (sx < 0)
		    sx = 0;
	    }
	    sinfo.nMin = 0;
	    sinfo.nMax = GetDisplayWidth(pImage);
	    sinfo.nPage = clientrect.Width();
	    sinfo.nPos = (int) (sx);

	    m_hScroll.SetScrollInfo(&sinfo);
	    m_hScroll.ShowScrollBar(TRUE);
	}


	if (GetDisplayHeight(pImage) <= clientrect.Height())
	{
	    sy = 0;
	    m_vScroll.SetScrollRange(0,0);
	    m_vScroll.ShowScrollBar(FALSE);
	} 
	else
	{
	    if (clientrect.Height() > GetDisplayHeight(pImage) - yscale * sy)
	    {
		sy = (int) ((GetDisplayHeight(pImage) - clientrect.Height()) / yscale);
		if (sy < 0)
		    sy = 0;
	    }

	    sinfo.nMin = 0;
	    sinfo.nMax = GetDisplayHeight(pImage);
	    sinfo.nPage = clientrect.Height();
	    sinfo.nPos = sy;
	    //GetDisplayHeight(pImage) - (sy * yscale) - sinfo.nPage; //(int) ((pImage->GetHeight() - sy) -sinfo.nPage);

	    m_vScroll.SetScrollInfo(&sinfo);
	    m_vScroll.ShowScrollBar(TRUE);
	}

	if (sx != worksx || sy != worksy)
	{
	    SetZoom(sx, sy, xscale, yscale);
	}
	//		TRACE("AdjustScrollbars pos = ( %d %d )  zoom = (%d %d %f %f)\n", 
	//				sx, sinfo.nPos, sx, sy, xscale, yscale);

	int nScrollWidth = 0;

	if (m_vScroll.IsWindowVisible())
	{
	    CRect rect;
	    m_vScroll.GetWindowRect(&rect);
	    nScrollWidth = rect.Width();

	}
	int nScrollHeight = 0;

	if (m_hScroll.IsWindowVisible())
	{
	    CRect rect;
	    m_hScroll.GetWindowRect(&rect);
	    nScrollHeight = rect.Height();

	}

	m_wData.SetScrollbarSizes(nScrollWidth, nScrollHeight);

    }

}

// nX, nY are in 
void CEdtImageWin::GotoScroll(int nX, int nY)
{
    // Set the scrollbar positions
    CRect rect;

    m_hScroll.SetScrollPos(nX);

    m_vScroll.SetScrollPos(nY);

    // Update the corners

    GetClientRect(&rect);

    // Convert nX to 

    nX = (int) (nX / m_wData.m_Zoom.GetZoomX());
    nY = (int) (nY / m_wData.m_Zoom.GetZoomY());

    m_wData.SetCorner(nX, nY);
    //GetHeight() - (nY + rect.Height()));

    //	TRACE("GotoScroll pos = ( %d %d )  image corner = (%d %d)\n", 
    //				nX, nY, nX, nY);

    UpdateZoomStatus();

    Invalidate();

}

void CEdtImageWin::SetMouseZoomMode(int nMode)
{
    m_nMouseZoomMode = nMode;
}



int CEdtImageWin::GetMouseZoomMode()
{
    return m_nMouseZoomMode;
}


void CEdtImageWin::ChangeZoom(double dChange, CPoint & center)
{
    double dZX, dZY;
    int nX, nY;
    CRect rect;

    GetZoomValues(nX, nY, dZX, dZY);

    GetClientRect(&rect);

    int nWidth = rect.Width();
    int nHeight = rect.Height();

    EdtImage *pImage = GetBaseImage();

    if (!pImage)
	return;

    if (dChange)
    {

	if (dZX < 1.0 || (dZX + dChange < 1.0))
	{
	    // invert dZX
	    dZX = 1.0 / dZX;

	    dZX -= dChange;

	    ASSERT(dZX != 0.0);

	    dZX = 1.0 / dZX;

	}
	else
	{
	    dZX += dChange;
	}

	SetScale(dZX, dZX);

    }

    // change window size to image size;

    nWidth = (int) (nWidth / dZX);
    nHeight = (int) (nHeight / dZX);

    if (nWidth > pImage->GetWidth())
	nWidth = pImage->GetWidth();
    if (nHeight > pImage->GetHeight())
	nHeight = pImage->GetHeight();

    int left, top, right, bottom;

    left = center.x - nWidth / 2;
    top = center.y - nHeight / 2;

    if (left < 0)
	left = 0;

    if (top < 0)
	top = 0;

    right = left + nWidth;
    bottom = top + nHeight;

    if (right > GetWidth())
    {
	left = GetWidth() - nWidth;
	right = GetWidth();
    }

    if (bottom > GetHeight())
    {
	top = GetHeight() - nHeight;
	bottom = GetHeight();
    }


    if (left < 0)
	left = 0;

    if (top < 0)
	top = 0;

    SetZoom(left, top, dZX, dZX);

    AdjustScrollbars();

    Invalidate();

}


void CEdtImageWin::ResetZoom()
{
    SetMouseZoomMode(ZOOM_MODE_NORMAL);

    SetZoom(0,0,1.0, 1.0);
    AdjustScrollbars();
    Invalidate();

}

void CEdtImageWin::OnRButtonDown(UINT nFlags, CPoint rawpoint) 
{
    // TODO: Add your message handler code here and/or call default


    CStatic::OnRButtonDown(nFlags, rawpoint);
}

void CEdtImageWin::OnRButtonUp(UINT nFlags, CPoint rawpoint) 
{
    // TODO: Add your message handler code here and/or call default
    // TODO: Add your message handler code here and/or call default
    bool bUpdate = FALSE;
    CPoint point = rawpoint;

    CClientDC dc(this);
    dc.DPtoLP(&point);

    if (PtInImage(point))
    {
	if (m_nMouseZoomMode)
	{
	    ConvertToImageCoordinates(point);

	    switch (m_nMouseZoomMode)
	    {

	    case ZOOM_MODE_INCREASE:
		ChangeZoomUpdate(-m_dDeltaZoom,point);
		break;
	    case ZOOM_MODE_DECREASE:
		ChangeZoomUpdate(m_dDeltaZoom,point);
		break;

	    }
	}
    }	
    CStatic::OnRButtonUp(nFlags, rawpoint);
}

void CEdtImageWin::VScroll(UINT nSBCode, UINT nPos)
{

    SCROLLINFO sinfo;
    m_vScroll.GetScrollInfo(&sinfo, SIF_ALL);

    switch (nSBCode)
    {
    case SB_BOTTOM:
	//	Scroll to bottom.

	break;

    case SB_ENDSCROLL:
	//	   End scroll.

	break;

    case SB_LINEDOWN:
	//	   Scroll one line down.

	break;

    case SB_LINEUP:
	//	   Scroll one line up.

	break;

    case SB_PAGEDOWN:
	//	   Scroll one page down.

	break;

    case SB_PAGEUP:
	//	   Scroll one page up.
	GotoScroll(GetScrollPos(SB_HORZ), nPos);
	break;

    case SB_THUMBPOSITION:
	//	   Scroll to the absolute position. The current position is provided in nPos.
	GotoScroll(GetScrollPos(SB_HORZ), nPos );

	break;

    case SB_THUMBTRACK:
	//	   Drag scroll box to specified position. The current position is provided in nPos.
	GotoScroll(GetScrollPos(SB_HORZ), nPos );

	break;

    case SB_TOP:
	//	   Scroll to top

	break;
    }
}

void CEdtImageWin::HScroll(UINT nSBCode, UINT nPos)
{

}

void CEdtImageWin::ConvertToDisplayCoordinates(CPoint &pt)
{
    m_wData.ConvertToDisplayCoordinates(pt);
}

void CEdtImageWin::ConvertToDisplayCoordinates(CRect &rect)
{
    m_wData.ConvertToDisplayCoordinates(rect);

}

int CEdtImageWin::GetZoomMode()
{	
    return m_nMouseZoomMode;

}

void CEdtImageWin::GetImageCenterPoint(CPoint & pt)
{
    CRect clientrect;
    GetClientRect(&clientrect);

    EdtImage *pImage = GetBaseImage();

    if (GetDisplayWidth(pImage) <= clientrect.Width())
	clientrect.right = GetDisplayWidth(pImage);

    if (GetDisplayHeight(pImage) <= clientrect.Height())
	clientrect.bottom = GetDisplayHeight(pImage);


    pt = clientrect.CenterPoint();

    ConvertToImageCoordinates(pt);

}

void CEdtImageWin::ChangeZoomUpdate(double delta, CPoint pt)
{
    ChangeZoom(delta,pt);


    UpdateZoomStatus();

}

void CEdtImageWin::UpdateZoomStatus()
{
    CWnd *wndParent = GetParent();

    if (wndParent)
    {
	wndParent->SendMessage(WM_ZOOM_STATE_UPDATE, 0, (LPARAM) this);
    }

}

void CEdtImageWin::SetImageHandle(EdtImage *pImage, const char *ImageName, const bool bReset)

{

    // Create local image for display if not


    m_wData.SetImageHandle(pImage,
	ImageName,
	bReset);

}


void CEdtImageWin::ResetImageHandle(EdtImage *pImage, const char *Name)
{
    m_wData.ResetImageHandle(pImage, Name);
}
