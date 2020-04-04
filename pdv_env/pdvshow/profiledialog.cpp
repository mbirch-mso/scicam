// ProfileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"

#include "ProfileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileDialog dialog


CProfileDialog::CProfileDialog(CWnd* pParent /*=NULL*/)
: CDialog(CProfileDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CProfileDialog)
    m_nX1 = 0;
    m_nX2 = 0;
    m_nY1 = 0;
    m_nY2 = 0;
    //}}AFX_DATA_INIT

    m_pImage = NULL;
}


void CProfileDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProfileDialog)
    DDX_Control(pDX, IDC_CURSOR_VALUES, m_wndCursorValues);
    DDX_Control(pDX, IDC_Y_POINT2, m_wndY2);
    DDX_Control(pDX, IDC_Y_POINT1, m_wndY1);
    DDX_Control(pDX, IDC_X_POINT2, m_wndX2);
    DDX_Control(pDX, IDC_X_POINT1, m_wndX1);
    DDX_Control(pDX, IDC_PROFILE_WINDOW, m_wndProfileCntl);
    DDX_Text(pDX, IDC_X_POINT1, m_nX1);
    DDX_Text(pDX, IDC_X_POINT2, m_nX2);
    DDX_Text(pDX, IDC_Y_POINT1, m_nY1);
    DDX_Text(pDX, IDC_Y_POINT2, m_nY2);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileDialog, CDialog)
    //{{AFX_MSG_MAP(CProfileDialog)
    ON_BN_CLICKED(IDC_COMPUTE_BUTTON, OnComputeButton)
    ON_WM_SHOWWINDOW()
    ON_MESSAGE(WM_PLOTWINDOW_BUTTON, OnPlotButton)
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDialog message handlers

void CProfileDialog::OnComputeButton() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    Compute(NULL);

}

void CProfileDialog::Compute(EdtImage * pImage)
{
    if (pImage)
	m_pImage = pImage;

    if (m_pImage && m_nX1 != m_nX2 || m_nY1 != m_nY2)
    {

	m_Profile.Compute(m_pImage,m_nX1, m_nY1, m_nX2, m_nY2);

	m_wndProfileCntl.Reset(m_Profile.GetProfileArray(), 
	    m_Profile.GetNPixels(),
	    m_pImage->GetMaxValue(), m_Profile.GetNColors());

	m_wndProfileCntl.Invalidate();
	m_wndProfileCntl.UpdateWindow();

    }

}

void CProfileDialog::SetPoints(CPoint point1, CPoint point2)
{
    m_nX1 = point1.x;
    m_nY1 = point1.y;

    m_nX2 = point2.x;
    m_nY2 = point2.y;

    CString s;
    s.Format("%d", m_nX1);
    m_wndX1.SetWindowText(s);
    s.Format("%d", m_nX2);
    m_wndX2.SetWindowText(s);
    s.Format("%d", m_nY1);
    m_wndY1.SetWindowText(s);
    s.Format("%d", m_nY2);
    m_wndY2.SetWindowText(s);

}

void CProfileDialog::Compute(EdtImage * pImage, CPoint pt1, CPoint pt2)
{
    SetPoints(pt1, pt2);
    Compute(pImage);
}

void CProfileDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CDialog::OnShowWindow(bShow, nStatus);

    // TODO: Add your message handler code here

    m_bVisible = bShow;

}

void CProfileDialog::OnClose() 
{
    // TODO: Add your message handler code here and/or call default
    m_bVisible = FALSE;
    CDialog::OnClose();
}

LRESULT CProfileDialog::OnPlotButton(WPARAM wparam, LPARAM lparam)
{
    // lparam is bin value of histogram
    CString s;

    int nBin = (int) lparam;

    CPoint pt1, pt2;
    m_Profile.GetPoints(pt1,pt2);

    EdtPoint *offsets = m_Profile.GetOffsets();
    int **pData = m_Profile.GetProfileArray();

    if (offsets && pData)
    {
	s.Format("(%d,%d) =", offsets[nBin].x + pt1.x, offsets[nBin].y + pt1.y);
	for (int i = 0;i< m_Profile.GetNColors(); i++)
	{
	    CString s2;
	    s2.Format(" %d", pData[i][nBin]);
	    s += s2;
	}
    }

    m_wndCursorValues.SetWindowText(s);

    return 0;
}
