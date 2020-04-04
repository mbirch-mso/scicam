// CCamControlDlg.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"


#include "CamControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Global references


/////////////////////////////////////////////////////////////////////////////
// CCamControlDlg dialog

CCamControlDlg::CCamControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamControlDlg::IDD, pParent)
{
	m_bInitialized = FALSE;
	m_bUpdating = FALSE;
	m_bUpdating1 = FALSE;

	//{{AFX_DATA_INIT(CCamControlDlg)
	m_nGain = 0;
	m_nLevel = 0;
	m_nShutter = 0;
	m_bHardwareInvert = FALSE;
	//}}AFX_DATA_INIT

	m_bShutterChanged = FALSE;
	m_bLevelChanged = FALSE;
	m_bGainChanged = FALSE;

	m_pCamera = NULL;

}

void CCamControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamControlDlg)
	DDX_Control(pDX, IDC_SPIN_SHUTTER, m_wndSpinShutter);
	DDX_Control(pDX, IDC_SPIN_LEVEL, m_wndSpinLevel);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_wndSpinGain);
	DDX_Control(pDX, IDC_GAIN_SCROLL, m_wndGainScroll);
	DDX_Control(pDX, IDC_LEVEL_SCROLL, m_wndLevelScroll);
	DDX_Control(pDX, IDC_SPEED_SCROLL, m_wndSpeedScroll);
	DDX_Control(pDX, IDC_EDIT_SHUTTER, m_ctlShutter);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_ctlGain);
	DDX_Control(pDX, IDC_EDIT_LEVEL, m_ctlLevel);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_nGain);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_nLevel);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_nShutter);
	DDX_Check(pDX, IDC_HARDWARE_INVERT, m_bHardwareInvert);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		m_wndGainScroll.SetPos(m_nGain);
		m_wndLevelScroll.SetPos(m_nLevel);
		m_wndSpeedScroll.SetPos(m_nShutter);
	}

}

BEGIN_MESSAGE_MAP(CCamControlDlg, CDialog)
	//{{AFX_MSG_MAP(CCamControlDlg)
	ON_BN_CLICKED(IDC_HARDWARE_INVERT, OnHardwareInvert)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	ON_EN_KILLFOCUS(IDC_EDIT_LEVEL, OnKillfocusEditLevel)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, OnKillfocusEditShutter)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamControlDlg message handlers

BOOL CCamControlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	if (!m_pCamera)
		return FALSE;

	m_pCamera->GetContinuousCapture(m_bContinuous);
	m_pCamera->GetGain(m_nGain);
	m_pCamera->GetLevel(m_nLevel);
	m_pCamera->GetShutterSpeed(m_nShutter);

	// This avoids an infinite loop while sync'ing the edit controls

	m_bInitialized = TRUE;

	int low = 0 , high = 100;

	m_pCamera->GetMinMaxShutter(low, high);

	if (m_nShutter < low)
		m_nShutter = low;
	else if (m_nShutter > high)
		m_nShutter = high;
	m_wndSpeedScroll.SetRange(low, high, FALSE);
	m_wndSpeedScroll.SetPos(m_nShutter);
	m_wndSpinShutter.SetRange(low,high);

	m_pCamera->GetMinMaxGain(low, high);
	if (m_nGain < low)
		m_nGain = low;
	else if (m_nGain > high)
		m_nGain = high;
	m_wndGainScroll.SetRange(low, high, FALSE);
	m_wndGainScroll.SetPos(m_nGain);
	m_wndSpinGain.SetRange(low,high);

	m_pCamera->GetMinMaxLevel(low, high);
	if (m_nLevel < low)
		m_nLevel = low;
	else if (m_nLevel > high)
		m_nLevel = high;
	m_wndLevelScroll.SetRange(low, high, FALSE);
	m_wndLevelScroll.SetPos(m_nLevel);

	m_wndSpinLevel.SetRange(low,high);

	UpdateData(FALSE);

	return FALSE;
}


void CCamControlDlg::OnOK() 
{
	UpdateData(TRUE);

#if 0

	if (!m_pCamera->SetShutterSpeed(m_nShutter))
	{	m_pCamera->GetShutterSpeed(m_nShutter);
		UpdateData(FALSE);
		m_ctlShutter.SetFocus();
		return;
	}

	if (!m_pCamera->SetGain(m_nGain))
	{	m_pCamera->GetGain(m_nGain);
		UpdateData(FALSE);
		m_ctlGain.SetFocus();
		return;
	}

	if (!m_pCamera->SetLevel(m_nLevel))
	{	m_pCamera->GetLevel(m_nLevel);
		UpdateData(FALSE);
		m_ctlLevel.SetFocus();
		return;
	}

#endif


//	m_pCamera->StoreCameraProfile();

	CDialog::OnOK();
}

void CCamControlDlg::OnCancel() 
{
//	m_pCamera->RestoreCameraProfile();

	CDialog::OnCancel();
}

void CCamControlDlg::OnHardwareInvert() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	
	m_pCamera->SetHardwareInvert(m_bHardwareInvert != 0);

}

void CCamControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
		int nControl = pScrollBar->GetDlgCtrlID();
	CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
	CString s;

	switch(nControl) {

		case IDC_GAIN_SCROLL:
			m_nGain= pControl->GetPos();
			UpdateData(FALSE);
			if (nSBCode == SB_ENDSCROLL) 
			{
				SetGain();
			}

		break;
		
		case IDC_LEVEL_SCROLL:
			m_nLevel = pControl->GetPos();
			UpdateData(FALSE);
			if (nSBCode == SB_ENDSCROLL) 
			{
				SetLevel();
			}
			
		break;
		case IDC_SPEED_SCROLL:
			m_nShutter = pControl->GetPos();
			UpdateData(FALSE);
			if (nSBCode == SB_ENDSCROLL) 
			{
				SetShutter();
			}
			
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCamControlDlg::OnKillfocusEditGain() 
{
	// TODO: Add your control notification handler code here
	int n = m_nGain;
	UpdateData(TRUE);
	
	if (m_nGain != n)
		SetGain();
}

bool CCamControlDlg::SetShutter()
{
	if (!m_pCamera->SetShutterSpeed(m_nShutter))
	{	
		m_pCamera->GetShutterSpeed(m_nShutter);
		UpdateData(FALSE);
		return FALSE;
	}
	return TRUE;
}

bool CCamControlDlg::SetGain()
{
	if (!m_pCamera->SetGain(m_nGain))
	{	
		m_pCamera->GetGain(m_nGain);
		UpdateData(FALSE);
		return FALSE;
	}
	return TRUE;
}

bool CCamControlDlg::SetLevel()
{
	if (!m_pCamera->SetLevel(m_nLevel))
	{	
		m_pCamera->GetLevel(m_nLevel);
		UpdateData(FALSE);
		return FALSE;
	}
	return TRUE;
}

void CCamControlDlg::OnKillfocusEditLevel() 
{
	// TODO: Add your control notification handler code here
	int n = m_nLevel;
	UpdateData(TRUE);
	if (m_nLevel != n)
		SetLevel();
}

void CCamControlDlg::OnKillfocusEditShutter() 
{
	// TODO: Add your control notification handler code here
	int n = m_nShutter;
	UpdateData(TRUE);
	
	if (m_nShutter != n)
		SetShutter();	
}

void CCamControlDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
		
	int nControl = pScrollBar->GetDlgCtrlID();
	CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
	CString s;

	switch(nControl) {

		case IDC_SPIN_GAIN:
			UpdateData(TRUE);
			SetGain();

		break;
		
		case IDC_SPIN_LEVEL:
			UpdateData(TRUE);
			SetLevel();
			
		break;
		case IDC_SPIN_SHUTTER:
			UpdateData(TRUE);
			SetShutter();
			
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
