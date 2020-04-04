// ZoomDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "ZoomDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomDialog dialog


CZoomDialog::CZoomDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomDialog)
	m_nXCorner = 0;
	m_nYCorner = 0;
	m_dXScale = 0.0;
	m_dYScale = 0.0;
	//}}AFX_DATA_INIT

	m_pAttachedWindow = NULL;

}


void CZoomDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDialog)
	DDX_Control(pDX, IDC_SPIN_ZOOM_SCALE, m_wndSpinZoomScale);
	DDX_Control(pDX, IDC_SPIN_YCORNER, m_wndSpinCornerY);
	DDX_Control(pDX, IDC_SPIN_XCORNER, m_wndSpinXCorner);
	DDX_Text(pDX, IDC_X_CORNER, m_nXCorner);
	DDX_Text(pDX, IDC_Y_CORNER, m_nYCorner);
	DDX_Text(pDX, IDC_X_SCALE, m_dXScale);
	DDX_Text(pDX, IDC_Y_SCALE, m_dYScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDialog, CDialog)
	//{{AFX_MSG_MAP(CZoomDialog)
	ON_EN_UPDATE(IDC_X_CORNER, OnUpdateXCorner)
	ON_EN_KILLFOCUS(IDC_X_CORNER, OnKillfocusXCorner)
	ON_EN_KILLFOCUS(IDC_Y_CORNER, OnKillfocusYCorner)
	ON_EN_UPDATE(IDC_Y_CORNER, OnUpdateYCorner)
	ON_EN_KILLFOCUS(IDC_Y_SCALE, OnKillfocusYScale)
	ON_EN_UPDATE(IDC_Y_SCALE, OnUpdateYScale)
	ON_EN_KILLFOCUS(IDC_X_SCALE, OnKillfocusXScale)
	ON_EN_UPDATE(IDC_X_SCALE, OnUpdateXScale)
	ON_BN_CLICKED(ID_CLOSE, OnClose)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZOOM_SCALE, OnDeltaposSpinZoomScale)
	ON_BN_CLICKED(IDC_UPDATE_WINDOW, OnUpdateWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDialog message handlers

void CZoomDialog::OnUpdateXCorner() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnKillfocusXCorner() 
{
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnKillfocusYCorner() 
{
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnUpdateYCorner() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnKillfocusYScale() 
{
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnUpdateYScale() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnKillfocusXScale() 
{
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnUpdateXScale() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void CZoomDialog::OnClose() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}

void CZoomDialog::OnDeltaposSpinZoomScale(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if (m_pAttachedWindow)
	{
		CPoint pt;

		m_pAttachedWindow->GetImageCenterPoint(pt);

		m_pAttachedWindow->ChangeZoom(-pNMUpDown->iDelta, pt);

		UpdateFromWindow();

		UpdateData(FALSE);

	}
	*pResult = 0;
}

void CZoomDialog::SetWindow(CEdtImageWin * pwindow)
{
	m_pAttachedWindow = pwindow;

	if (m_pAttachedWindow)
		UpdateFromWindow();

}

void CZoomDialog::UpdateFromWindow()
{
	if (m_pAttachedWindow)
		m_pAttachedWindow->GetZoomValues(m_nXCorner, m_nYCorner, m_dXScale, m_dYScale);
	if (m_hWnd)
		UpdateData(FALSE);

}

void CZoomDialog::UpdateToWindow()
{
	if (m_hWnd)
		UpdateData(TRUE);

	if (m_pAttachedWindow)
	{
		m_pAttachedWindow->SetZoom(m_nXCorner, m_nYCorner, m_dXScale, m_dYScale);
		m_pAttachedWindow->UpdateDisplay();
	}

}

void CZoomDialog::OnUpdateWindow() 
{
	// TODO: Add your control notification handler code here
	UpdateToWindow();

}
