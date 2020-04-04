// WhiteBalanceSetup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WhiteBalanceSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWhiteBalanceSetup dialog


CWhiteBalanceSetup::CWhiteBalanceSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CWhiteBalanceSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWhiteBalanceSetup)
	m_bAutoRange = TRUE;
	m_nMaxValue = 254;
	m_nMinValue = 1;
	m_bUseRatios = FALSE;
	m_dSamplePercent = 10.0;
	//}}AFX_DATA_INIT
}


void CWhiteBalanceSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhiteBalanceSetup)
	DDX_Check(pDX, IDC_AUTORANGE, m_bAutoRange);
	DDX_Text(pDX, IDC_MAXVALUE, m_nMaxValue);
	DDX_Text(pDX, IDC_MINVALUE, m_nMinValue);
	DDX_Check(pDX, IDC_USE_RATIOS, m_bUseRatios);
	DDX_Text(pDX, IDC_SAMPLE_PERCENT, m_dSamplePercent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhiteBalanceSetup, CDialog)
	//{{AFX_MSG_MAP(CWhiteBalanceSetup)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhiteBalanceSetup message handlers
