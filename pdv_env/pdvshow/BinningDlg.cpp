// BinningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BinningDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBinningDlg dialog


CBinningDlg::CBinningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBinningDlg)
	m_nHbin = 0;
	m_nHeight = 0;
	m_nWidth = 0;
	//}}AFX_DATA_INIT

	m_pCamera = 0;
}


void CBinningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBinningDlg)
	DDX_Radio(pDX, IDC_HBIN1, m_nHbin);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBinningDlg, CDialog)
	//{{AFX_MSG_MAP(CBinningDlg)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_BN_CLICKED(IDC_HBIN1, OnHbin1)
	ON_BN_CLICKED(IDC_HBIN2, OnHbin1)
	ON_BN_CLICKED(IDC_HBIN4, OnHbin1)
	ON_BN_CLICKED(IDC_HBIN8, OnHbin1)
	ON_BN_CLICKED(IDC_HBIN1X2, OnHbin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBinningDlg message handlers



void CBinningDlg::OnApply() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	// Set the binning factors

	if (m_pCamera)
	{
		int xval = 1;
		int yval = 1;

		switch (m_nHbin) {

		case 0:
			xval = 1;
			yval = 1;
			break;

		case 1:
			xval = 1;
			yval = 2;
			break;

		case 2:
			xval = 2;
			yval = 2;
			break;

		case 3:
			xval = 4;
			yval = 4;
			break;

		case 4:
			xval = 8;
			yval = 8;
			break;

			
		}
		
		m_pCamera->SetBinning(xval, yval);

		m_nWidth = m_pCamera->GetWidth();
		m_nHeight = m_pCamera->GetHeight();


		if (m_pFire)
		{
			m_pFire(m_pFireTarget);
		}

		UpdateData(FALSE);
	}

}

void CBinningDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CDialog::OnOK();
}



void CBinningDlg::OnHbin1() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

}



void CBinningDlg::UpdateFromCamera(PdvInput * pCamera)
{
	if (pCamera)
	{
		m_pCamera = pCamera;
	}

	if (!m_pCamera)
		return;

	m_nWidth = m_pCamera->GetWidth();
	m_nHeight = m_pCamera->GetHeight();

	int xval = m_pCamera->GetBinX();
	int yval = m_pCamera->GetBinY();

	switch (xval)
	{
	case 0:

	case 1:
		if (yval == 2)
			m_nHbin = 1;
		else
			m_nHbin = 0;
	break;

	case 2:
		m_nHbin = 2;
		break;

	case 4:
		m_nHbin = 3;
		break;

	case 8:
		m_nHbin = 4;
		break;
	
	}
	if (m_hWnd)
	{

		UpdateData(FALSE);
	}
}
