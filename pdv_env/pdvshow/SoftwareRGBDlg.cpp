// SoftwareRGBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "edtdefines.h"
#include "edtimage.h"

extern "C" {

#include "libpdv.h"

}

#include "SoftwareRGBDlg.h"

#include "colorbalance.h"

#include "WhiteBalanceSetup.h"

#include "bayer_filter_asm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int n_loops = 1;

/////////////////////////////////////////////////////////////////////////////
// CSoftwareRGBDlg dialog


CSoftwareRGBDlg::CSoftwareRGBDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSoftwareRGBDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSoftwareRGBDlg)
    m_bGreenPixelFirst = FALSE;
    m_bRedRowFirst = FALSE;
    m_dRGBGamma = 1.0;
    m_dScaleBlue = 1.0;
    m_dScaleGreen = 1.0;
    m_dScaleRed = 1.0;
    m_nBlackOffset = 0;
    m_dSpeedValue = 0.0;
    m_nQuality = 1;
    m_bUseGamma = FALSE;
    m_nBias = 12;
    m_bGradientColor = TRUE;
    //}}AFX_DATA_INIT

    // DVC values for testing

    m_nSourceDepth = 10;
    m_bGreenPixelFirst = 1;
    m_bRedRowFirst = 0;

    m_pSrcTest = NULL;
    m_pDestTest = NULL;

    m_pRequestTarget = NULL;
    m_RequestImage = NULL;

    m_UpdateTestImage = NULL; 
    m_pUpdateTestTarget = NULL;

    m_pCamera = NULL;

    m_bShowTest = FALSE;

    m_WBDlg = NULL;

}

CSoftwareRGBDlg::~CSoftwareRGBDlg()

{
    if (m_WBDlg)
	delete m_WBDlg;

}


void CSoftwareRGBDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSoftwareRGBDlg)
    DDX_Control(pDX, IDC_BIAS_SPIN, m_wndBiasSpin);
    DDX_Control(pDX, IDC_BIAS_SLIDER, m_wndBiasSlider);
    DDX_Control(pDX, IDC_BLACK_OFFSET_SLIDER, m_wndBlackOffsetSlider);
    DDX_Control(pDX, IDC_SCALE_RED_SLIDER, m_wndScaleRedSlider);
    DDX_Control(pDX, IDC_SCALE_GREEN_SLIDER, m_wndScaleGreenSlider);
    DDX_Control(pDX, IDC_SCALE_BLUE_SLIDER, m_wndScaleBlueSlider);
    DDX_Control(pDX, IDC_GAMMA_SLIDER, m_wndGammaSlider);
    DDX_Check(pDX, IDC_GREEN_PIXEL_FIRST, m_bGreenPixelFirst);
    DDX_Check(pDX, IDC_RED_ROW_FIRST, m_bRedRowFirst);
    DDX_Text(pDX, IDC_RGB_GAMMA, m_dRGBGamma);
    DDX_Text(pDX, IDC_SCALE_BLUE, m_dScaleBlue);
    DDX_Text(pDX, IDC_SCALE_GREEN, m_dScaleGreen);
    DDX_Text(pDX, IDC_SCALE_RED, m_dScaleRed);
    DDX_Text(pDX, IDC_BLACK_OFFSET, m_nBlackOffset);
    DDX_Text(pDX, IDC_SPEED_VALUE, m_dSpeedValue);
    DDX_Radio(pDX, IDC_QUALITY_0, m_nQuality);
    DDX_Check(pDX, IDC_USE_GAMMA, m_bUseGamma);
    DDX_Text(pDX, IDC_BIAS_VALUE, m_nBias);
    DDV_MinMaxInt(pDX, m_nBias, 0, 255);
    DDX_Check(pDX, IDC_GRADIENT_COLOR, m_bGradientColor);
    //}}AFX_DATA_MAP

    if (pDX->m_bSaveAndValidate)
    {
	m_wndGammaSlider.SetPos((int) (m_dRGBGamma * 1000));
	m_wndScaleRedSlider.SetPos((int) (m_dScaleRed * 1000));
	m_wndScaleGreenSlider.SetPos((int) (m_dScaleGreen * 1000));
	m_wndScaleBlueSlider.SetPos((int) (m_dScaleBlue * 1000));
	m_wndBiasSlider.SetPos((int) (m_nBias));
    }
}


BEGIN_MESSAGE_MAP(CSoftwareRGBDlg, CDialog)
    //{{AFX_MSG_MAP(CSoftwareRGBDlg)
    ON_BN_CLICKED(IDAPPLY, OnApply)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_SAMPLE_WB, OnSampleWb)
    ON_BN_CLICKED(IDC_TEST_BUTTON, OnTestButton)
    ON_BN_CLICKED(IDC_USE_GAMMA, OnUseGamma)
    ON_BN_CLICKED(IDC_QUALITY_1, OnQuality1)
    ON_BN_CLICKED(IDC_QUALITY_0, OnQuality0)
    ON_BN_CLICKED(IDC_QUALITY_2, OnQuality2)
    ON_BN_CLICKED(IDC_WB_DETAILS, OnWbDetails)
    ON_BN_CLICKED(IDC_RED_ROW_FIRST, OnBnClickedRedRowFirst)
    ON_BN_CLICKED(IDC_GREEN_PIXEL_FIRST, OnBnClickedGreenPixelFirst)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoftwareRGBDlg message handlers

void CSoftwareRGBDlg::OnApply() 
{
    // TODO: Add your control notification handler code here
    double rgb_scale[3];

    if (m_hWnd)
	UpdateData(TRUE);

    rgb_scale[0] = m_dScaleRed;
    rgb_scale[1] = m_dScaleGreen;
    rgb_scale[2] = m_dScaleBlue;

    if (m_pCamera)
    {
	m_pCamera->SetBayerParameters(m_nSourceDepth,
	    rgb_scale,
	    m_dRGBGamma,
	    m_nBlackOffset,
	    m_bRedRowFirst,
	    m_bGreenPixelFirst,
	    m_nQuality,
	    m_nBias,
	    m_bGradientColor);

    }

}

void CSoftwareRGBDlg::SetCamera(PdvInput *pCamera)

{
    m_pCamera = pCamera;

    if (m_pCamera)
    {
	bool redrow = (m_bRedRowFirst != 0), 
	    greenpixel = (m_bGreenPixelFirst != 0);

	m_pCamera->GetBayerOrder(redrow,greenpixel);

	m_bRedRowFirst = redrow;
	m_bGreenPixelFirst = greenpixel;

	if (m_hWnd)
	    UpdateData(FALSE);
    }

}

void CSoftwareRGBDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // TODO: Add your message handler code here and/or call default
    int nControl = pScrollBar->GetDlgCtrlID();
    CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
    CString s;
    int nGamma;

    static int incall = FALSE;

    switch(nControl) {

	case IDC_GAMMA_SLIDER:
	    nGamma= pControl->GetPos();
	    UpdateData(TRUE);

	    if (m_bUseGamma)
		m_dRGBGamma = nGamma * 0.001;

	    UpdateData(FALSE);

	    if (nSBCode == SB_ENDSCROLL) 
	    {
		OnApply();
	    }


	    break;
	case IDC_BLACK_OFFSET_SLIDER:
	    nGamma= pControl->GetPos();
	    UpdateData(TRUE);

	    m_nBlackOffset = nGamma;


	    UpdateData(FALSE);

	    if (nSBCode == SB_ENDSCROLL) 
	    {
		OnApply();
	    }


	    break;	
	case IDC_SCALE_RED_SLIDER:
	    nGamma= pControl->GetPos();
	    UpdateData(TRUE);

	    m_dScaleRed = nGamma * 0.001;


	    UpdateData(FALSE);

	    if (nSBCode == SB_ENDSCROLL) 
	    {
		OnApply();
	    }


	    break;
	case IDC_SCALE_GREEN_SLIDER:
	    nGamma= pControl->GetPos();
	    UpdateData(TRUE);

	    m_dScaleGreen = nGamma * 0.001;


	    UpdateData(FALSE);

	    if (nSBCode == SB_ENDSCROLL) 
	    {
		OnApply();
	    }


	    break;

	case IDC_SCALE_BLUE_SLIDER:
	    nGamma= pControl->GetPos();
	    UpdateData(TRUE);

	    m_dScaleBlue = nGamma * 0.001;


	    UpdateData(FALSE);

	    if (nSBCode == SB_ENDSCROLL) 
	    {
		OnApply();
	    }


	    break;
	case IDC_BIAS_SLIDER:

	    nGamma= pControl->GetPos();

	    UpdateData(TRUE);

	    m_nBias = nGamma;

	    UpdateData(FALSE);

	    if (nSBCode == SB_ENDSCROLL) 
	    {
		OnApply();
	    }


	    break;
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSoftwareRGBDlg::SetCameraValues(int src_depth, int red_row_first, int green_pixel_first)

{
    m_bGreenPixelFirst = green_pixel_first;
    m_bRedRowFirst = red_row_first;
    m_nSourceDepth = src_depth;

    if (m_hWnd)
	UpdateData(FALSE);
}

BOOL CSoftwareRGBDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    m_wndGammaSlider.SetRange(0,5000);
    m_wndScaleBlueSlider.SetRange(0,10000);
    m_wndScaleGreenSlider.SetRange(0,10000);
    m_wndScaleRedSlider.SetRange(0,10000);
    m_wndBlackOffsetSlider.SetRange(0,255);
    m_wndBiasSlider.SetRange(0,255);

    m_wndGammaSlider.SetPos(1000);
    m_wndScaleBlueSlider.SetPos(1000);
    m_wndScaleGreenSlider.SetPos(1000);
    m_wndScaleRedSlider.SetPos(1000);

    m_wndBiasSlider.SetPos(m_nBias);

    m_wndBiasSpin.SetRange(255,0);

    EnableTesting(m_bShowTest);
    SetUseGammaState();
    SetUseBiasState();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

int CSoftwareRGBDlg::UpdateImage(void *pObj, EdtImage *pImage)

{

    CSoftwareRGBDlg *pThis = (CSoftwareRGBDlg *) pObj;

    return pThis->ComputeWB(pImage);

}

int CSoftwareRGBDlg::ComputeWB(EdtImage *pImage)

{
    if (pImage == 0)
	return -1;

    double ratios[3];

    UpdateData(TRUE);

    EdtColorBalance balance;

    if (!m_WBDlg)
	m_WBDlg = new CWhiteBalanceSetup;

    balance.m_nMinValue = m_WBDlg->m_nMinValue;
    balance.m_nMaxValue = m_WBDlg->m_nMaxValue;
    balance.m_bAutoRange = (m_WBDlg->m_bAutoRange != 0);
    balance.m_dAutoPercent = m_WBDlg->m_dSamplePercent;

    if (balance.ComputeColorBalance(pImage, ratios, m_WBDlg->m_bUseRatios?CB_RATIOS:CB_MEANS) == 0) 
    {

	m_dScaleRed = ratios[0];
	m_dScaleGreen = 1;
	m_dScaleBlue = ratios[2];

	if (m_WBDlg->m_bAutoRange)
	{

	    m_WBDlg->m_nMinValue = balance.m_nMinValue;
	    m_WBDlg->m_nMaxValue = balance.m_nMaxValue;

	}


	UpdateData(FALSE);

	OnApply();

    }

    return 0;

}

void CSoftwareRGBDlg::OnSampleWb() 
{
    // TODO: Add your control notification handler code here

    if (m_RequestImage && m_pRequestTarget && m_pCamera)
    {

	// Set ratios to 1
	double rgb_scale[3];
	rgb_scale[0] = 1.0;
	rgb_scale[1] = 1.0;
	rgb_scale[2] = 1.0;

	m_pCamera->SetBayerParameters(m_nSourceDepth,
	    rgb_scale,
	    1.0,
	    m_nBlackOffset,
	    m_bRedRowFirst,
	    m_bGreenPixelFirst,
	    m_nQuality,
	    m_nBias,
	    m_bGradientColor);


	m_RequestImage(m_pRequestTarget, UpdateImage, this, TRUE);

    }

}

extern "C" {

    extern int
	convert_bayer_image_16_BGR(u_short * src, 
	int width, int rows, int pitch, 
	u_char * dest, 
	int shrink, int order, int depth);

}

void CSoftwareRGBDlg::OnTestButton() 
{
    // TODO: Add your control notification handler code here
    if (m_hWnd)
	UpdateData(TRUE);

    m_pCamera->SetBayerQuality(m_nQuality, m_bGradientColor != 0);

    if (m_pSrcTest && m_pDestTest) 
    {

	edt_dtime();

	u_char *pSrcData = (u_char *) m_pSrcTest->GetBaseData();
	u_char *pDestData = (u_char *) m_pDestTest->GetBaseData();

	if (pSrcData && pDestData)

	    for (int i = 0; i< n_loops; i++)
		convert_bayer_image_16_BGR(
		(u_short *) pSrcData, 
		m_pSrcTest->GetWidth(), 
		m_pSrcTest->GetHeight(),
		m_pSrcTest->GetPitchPixels(),
		pDestData,  
		0, 
		(m_bRedRowFirst << 1) | (m_bGreenPixelFirst),
		m_nSourceDepth);


	m_dSpeedValue = (double) ((int) (((edt_dtime() * 1000000)) / n_loops)) / 1000.0;

	if (m_UpdateTestImage && m_pUpdateTestTarget)
	{
	    m_UpdateTestImage(m_pUpdateTestTarget);
	}
    }
    if (m_hWnd)
	UpdateData(FALSE);
}

void CSoftwareRGBDlg::EnableTesting(bool on)
{

    if (m_hWnd)
    {

	CWnd *item1 = GetDlgItem(IDC_SPEED_VALUE);
	CWnd *item2 = GetDlgItem(IDC_TEST_BUTTON);
	CWnd *item3 = GetDlgItem(IDC_GRADIENT_COLOR);

	if (item1)
	    item1->ShowWindow((on)? SW_SHOW:SW_HIDE);
	if (item2)
	    item2->ShowWindow((on)? SW_SHOW:SW_HIDE);

	if (item3)
	    item3->ShowWindow((on)? SW_SHOW:SW_HIDE);

    }

    m_bShowTest = on;

}

void CSoftwareRGBDlg::OnUseGamma() 
{
    // TODO: Add your control notification handler code here
    if (m_hWnd)
    {

	UpdateData(TRUE);

	SetUseGammaState();

	UpdateData(FALSE);

	OnApply();

    }

}

void CSoftwareRGBDlg::SetUseGammaState()
{
    if (m_hWnd)
    {
	if (!m_bUseGamma)
	    m_dRGBGamma = 1.0;


	CWnd *item1 = GetDlgItem(IDC_RGB_GAMMA);
	CWnd *item2 = GetDlgItem(IDC_GAMMA_SLIDER);

	if (item1)
	    item1->EnableWindow((m_bUseGamma)? SW_SHOW:SW_HIDE);
	if (item2)
	    item2->EnableWindow((m_bUseGamma)? SW_SHOW:SW_HIDE);

    }
}

void CSoftwareRGBDlg::OnQuality1() 
{
    // TODO: Add your control notification handler code here
    OnApply();
    SetUseBiasState();
}

void CSoftwareRGBDlg::OnQuality0() 
{
    // TODO: Add your control notification handler code here
    OnApply();
    SetUseBiasState();
}

void CSoftwareRGBDlg::OnQuality2() 
{
    // TODO: Add your control notification handler code here
    OnApply();
    SetUseBiasState();
}

void CSoftwareRGBDlg::SetUseBiasState()
{
    if (m_hWnd)
    {


	CWnd *item1 = GetDlgItem(IDC_BIAS_VALUE);
	CWnd *item2 = GetDlgItem(IDC_BIAS_SLIDER);
	CWnd *item3 = GetDlgItem(IDC_BIAS_SPIN);

	if (item1)
	    item1->EnableWindow((m_nQuality == 2)? SW_SHOW:SW_HIDE);
	if (item2)
	    item2->EnableWindow((m_nQuality == 2)? SW_SHOW:SW_HIDE);
	if (item3)
	    item3->EnableWindow((m_nQuality == 2)? SW_SHOW:SW_HIDE);

    }
}

void CSoftwareRGBDlg::OnWbDetails() 
{
    // TODO: Add your control notification handler code here

    if (!m_WBDlg)
	m_WBDlg = new CWhiteBalanceSetup;

    m_WBDlg->DoModal();

}

void CSoftwareRGBDlg::OnBnClickedRedRowFirst()
{
    // TODO: Add your control notification handler code here
    OnApply();
}

void CSoftwareRGBDlg::OnBnClickedGreenPixelFirst()
{
    // TODO: Add your control notification handler code here
    OnApply();
}
