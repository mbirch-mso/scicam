// HistDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"

#include "HistDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistDialog dialog


CHistDialog::CHistDialog(CWnd* pParent /*=NULL*/)
: CDialog(CHistDialog::IDD, pParent)
{
    EnableAutomation();

    //{{AFX_DATA_INIT(CHistDialog)
    m_sNPixels = _T("");
    m_nDoPixels = 0;
    //}}AFX_DATA_INIT

    for (int i =0; i< 3; i++)
    {
	m_sMaxPixel[i] = _T("");
	m_sMean[i]  = _T("");
	m_sMedian[i]  = _T("");
	m_sMinPixel[i]  = _T("");
	m_sSD[i]  = _T("");
    }

    m_pImage = NULL;
}


void CHistDialog::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.

    CDialog::OnFinalRelease();
}

void CHistDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHistDialog)
    DDX_Control(pDX, IDC_CURSOR_VALUES, m_wndCursorValues);
    DDX_Control(pDX, IDC_S_NPIXELS, m_wndSNPixels);
    DDX_Text(pDX, IDC_S_NPIXELS, m_sNPixels);
    DDX_Control(pDX, IDC_S_MEDIAN, m_wndSMedian[0]);
    DDX_Control(pDX, IDC_S_MINPIXEL, m_wndSMinPixel[0]);
    DDX_Control(pDX, IDC_S_SD, m_wndSSD[0]);
    DDX_Control(pDX, IDC_S_MEAN, m_wndSMean[0]);
    DDX_Control(pDX, IDC_S_MAXPIXEL, m_wndSMaxPixel[0]);
    DDX_Text(pDX, IDC_S_MAXPIXEL, m_sMaxPixel[0]);
    DDX_Text(pDX, IDC_S_MEAN, m_sMean[0]);
    DDX_Text(pDX, IDC_S_MEDIAN, m_sMedian[0]);
    DDX_Text(pDX, IDC_S_MINPIXEL, m_sMinPixel[0]);
    DDX_Text(pDX, IDC_S_SD, m_sSD[0]);
    DDX_Control(pDX, IDC_S_MEDIAN2, m_wndSMedian[1]);
    DDX_Control(pDX, IDC_S_MINPIXEL2, m_wndSMinPixel[1]);
    DDX_Control(pDX, IDC_S_SD2, m_wndSSD[1]);
    DDX_Control(pDX, IDC_S_MEAN2, m_wndSMean[1]);
    DDX_Control(pDX, IDC_S_MAXPIXEL2, m_wndSMaxPixel[1]);
    DDX_Text(pDX, IDC_S_MAXPIXEL2, m_sMaxPixel[1]);
    DDX_Text(pDX, IDC_S_MEAN2, m_sMean[1]);
    DDX_Text(pDX, IDC_S_MEDIAN2, m_sMedian[1]);
    DDX_Text(pDX, IDC_S_MINPIXEL2, m_sMinPixel[1]);
    DDX_Text(pDX, IDC_S_SD2, m_sSD[1]);
    DDX_Control(pDX, IDC_S_MEDIAN3, m_wndSMedian[2]);
    DDX_Control(pDX, IDC_S_MINPIXEL3, m_wndSMinPixel[2]);
    DDX_Control(pDX, IDC_S_SD3, m_wndSSD[2]);
    DDX_Control(pDX, IDC_S_MEAN3, m_wndSMean[2]);
    DDX_Control(pDX, IDC_S_MAXPIXEL3, m_wndSMaxPixel[2]);
    DDX_Text(pDX, IDC_S_MAXPIXEL3, m_sMaxPixel[2]);
    DDX_Text(pDX, IDC_S_MEAN3, m_sMean[2]);
    DDX_Text(pDX, IDC_S_MEDIAN3, m_sMedian[2]);
    DDX_Text(pDX, IDC_S_MINPIXEL3, m_sMinPixel[2]);
    DDX_Text(pDX, IDC_S_SD3, m_sSD[2]);
    DDX_Control(pDX, IDC_PLOT_WINDOW, m_wndHistWindow);
    DDX_Radio(pDX, IDC_PIXEL_VALUE_BUTTON, m_nDoPixels);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistDialog, CDialog)
    //{{AFX_MSG_MAP(CHistDialog)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_PIXEL_VALUE_BUTTON, OnPixelValues)
    ON_BN_CLICKED(IDC_BITS, OnPixelValues)
    ON_MESSAGE(WM_PLOTWINDOW_BUTTON, OnPlotButton)
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CHistDialog, CDialog)
    //{{AFX_DISPATCH_MAP(CHistDialog)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IHistDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8C1D618C-2AFB-11D2-97D9-00104B357776}
static const IID IID_IHistDialog =
{ 0x8c1d618c, 0x2afb, 0x11d2, { 0x97, 0xd9, 0x0, 0x10, 0x4b, 0x35, 0x77, 0x76 } };

BEGIN_INTERFACE_MAP(CHistDialog, CDialog)
    INTERFACE_PART(CHistDialog, IID_IHistDialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistDialog message handlers

void CHistDialog::RefreshData()
{

}

void CHistDialog::Compute(EdtImage * pImage)
{
    if (pImage)
	m_pImage = pImage;

    if (m_pImage)
    {
	m_Hist.Compute(m_pImage);
	m_Hist.ComputeStatistics(m_Stats);

	// Refresh Plot

	if (m_nDoPixels == 1)
	{
	    m_Hist.CountBits();
	    m_pBitCounts[0] = m_Hist.GetBitCounts();

	    int nMax = 0;
	    for (int i = 0; i< m_Hist.GetNBits();i++)
	    {
		if (m_pBitCounts[0][i] > nMax)
		    nMax = m_pBitCounts[0][i];

	    }
	    m_wndHistWindow.Reset(m_pBitCounts, 
		m_Hist.GetNBits(),
		nMax, 1);

	} 
	else
	{
	    m_wndHistWindow.Reset(m_Hist.GetHistogramArray(), 
		m_Hist.GetNBins(),
		m_Hist.GetModeValue(), m_Hist.GetNColors());

	}



	m_wndHistWindow.Invalidate();
	m_wndHistWindow.UpdateWindow();


	UpdateStatistics();

    }

}

void CHistDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    m_bVisible = bShow;

    CDialog::OnShowWindow(bShow, nStatus);

    // TODO: Add your message handler code here

}

void CHistDialog::OnPixelValues() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_bVisible)
	Compute(NULL);

}

void CHistDialog::OnClose() 
{
    // TODO: Add your message handler code here and/or call default
    m_bVisible = FALSE;
    CDialog::OnClose();
}

LRESULT CHistDialog::OnPlotButton(WPARAM wparam, LPARAM lparam)
{
    // lparam is bin value of histogram
    CString s;

    int nBin = (int) lparam;

    if (m_nDoPixels == 1)
    {

	s.Format("Bit %d = %d", nBin, m_pBitCounts[0][nBin]);

    } 
    else
    {
	s.Format("Grayscale %d =", nBin);
	CString s2;

	int **pHist = m_Hist.GetHistogramArray();

	if (!pHist)
	    return -1;

	for (int i=0;i<m_Hist.GetNColors();i++)
	{
	    s2.Format(" %d",pHist[i][nBin]);
	    s += s2;
	}
    }

    m_wndCursorValues.SetWindowText(s);

    return 0;
}

void CHistDialog::UpdateStatistics()
{
    CString s;

    m_sNPixels.Format("%d", m_Stats[0].m_nN);		
    m_wndSNPixels.SetWindowText(m_sNPixels);

    for (int i = 0; i< m_pImage->GetNColors();i++)
    {

	m_sMean[i].Format("%g", m_Stats[i].m_dMean);
	m_sSD[i].Format("%g", m_Stats[i].m_dSD);
	m_sMinPixel[i].Format("%d", m_Stats[i].m_nMin);		
	m_sMaxPixel[i].Format("%d", m_Stats[i].m_nMax);		
	m_sMedian[i].Format("%d", m_Stats[i].m_nMedian);		

	m_wndSSD[i].SetWindowText(m_sSD[i]);
	m_wndSMean[i].SetWindowText(m_sMean[i]);
	m_wndSMedian[i].SetWindowText(m_sMedian[i]);
	m_wndSMinPixel[i].SetWindowText(m_sMinPixel[i]);
	m_wndSMaxPixel[i].SetWindowText(m_sMaxPixel[i]);

    }


}
