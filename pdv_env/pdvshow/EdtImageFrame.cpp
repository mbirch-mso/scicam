// EdtImageFrame.cpp : implementation file
//

#include "stdafx.h"

#include "edtimage.h"

#include "resource.h"

#include "EdtImageFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "ErrorHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CEdtImageFrame

IMPLEMENT_DYNCREATE(CEdtImageFrame, CFrameWnd)

CEdtImageFrame::CEdtImageFrame()
{
	m_pImageWindow = NULL;
	m_LastPosition = CRect(0,0,0,0);
	m_bValueSet = FALSE;

	m_nX = 0;
	m_nY = 0;

	m_sPixelValue = "";
	m_sStatus = "";

	m_dZoom = 1.0;

}

CEdtImageFrame::~CEdtImageFrame()
{
}


BEGIN_MESSAGE_MAP(CEdtImageFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CEdtImageFrame)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtImageFrame message handlers

void CEdtImageFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	m_wndStatusBar.GetWindowRect(&m_StatusRect);

	if (m_pImageWindow)
		m_pImageWindow->MoveWindow(0,0,cx,cy-m_StatusRect.Height());

	if (!m_bValueSet)
		GetWindowRect(&m_LastPosition);
	
}

void CEdtImageFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	


	if (m_pImageWindow)
	{
		
	
		CPoint subpoint = point;
		//CClientDC dc(this);
		//dc.LPtoDP(&subpoint);

	//	TRACE("Mouse %d,%d maps to %d,%d\n",
	//			point.x, point.y,
	//			subpoint.x, subpoint.y);

		m_pImageWindow->ConvertToImageCoordinates(point);

		m_nX = point.x;
		m_nY = point.y;

		EdtImage *pImage;

		if (pImage = m_pImageWindow->GetBaseImage())
		{

			if (point.x <= pImage->GetWidth() &&
				point.y <= pImage->GetHeight()) 
			{
				int Value = 0;
				pImage->GetPixel(point.x, point.y, Value);

				if (pImage->GetNColors() == 1)
					m_sPixelValue.Format("%5d",  Value);
				else
					m_sPixelValue.Format("(%3d,%3d,%3d)", 
						GetRValue(Value), GetGValue(Value), GetBValue(Value));

			}
			else
			{
				m_sPixelValue = "";
			
			}
		}

		m_dZoom = m_pImageWindow->m_wData.m_Zoom.GetZoomX();

		UpdateXYPixelZoom();

		m_pImageWindow->SendMessage(WM_MOUSEMOVE, nFlags, MAKELPARAM(subpoint.x, subpoint.y));

	}
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CEdtImageFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	//CFrameWnd::OnClose();

	CFrameWnd::ShowWindow(SW_HIDE);

}

void CEdtImageFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	if (!m_bValueSet)
		GetWindowRect(&m_LastPosition);

}

void CEdtImageFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if (bShow)
	{
		if (m_LastPosition.Width())
		{
			MoveWindow(&m_LastPosition);
			m_bValueSet = FALSE;
		}
	}
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CEdtImageFrame::RecalcLayout(BOOL bNotify) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFrameWnd::RecalcLayout(bNotify);
}

void CEdtImageFrame::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFrameWnd::CalcWindowRect(lpClientRect, nAdjustType);
}


static UINT indicators[] =
{
	ID_INDICATOR_STATUS,           // status line indicator
	ID_INDICATOR_X,
	ID_INDICATOR_Y,
	ID_INDICATOR_PIXEL_VALUE,
	ID_INDICATOR_ZOOM
};

int CEdtImageFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		return -1;      // fail to create
	}
	
	// TODO: Add your specialized creation code here
	
	m_wndStatusBar.GetWindowRect(&m_StatusRect);

	return 0;
}

void CEdtImageFrame::OnUpdateIndicatorX(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CString s;

	s.Format("X = %4d", m_nX);

	pCmdUI->SetText(s);
	pCmdUI->Enable(TRUE);


}


void CEdtImageFrame::OnUpdateIndicatorY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CString s;

	s.Format("Y = %4d", m_nY);

	pCmdUI->SetText(s);
	pCmdUI->Enable(TRUE);


}


void CEdtImageFrame::OnUpdateIndicatorZoom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CString s;

	int pct = (int) (m_dZoom * 100);

	s.Format("Zoom = %5d %%", pct);

	pCmdUI->SetText(s);
	pCmdUI->Enable(TRUE);


}



void CEdtImageFrame::OnUpdateIndicatorPixelValue(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(m_sPixelValue);
	pCmdUI->Enable(TRUE);


}


void CEdtImageFrame::OnUpdateIndicatorStatus(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(m_sStatus);
	pCmdUI->Enable(TRUE);


}

void
CEdtImageFrame::UpdateXYPixelZoom()

{

	m_wndStatusBar.SetPaneText(0,m_sStatus);
	
	CString s;

	
	s.Format("%4d", m_nX);
	m_wndStatusBar.SetPaneText(1,s);
	s.Format("%4d", m_nY);
	m_wndStatusBar.SetPaneText(2,s);

	m_wndStatusBar.SetPaneText(3,m_sPixelValue);

}

void CEdtImageFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pImageWindow)
	{
		

		m_pImageWindow->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));

	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CEdtImageFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		if (m_pImageWindow)
	{
		

		m_pImageWindow->SendMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));

	}
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CEdtImageFrame::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pImageWindow)
	{
		

		m_pImageWindow->SendMessage(WM_RBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));

	}	
	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CEdtImageFrame::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pImageWindow)
	{
		

		m_pImageWindow->SendMessage(WM_RBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));

	}	
	
	CFrameWnd::OnRButtonUp(nFlags, point);
}

/**
 * Given an Image create a new Frame and EdtImageWin
*/

void CEdtImageFrame::CreateImageViewer(const char *Label, 
												EdtImage *pImage,
												CRect *pBox)

{

	CRect sub;

	if (pBox)
		sub = *pBox;
	else if (pImage)
		sub.SetRect(0,0,pImage->GetWidth(), pImage->GetHeight());
	else
		sub.SetRect(0,0,512,512);


	Create(NULL,Label, WS_OVERLAPPEDWINDOW, sub);

	CRect sub2;
	CRect wrect;

	GetWindowRect(&wrect);

	GetClientRect(&sub2);

	wrect.right += sub.Width() - sub2.Width();
	wrect.bottom += sub.Height() - sub2.Height();

	MoveWindow(&wrect,FALSE);

	m_pImageWindow = new CEdtImageWin;

	m_pImageWindow->Create(Label,WS_VISIBLE, sub, this, 2000);

	m_pImageWindow->m_bResizeParent = TRUE;

	if (pImage)
		m_pImageWindow->SetImageHandle(pImage, Label, TRUE);

	m_pImageWindow->SetMouseZoomMode(ZOOM_MODE_INCREASE);

	ShowWindow(SW_SHOW);


}

/**
 * Factory function to create a new Image frame based on a provided EdtImage
 */


CEdtImageFrame * CEdtImageFrame::NewImageViewer(const char *Label, 
												EdtImage *pImage,
												CRect *pBox)

{	

	CEdtImageFrame *pFrame = new CEdtImageFrame;

	pFrame->CreateImageViewer(Label,pImage,pBox);

	
	return pFrame;

}
