// ContrastDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Resource.h"

#include "ContrastDlg.h"


#include "EdtImgMinMax.h"

#include "PdvInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContrastDlg dialog


CContrastDlg::CContrastDlg(CWnd* pParent /*=NULL*/)
: CDialog(CContrastDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CContrastDlg)
    m_nHighValue = 0;
    m_nLowValue = 0;
    m_nMapType = 0;
    m_nColor = 0;
    m_dGammaValue = 1.0;
    m_nLutMode = -1;
    //}}AFX_DATA_INIT

    m_pImageWindow = NULL;

    m_pCamera = NULL;

}


void CContrastDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CContrastDlg)
    DDX_Control(pDX, IDC_GAMMA_SLIDER, m_wndGammaSlider);
    DDX_Control(pDX, IDC_RGB_GREEN, m_wndGreenButton);
    DDX_Control(pDX, IDC_RGB_BLUE, m_wndBlueButton);
    DDX_Control(pDX, IDC_RGB_RED, m_wndRedButton);
    DDX_Control(pDX, IDC_LOW_CTRST_SLIDER, m_wndLowSlider);
    DDX_Control(pDX, IDC_HIGH_CTRST_SLIDER, m_wndHighSlider);
    DDX_Text(pDX, IDC_HIGH_CTRST_EDIT, m_nHighValue);
    DDX_Text(pDX, IDC_LOW_CTRST_EDIT, m_nLowValue);
    DDX_Radio(pDX, IDC_MODE_LINEAR, m_nMapType);
    DDX_Radio(pDX, IDC_RGB_RED, m_nColor);
    DDX_Text(pDX, IDC_GAMMA_VALUE, m_dGammaValue);
    DDV_MinMaxDouble(pDX, m_dGammaValue, 1.e-003, 5.);
    DDX_Radio(pDX, IDC_GRAYSCALE, m_nLutMode);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContrastDlg, CDialog)
    //{{AFX_MSG_MAP(CContrastDlg)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_APPLY_TO_LIVE, OnApplyToLive)
    ON_BN_CLICKED(IDC_RESET_LIVE, OnResetLive)
    ON_BN_CLICKED(IDC_MODE_LINEAR, OnMapMode)
    ON_BN_CLICKED(IDC_DO_HEQ, OnDoHeq)
    ON_BN_CLICKED(IDC_USER_SET, OnUserSet)
    ON_BN_CLICKED(IDC_SQRT, OnSqrt)
    ON_BN_CLICKED(IDC_AUTO_LINEAR, OnAutoLinear)
    ON_BN_CLICKED(IDC_HEQ, OnHeq)
    ON_BN_CLICKED(IDC_RGB_BLUE, OnRgbBlue)
    ON_BN_CLICKED(IDC_RGB_GREEN, OnRgbGreen)
    ON_BN_CLICKED(IDC_RGB_RED, OnRgbRed)
    ON_BN_CLICKED(IDC_GAMMA, OnGamma)
    ON_BN_CLICKED(IDC_INFRARED, OnInfrared)
    ON_BN_CLICKED(IDC_GRAYSCALE, OnGrayscale)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContrastDlg message handlers

void CContrastDlg::SetImageWindow(CEdtImageWin * pWindow)
{
    m_pImageWindow = pWindow;

    if (m_pImageWindow)
    {

	EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

	if (pDS)
	{
	    m_nLowValue = pDS->GetMinValue(m_nColor);
	    m_nHighValue = pDS->GetMaxValue(m_nColor);
	    m_dGammaValue = pDS->GetGamma(m_nColor);

	    m_nMapType = pDS->GetMapType();

	}
    }

    RefreshImageValues();
}

void CContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // TODO: Add your message handler code here and/or call default
    int nControl = pScrollBar->GetDlgCtrlID();
    CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
    CString s;

    switch(nControl) {

		case IDC_HIGH_CTRST_SLIDER:
		    m_nHighValue = pControl->GetPos();
		    UpdateData(FALSE);
		    if (nSBCode == SB_ENDSCROLL) 
			RefreshLowHigh();
		    break;

		case IDC_LOW_CTRST_SLIDER:
		    m_nLowValue = pControl->GetPos();
		    UpdateData(FALSE);
		    if (nSBCode == SB_ENDSCROLL) 
			RefreshLowHigh();
		    break;

		case IDC_GAMMA_SLIDER:
		    int nGamma = pControl->GetPos();

		    m_dGammaValue = nGamma / 1000.0;
		    if (m_nMapType != EdtLut::Gamma)
			SetMapType(EdtLut::Gamma);

		    UpdateData(FALSE);

		    if (nSBCode == SB_ENDSCROLL) 
			RefreshLowHigh();
		    break;
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CContrastDlg::RefreshLowHigh()
{
    EdtImage *pImage = GetCurrentImage();

    if (pImage)
    {

	EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

	pImage->SetDataChanged();

	if (pDS)
	{
	    if (pDS->GetMapType() == EdtLut::AutoLinear)
	    {

		EdtImgMinMax MinMax;

		MinMax.Compute(pImage,m_nLowValue, m_nHighValue,m_nColor);

		m_pImageWindow->RedrawWindow(NULL,NULL,RDW_INVALIDATE);

		m_wndLowSlider.SetPos(m_nLowValue);
		m_wndHighSlider.SetPos(m_nHighValue);

		UpdateData(FALSE);
	    }
	    else
	    {
		pDS->SetMinValue(m_nLowValue,m_nColor);
		pDS->SetMaxValue(m_nHighValue,m_nColor);
		pDS->SetAllGamma(m_dGammaValue);
		pDS->UpdateMap(pImage);
		m_pImageWindow->RedrawWindow(NULL,NULL,RDW_INVALIDATE);
	    }
	}
    }
}

BOOL CContrastDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    RefreshImageValues();

    EdtImage *pImage = GetCurrentImage();

    if (pImage && (pImage->GetNColors() == 1))
    {
	m_wndRedButton.EnableWindow(0);
	m_wndGreenButton.EnableWindow(0);
	m_wndBlueButton.EnableWindow(0);
    }

    return TRUE;

}


void CContrastDlg::OnApplyToLive() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    EdtImage *pImage = GetCurrentImage();

    if (pImage)
    {

	EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

	if (pDS)
	{

	    byte *pLut = (byte *) pDS->GetLutElement(0);
	    if (m_pCamera)
		m_pCamera->SetLiveLut(pLut);

	}
    }

}	

void CContrastDlg::OnResetLive() 
{
    // TODO: Add your control notification handler code here
    if (m_pCamera)
	m_pCamera->SetLiveLut(NULL);
}

void CContrastDlg::OnMapMode() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    EdtImage *pImage = GetCurrentImage();

    if (pImage)
    {

	EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

	if (pDS)
	{

	    pDS->SetMapType((EdtLut::MapType)m_nMapType);

	    RefreshLowHigh();
	}
    }		
}

void CContrastDlg::OnDoHeq() 
{
    // TODO: Add your control notification handler code here
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    EdtImage *pImage = GetCurrentImage();

    {

	EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

	if (pDS)
	{

	    pDS->UpdateMapHeq(pImage);

	    if (pImage)
		pImage->SetDataChanged();

	    m_pImageWindow->RedrawWindow(NULL,NULL,RDW_INVALIDATE);

	}
    }

}

void CContrastDlg::OnUserSet() 
{
    // TODO: Add your control notification handler code here
    OnMapMode();
}

void CContrastDlg::OnSqrt() 
{
    // TODO: Add your control notification handler code here
    OnMapMode();	
}

void CContrastDlg::OnAutoLinear() 
{
    // TODO: Add your control notification handler code here
    OnMapMode();	
}

void CContrastDlg::OnHeq() 
{
    // TODO: Add your control notification handler code here
    OnMapMode();	
}

void CContrastDlg::OnRgbBlue() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    RefreshImageValues();
    UpdateData(FALSE);
}

void CContrastDlg::OnRgbGreen() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    RefreshImageValues();
    UpdateData(FALSE);
}

void CContrastDlg::OnRgbRed() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    RefreshImageValues();
    UpdateData(FALSE);
}

void CContrastDlg::RefreshImageValues()
{
    EdtImage *pImage = GetCurrentImage();
    if (pImage && m_hWnd)
    {
	EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

	int nLow;
	int nHigh;

	nLow	= pImage->GetMinValue();
	nHigh	= pImage->GetMaxValue();

	m_wndLowSlider.SetRange(nLow, nHigh);
	m_wndHighSlider.SetRange(nLow, nHigh);
	m_wndGammaSlider.SetRange(1,5000);
	m_wndGammaSlider.SetTicFreq(200);

	if (pDS)
	{
	    m_nLowValue = pDS->GetMinValue(m_nColor);
	    m_nHighValue = pDS->GetMaxValue(m_nColor);
	    m_dGammaValue = pDS->GetGamma(m_nColor);
	}


	m_wndLowSlider.SetPos(m_nLowValue);
	m_wndHighSlider.SetPos(m_nHighValue);

	m_wndGammaSlider.SetPos((int) (m_dGammaValue * 1000));
    }
}

EdtImage * CContrastDlg::GetCurrentImage()
{

    if (m_pImageWindow)
	return m_pImageWindow->GetBaseImage();
    else
	return NULL;
}

void CContrastDlg::OnGamma() 
{

    OnMapMode();

}

void CContrastDlg::SetMapType(int nMapType)
{
    m_nMapType = nMapType;


    EdtScaledLut *pDS = m_pImageWindow->GetDisplayScale();

    if (pDS)
    {

	pDS->SetMapType((EdtLut::MapType)m_nMapType);
    }

}

void CContrastDlg::OnInfrared() 
{
    // TODO: Add your control notification handler code here
    EdtImage *pImage = GetCurrentImage();

    m_pImageWindow->SetPaletteInfrared();

    if (pImage)
	pImage->SetDataChanged();

    m_pImageWindow->RedrawWindow(NULL,NULL,RDW_INVALIDATE);


}

void CContrastDlg::OnGrayscale() 
{
    // TODO: Add your control notification handler code here
    // TODO: Add your control notification handler code here
    EdtImage *pImage = GetCurrentImage();

    m_pImageWindow->SetPaletteLinear();

    if (pImage)
	pImage->SetDataChanged();

    m_pImageWindow->RedrawWindow(NULL,NULL,RDW_INVALIDATE);

}
