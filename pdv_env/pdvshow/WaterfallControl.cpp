// WaterfallControl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "WaterfallControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaterfallControl dialog


CWaterfallControl::CWaterfallControl(CWnd* pParent /*=NULL*/)
	: CDialog(CWaterfallControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaterfallControl)
	m_nHeight = 0;
	m_nRow0 = 0;
	m_nRow1 = 0;
	m_nRow2 = 0;
	m_bUseRgbOut = FALSE;
	m_nWidth = 0;
	m_bWidthFromCamera = FALSE;
	m_bActivateWaterfall = FALSE;
	m_nDecimate = 1;
	//}}AFX_DATA_INIT

	m_pSrcImage = NULL;
	m_pDisplay = NULL;


}


void CWaterfallControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaterfallControl)
	DDX_Control(pDX, IDC_DECIMATE_SPIN, m_wndDecimateSpin);
	DDX_Control(pDX, IDC_ROW1, m_wndRow1);
	DDX_Control(pDX, IDC_ROW2, m_wndRow2);
	DDX_Control(pDX, IDC_WIDTH_SPIN, m_wndWidthSpin);
	DDX_Control(pDX, IDC_ROW2_SPIN, m_wndRow2Spin);
	DDX_Control(pDX, IDC_ROW1_SPIN, m_wndRow1Spin);
	DDX_Control(pDX, IDC_ROW0_SPIN, m_wndRow0Spin);
	DDX_Control(pDX, IDC_HEIGHT_SPIN, m_wndHeightSpin);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_ROW0, m_nRow0);
	DDX_Text(pDX, IDC_ROW1, m_nRow1);
	DDX_Text(pDX, IDC_ROW2, m_nRow2);
	DDX_Check(pDX, IDC_USE_RGB_OUT, m_bUseRgbOut);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDX_Check(pDX, IDC_WIDTH_FROM_CAMERA, m_bWidthFromCamera);
	DDX_Check(pDX, IDC_ACTIVATE_WATERFALL, m_bActivateWaterfall);
	DDX_Text(pDX, IDC_DECIMATE, m_nDecimate);
	DDV_MinMaxInt(pDX, m_nDecimate, 1, 32);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CWaterfallControl, CDialog)
	//{{AFX_MSG_MAP(CWaterfallControl)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_USE_RGB_OUT, OnUseRgbOut)
	ON_BN_CLICKED(IDC_WIDTH_FROM_CAMERA, OnWidthFromCamera)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterfallControl message handlers

void CWaterfallControl::OnApply() 
{
	// TODO: Add your control notification handler code here
	if (m_pDisplay)
	{

		UpdateData(TRUE);

		m_pDisplay->SetWidth(m_nWidth);
		m_pDisplay->SetHeight(m_nHeight);

		if (m_bUseRgbOut)
			m_pDisplay->m_nNWaterfallSrcRows = 3;
		else
			m_pDisplay->m_nNWaterfallSrcRows = 1;


		m_pDisplay->m_nWaterfallSrcRows[0] = m_nRow0;

		if (m_bUseRgbOut)
		{
			m_pDisplay->m_nWaterfallSrcRows[1] = m_nRow1;
			m_pDisplay->m_nWaterfallSrcRows[2] = m_nRow2;				
		}

		m_pDisplay->m_bWaterfall = m_bActivateWaterfall;

		m_pDisplay->UpdateWaterfallImage(m_pSrcImage);

		m_pDisplay->m_nDecimate = m_nDecimate;

	}
}

void CWaterfallControl::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();

	CDialog::OnOK();
}

void CWaterfallControl::OnUseRgbOut() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_wndRow1.EnableWindow(m_bUseRgbOut);
	m_wndRow2.EnableWindow(m_bUseRgbOut);
	m_wndRow1Spin.EnableWindow(m_bUseRgbOut);
	m_wndRow2Spin.EnableWindow(m_bUseRgbOut);

}

void CWaterfallControl::OnWidthFromCamera() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_bWidthFromCamera)
	{
		if (m_pCamera)
		{
			m_nWidth = m_pCamera->GetWidth();
			if (m_nDecimate)
				m_nWidth /= m_nDecimate;
			UpdateData(FALSE);
		}
	}
	
}

BOOL CWaterfallControl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (m_bUseRgbOut)
	{
		m_wndRow1.EnableWindow(TRUE);
		m_wndRow2.EnableWindow(TRUE);
		m_wndRow1Spin.EnableWindow(TRUE);
		m_wndRow2Spin.EnableWindow(TRUE);
	}
	else
	{
		m_wndRow1.EnableWindow(FALSE);
		m_wndRow2.EnableWindow(FALSE);
		m_wndRow1Spin.EnableWindow(FALSE);
		m_wndRow2Spin.EnableWindow(FALSE);

	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
