// DalsaCameraControl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "edtdefines.h"
#include "PdvInput.h"

#include "DalsaCameraControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDalsaCameraControl dialog


CDalsaCameraControl::CDalsaCameraControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDalsaCameraControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDalsaCameraControl)
	m_nExposePercent = 0;
	m_nLineRate = 0;
	m_dExposeTime = 0.0;
	m_dTotalTime = 0.0;
	//}}AFX_DATA_INIT

	m_pCamera = NULL;
}


void CDalsaCameraControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if (!pDX->m_bSaveAndValidate)
	{
		UpdateSecondaryValues();
	}
	double oldexpose = m_dExposeTime;
	double oldtotal = m_dTotalTime;

	DDX_Text(pDX, IDC_EDIT_EXPOSURE, m_dExposeTime);
	DDX_Text(pDX, IDC_EDIT_LINERATE, m_dTotalTime);

	if (pDX->m_bSaveAndValidate)
	{
		if (oldexpose != m_dExposeTime)
		{	
			if (m_dTotalTime < m_dExposeTime)
				m_dTotalTime = m_dExposeTime;
		} 
		else if (oldtotal != m_dTotalTime)
		{
			if (m_dTotalTime < m_dExposeTime)
				 m_dExposeTime = m_dTotalTime;
		}

		UpdateSecondaryValues();
	}

	
	DDX_Text(pDX, IDC_EXPOSE_PERCENT, m_nExposePercent);
	DDX_Text(pDX, IDC_LINE_RATE, m_nLineRate);
	//{{AFX_DATA_MAP(CDalsaCameraControl)
	DDX_Control(pDX, IDC_LINERATE_SCROLL, m_wndTotalTimeScroll);
	DDX_Control(pDX, IDC_EXPOSURE_SCROLL, m_wndExposeScroll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDalsaCameraControl, CDialog)
	//{{AFX_MSG_MAP(CDalsaCameraControl)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDalsaCameraControl message handlers

void CDalsaCameraControl::OnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_pCamera)
	{
		int hskip;
		int hactv;

		hskip = (int) (m_dInactiveTime / 0.05);
		hactv = (int) (m_dExposeTime / 0.05);

		m_pCamera->DalsaSetLSExposure( hskip, hactv);

		TRACE("Setting hskip = %d hactv = %d\n", hskip, hactv);

	}
	
}

void CDalsaCameraControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default		
	int nControl = pScrollBar->GetDlgCtrlID();
	CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
	CString s;

	switch(nControl) {

		case IDC_EXPOSURE_SCROLL:

			m_dExposeTime = pControl->GetPos() * 0.05;
			if (m_dExposeTime > m_dTotalTime)
				m_dTotalTime = m_dExposeTime;

			UpdateData(FALSE);

			if (nSBCode == SB_ENDSCROLL) 
			{
				//SetGain();
			}

		break;
		
		case IDC_LINERATE_SCROLL:
			m_dTotalTime = pControl->GetPos() * 0.05;
			if (m_dExposeTime > m_dTotalTime)
				m_dExposeTime = m_dTotalTime;

			UpdateData(FALSE);
			if (nSBCode == SB_ENDSCROLL) 
			{
			//	SetLevel();
			}
			
		break;

	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDalsaCameraControl::GetCurrentValues()
{

	if (m_pCamera)
	{
		int hactv = 0, hskip = 0;
		m_pCamera->DalsaGetLSExposure(hskip, hactv);

		m_dExposeTime = hactv * 0.05;
		m_dInactiveTime = hskip * 0.05;
		m_dTotalTime = m_dExposeTime + m_dInactiveTime;		
	}

	UpdateSecondaryValues();

}

BOOL CDalsaCameraControl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (m_wndExposeScroll.m_hWnd)
		m_wndExposeScroll.SetRange(0,65535);

	if (m_wndTotalTimeScroll.m_hWnd)
		m_wndTotalTimeScroll.SetRange(0,65535);

	GetCurrentValues();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDalsaCameraControl::UpdateSecondaryValues()
{
	if (m_wndExposeScroll.m_hWnd)
		m_wndExposeScroll.SetPos((int) (m_dExposeTime / 0.05));
	if (m_wndTotalTimeScroll.m_hWnd)
		m_wndTotalTimeScroll.SetPos((int) (m_dTotalTime / 0.05));
	m_dInactiveTime = m_dTotalTime - m_dExposeTime;

	if (m_dTotalTime > 0)
	{
		m_nLineRate = (int) (1000000.0 / m_dTotalTime);
		m_nExposePercent = (int) (100 * (m_dExposeTime / m_dTotalTime));
	}

}

void CDalsaCameraControl::SetCamera(PdvInput *pCamera)
{
	m_pCamera = pCamera;
}	

BOOL CDalsaCameraControl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreCreateWindow(cs);
}

int CDalsaCameraControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
