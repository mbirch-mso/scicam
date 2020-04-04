// DisplaySkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DisplaySkipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplaySkipDlg dialog


CDisplaySkipDlg::CDisplaySkipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplaySkipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplaySkipDlg)
	m_nDisplaySkip = 1;
	//}}AFX_DATA_INIT

	m_pNotifyTarget = NULL;
	m_pNotifier = NULL;

}


void CDisplaySkipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplaySkipDlg)
	DDX_Control(pDX, IDC_DISPLAY_SKIP_SLIDER, m_wndDisplaySkipSlider);
	DDX_Control(pDX, IDC_DISPLAY_SKIP_SPIN, m_wndDisplaySkipSpin);
	DDX_Text(pDX, IDC_DISPLAY_SKIP_VALUE, m_nDisplaySkip);
	DDV_MinMaxInt(pDX, m_nDisplaySkip, 1, 10000);
	//}}AFX_DATA_MAP

	m_wndDisplaySkipSlider.SetPos(m_nDisplaySkip);

}


BEGIN_MESSAGE_MAP(CDisplaySkipDlg, CDialog)
	//{{AFX_MSG_MAP(CDisplaySkipDlg)
	ON_EN_UPDATE(IDC_DISPLAY_SKIP_VALUE, OnUpdateDisplaySkipValue)
	ON_EN_KILLFOCUS(IDC_DISPLAY_SKIP_VALUE, OnKillfocusDisplaySkipValue)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_DISPLAY_SKIP_SPIN, OnDeltaposDisplaySkipSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplaySkipDlg message handlers

void CDisplaySkipDlg::OnUpdateDisplaySkipValue() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	if (m_wndDisplaySkipSlider.m_hWnd)
		UpdateData(TRUE);
	UpdateOwner();
}

void CDisplaySkipDlg::OnKillfocusDisplaySkipValue() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateOwner();
}

void CDisplaySkipDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nControl = pScrollBar->GetDlgCtrlID();
	CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
	CString s;

	switch(nControl) {

		case IDC_DISPLAY_SKIP_SLIDER:

			m_nDisplaySkip = pControl->GetPos();

			UpdateData(FALSE);

			if (nSBCode == SB_ENDSCROLL) 
			{
				UpdateOwner();
			}

		break;
		
		
	}
		// TODO: Add your message handler code here and/or call default
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDisplaySkipDlg::UpdateOwner()
{

	if (m_pNotifyTarget && m_pNotifier)
		m_pNotifier(m_pNotifyTarget, m_nDisplaySkip);

}

void CDisplaySkipDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	UpdateOwner();

	CDialog::OnOK();
}

BOOL CDisplaySkipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_wndDisplaySkipSlider.SetRange(1,100);
	m_wndDisplaySkipSpin.SetRange(1,100);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDisplaySkipDlg::OnDeltaposDisplaySkipSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if ( m_nDisplaySkip - pNMUpDown->iDelta >= 0 && m_nDisplaySkip <= 100)
	{
		m_nDisplaySkip -= pNMUpDown->iDelta;
		UpdateData(FALSE);
		UpdateOwner();
		*pResult = 0;
	}		
	else
		*pResult = 1;

}
