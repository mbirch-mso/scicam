// RoiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "edtimage.h"
#include "RoiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoiDlg dialog


CRoiDlg::CRoiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoiDlg)
	m_sHActvMax = _T("");
	m_nHActv = 0;
	m_nHSkip = 0;
	m_sHSkipMax = _T("");
	m_nVActv = 0;
	m_sVActvMax = _T("");
	m_nVSkip = 0;
	m_sVSkipMax = _T("");
	//}}AFX_DATA_INIT

	m_pCamera = NULL;
	m_pFire = NULL;
	m_pFireTarget = NULL;

}


void CRoiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoiDlg)
	DDX_Control(pDX, IDC_VSKIP_SPIN, m_wndVSkipSpin);
	DDX_Control(pDX, IDC_VACTV_SPIN, m_wndVActvSpin);
	DDX_Control(pDX, IDC_HACTV_SPIN, m_wndHActvSpin);
	DDX_Control(pDX, IDC_HSKIP_SPIN, m_wndHSkipSpin);
	DDX_Control(pDX, IDC_HACTV_SLIDER, m_wndHActvSlider);
	DDX_Control(pDX, IDC_HSKIP_SLIDER, m_wndHSkipSlider);
	DDX_Control(pDX, IDC_VACTV_SLIDER, m_wndVActvSlider);
	DDX_Control(pDX, IDC_VSKIP_SLIDER, m_wndVSkipSlider);
	DDX_Text(pDX, IDC_HACTV_MAX, m_sHActvMax);
	DDX_Text(pDX, IDC_HACTV_EDIT, m_nHActv);
	DDX_Text(pDX, IDC_HSKIP_EDIT, m_nHSkip);
	DDX_Text(pDX, IDC_HSKIP_MAX, m_sHSkipMax);
	DDX_Text(pDX, IDC_VACTV_EDIT, m_nVActv);
	DDX_Text(pDX, IDC_VACTV_MAX, m_sVActvMax);
	DDX_Text(pDX, IDC_VSKIP_EDIT, m_nVSkip);
	DDX_Text(pDX, IDC_VSKIP_MAX, m_sVSkipMax);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		m_wndHSkipSlider.SetPos(m_nHSkip);
		m_wndHActvSlider.SetPos(m_nHActv);
		m_wndVSkipSlider.SetPos(m_nVSkip);
		m_wndVActvSlider.SetPos(m_nVActv);
	}

}


BEGIN_MESSAGE_MAP(CRoiDlg, CDialog)
	//{{AFX_MSG_MAP(CRoiDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_EN_KILLFOCUS(IDC_HACTV_EDIT, OnKillfocusHactvEdit)
	ON_EN_KILLFOCUS(IDC_HSKIP_EDIT, OnKillfocusHskipEdit)
	ON_EN_KILLFOCUS(IDC_VACTV_EDIT, OnKillfocusVactvEdit)
	ON_EN_KILLFOCUS(IDC_VSKIP_EDIT, OnKillfocusVskipEdit)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_HACTV_SPIN, OnDeltaposHactvSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HSKIP_SPIN, OnDeltaposHskipSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VACTV_SPIN, OnDeltaposVactvSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VSKIP_SPIN, OnDeltaposVskipSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoiDlg message handlers

void CRoiDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ApplyToCamera();

}

void CRoiDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}

void CRoiDlg::OnKillfocusHactvEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateData(FALSE);
	
}

void CRoiDlg::OnKillfocusHskipEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateData(FALSE);
	
}

void CRoiDlg::OnKillfocusVactvEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateData(FALSE);
	
}

void CRoiDlg::OnKillfocusVskipEdit() 
{
	// TODO: Add your control notification handler code here
	// Set the slider
	UpdateData(TRUE);
	UpdateData(FALSE);

}

void CRoiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	if (!pScrollBar)
		return;
	
	int nControl = pScrollBar->GetDlgCtrlID();
	CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
	CString s;
	
	int pos =  pControl->GetPos();

	switch(nControl) {

	case IDC_HSKIP_SLIDER:

			UpdateData(TRUE);

			m_nHSkip = pos & ~3;

			ApplyToCamera();

			UpdateData(FALSE);

		break;

	case IDC_VSKIP_SLIDER:

			UpdateData(TRUE);

			m_nVSkip = pos;

			ApplyToCamera();

			UpdateData(FALSE);

		break;
	case IDC_HACTV_SLIDER:

			UpdateData(TRUE);

			m_nHActv = pos;

			UpdateData(FALSE);

		break;
	case IDC_VACTV_SLIDER:

			UpdateData(TRUE);

			m_nVActv = pos;

			UpdateData(FALSE);

		break;
		
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRoiDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
//	UpdateData(TRUE);
	TRACE("HACTV %d\n", m_nHActv);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRoiDlg::UpdateFromCamera(PdvInput * pCamera)
{
	if (pCamera)
	{
		m_pCamera = pCamera;
	}

	if (!m_pCamera)
		return;

	m_pCamera->GetROI(m_nHSkip, m_nVSkip, 
		m_nHActv, m_nVActv);


	m_pCamera->GetCamSize(m_nWidth, m_nHeight);

	if (m_hWnd)
	{
		m_wndHSkipSlider.SetRange(0,m_nWidth-1);
		m_wndHActvSlider.SetRange(1,m_nWidth);
		m_wndVSkipSlider.SetRange(0,m_nHeight-1);
		m_wndVActvSlider.SetRange(1,m_nHeight);

		m_sHActvMax.Format("%d",m_nWidth);
		m_sHSkipMax.Format("%d",m_nWidth-1);

		m_sVActvMax.Format("%d",m_nHeight);
		m_sVSkipMax.Format("%d",m_nHeight - 1);

		m_wndHSkipSpin.SetRange(0,m_nWidth-1);
		m_wndHActvSpin.SetRange(1,m_nWidth);
		m_wndVSkipSpin.SetRange(0,m_nHeight-1);
		m_wndVActvSpin.SetRange(1,m_nHeight);
	

		UpdateData(FALSE);
	}
}

void CRoiDlg::ApplyToCamera()
{
	if (m_pCamera)
	{
		m_pCamera->SetROI(m_nHSkip, m_nVSkip, 
			m_nHActv, m_nVActv);

		if (m_pFire)
			m_pFire(m_pFireTarget);
	}

}

BOOL CRoiDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoiDlg::OnDeltaposHactvSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if ( m_nHActv - pNMUpDown->iDelta >= 1 && m_nHActv - pNMUpDown->iDelta <= m_nWidth - m_nHSkip)
	{
		m_nHActv -= pNMUpDown->iDelta;
		UpdateData(FALSE);
		ApplyToCamera();
		*pResult = 0;
	}		
	else
		*pResult = 1;
}

void CRoiDlg::OnDeltaposHskipSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if ( m_nHSkip - pNMUpDown->iDelta >= 0 && m_nHActv <= m_nWidth - m_nHSkip - pNMUpDown->iDelta)
	{
		m_nHSkip -= pNMUpDown->iDelta;
		UpdateData(FALSE);
		ApplyToCamera();
	*pResult = 0;
	}		
	else
		*pResult = 1;
}

void CRoiDlg::OnDeltaposVactvSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if ( m_nVActv - pNMUpDown->iDelta >= 1 && m_nVActv - pNMUpDown->iDelta <= m_nHeight - m_nVSkip)
	{
		m_nVActv -= pNMUpDown->iDelta;
		UpdateData(FALSE);
		ApplyToCamera();
	*pResult = 0;
	}		
	else
		*pResult = 1;
}

void CRoiDlg::OnDeltaposVskipSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	
	if ( m_nVSkip - pNMUpDown->iDelta >= 0 && m_nVActv <= m_nWidth - m_nVSkip - pNMUpDown->iDelta)
	{
		m_nVSkip -= pNMUpDown->iDelta;
		UpdateData(FALSE);
		ApplyToCamera();
	*pResult = 0;
	}		
	else
		*pResult = 1;
}
