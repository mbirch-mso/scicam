// DVCControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "edtdefines.h"
#include "resource.h"

#include "DVCControlDlg.h"

#include "ConfigDataSet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDVCControlDlg dialog


CDVCControlDlg::CDVCControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDVCControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDVCControlDlg)
	m_sGainValue = _T("");
	m_sOffsetValue = _T("");
	m_sExposureValue = _T("");
	m_bConnect = FALSE;
	m_sBinning = _T("");
	m_sCamera = _T("");
	m_sMode = _T("");
	m_bRoiEnabled = FALSE;
	m_nRoiBottom = 0;
	m_nRoiTop = 0;
	m_nSpeed = 0;
	//}}AFX_DATA_INIT

	m_pCamera = NULL;

	m_nGain = 0;
	m_nOffset = 31;
	m_nExposure = 1;

	m_pRGBDlg = NULL;

	m_pFire = NULL;
	m_pLive = NULL;
	m_pGrab = NULL;
	m_pIsLive = NULL;

	m_bGoing = FALSE;

	m_pFireTarget = NULL;
}

#define EXPOSE_UNIT (81.8 / 256);

void CDVCControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDVCControlDlg)
	DDX_Control(pDX, IDC_EXPOSURE_SLIDER, m_wndExposureSlider);
	DDX_Control(pDX, IDC_OFFSET_SLIDER, m_wndOffsetSlider);
	DDX_Control(pDX, IDC_GAIN_SLIDER, m_wndGainSlider);
	DDX_Control(pDX, IDC_OFFSET_SPIN, m_wndOffsetSpin);
	DDX_Control(pDX, IDC_GAIN_SPIN, m_wndGainSpin);
	DDX_Control(pDX, IDC_EXPOSE_SPIN, m_wndExposureSpin);
	DDX_Text(pDX, IDC_GAIN_VALUE, m_sGainValue);
	DDX_Text(pDX, IDC_OFFSET_VALUE, m_sOffsetValue);
	DDX_Text(pDX, IDC_EXPOSURE_VALUE, m_sExposureValue);
	DDX_Control(pDX, IDC_MODE_COMBO, m_wndModeCombo);
	DDX_Control(pDX, IDC_BINNING_COMBO, m_wndBinningCombo);
	DDX_CBString(pDX, IDC_BINNING_COMBO, m_sBinning);
	DDX_CBString(pDX, IDC_MODE_COMBO, m_sMode);
	DDX_Check(pDX, IDC_ENABLE_ROI, m_bRoiEnabled);
	DDX_Text(pDX, IDC_ROI_BOTTOM_EDIT, m_nRoiBottom);
	DDX_Text(pDX, IDC_ROI_TOP_EDIT, m_nRoiTop);
	DDX_Radio(pDX, IDC_SPEED1, m_nSpeed);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)	
	{
		m_wndExposureSlider.SetPos(m_nExposure);
		m_wndGainSlider.SetPos(m_nGain);
		m_wndOffsetSlider.SetPos(m_nOffset);
	}
}


BEGIN_MESSAGE_MAP(CDVCControlDlg, CDialog)
	//{{AFX_MSG_MAP(CDVCControlDlg)
	ON_BN_CLICKED(IDC_SETUP_BUTTON, OnSetupButton)
	ON_BN_CLICKED(IDC_WB_BUTTON, OnWbButton)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_OFFSET_SPIN, OnDeltaposOffsetSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_GAIN_SPIN, OnDeltaposGainSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_EXPOSE_SPIN, OnDeltaposExposeSpin)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_BINNING_COMBO, OnSelchangeBinningCombo)
	ON_CBN_SELCHANGE(IDC_CAMERA_COMBO, OnSelchangeCameraCombo)
	ON_CBN_SELCHANGE(IDC_MODE_COMBO, OnSelchangeModeCombo)
	ON_BN_CLICKED(IDC_OPEN_CAMERA, OnOpenCamera)
	ON_BN_CLICKED(IDC_APPLY_BINNING, OnApplyBinning)
	ON_BN_CLICKED(IDC_APPLY_CAMERA_MODE, OnApplyCameraMode)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnRefreshButton)
	ON_BN_CLICKED(IDC_ENABLE_ROI, OnEnableRoi)
	ON_BN_CLICKED(IDC_SPEED1, OnSpeed1)
	ON_BN_CLICKED(IDC_SPEED2, OnSpeed2)
	ON_BN_CLICKED(IDC_SPEED4, OnSpeed4)
	ON_BN_CLICKED(IDC_SPEED8, OnSpeed8)
	ON_BN_CLICKED(IDC_GRAB_BUTTON, OnGrabButton)
	ON_BN_CLICKED(IDC_LIVE_BUTTON, OnLiveButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVCControlDlg message handlers

void CDVCControlDlg::OnSetupButton() 
{
	// TODO: Add your control notification handler code here
//	m_SetupDlg.SetCamera(m_pCamera);

//	m_SetupDlg.DoModal();

}

void CDVCControlDlg::OnWbButton() 
{
	// TODO: Add your control notification handler code here
	// spawn a Woft

	if (m_pRGBDlg)
	
	{

		m_pRGBDlg->SetCamera(m_pCamera);

		if (!m_pRGBDlg->m_hWnd)
		{
			m_pRGBDlg->Create(IDD_SOFTWARE_RGB,this);

			m_pRGBDlg->ShowWindow(SW_SHOW);
		}
		else
		{

			m_pRGBDlg->ShowWindow(SW_RESTORE);

		}
	}

}

void CDVCControlDlg::SetCamera(CPdvInput *pCamera)
{
	m_pCamera = pCamera;

	if (m_pCamera)
	{
		UpdateFromCamera();
	}

}

void CDVCControlDlg::SetGain()
{
	if (m_hWnd)
	{
		UpdateData(TRUE);

		if (m_pCamera)
		{
			m_pCamera->SetGain(m_nGain);
		}

	}

}

void CDVCControlDlg::SetExposure()
{
	if (m_hWnd)
	{
		UpdateData(TRUE);

		if (m_pCamera)
		{
			m_pCamera->SetShutterSpeed(m_nExposure);
		}

	}


}

void CDVCControlDlg::SetOffset()
{
	if (m_hWnd)
	{
		UpdateData(TRUE);

		if (m_pCamera)
		{
			m_pCamera->SetLevel(m_nOffset);
		}

	}

}

void CDVCControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nControl = pScrollBar->GetDlgCtrlID();
	
	CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
	CString s;


	switch(nControl) {

		case IDC_EXPOSURE_SLIDER:

			UpdateData(TRUE);
			
			m_nExposure = pControl->GetPos();
			m_sExposureValue.Format("%4.1f", ScreenExposure(m_nExposure));

			UpdateData(FALSE);

			if (nSBCode == SB_ENDSCROLL) 
			{
				SetExposure();
			}

		break;
		
		case IDC_GAIN_SLIDER:

			UpdateData(TRUE);
			m_nGain = pControl->GetPos();
			m_sGainValue.Format("%4.1f", ScreenGain(m_nGain));

			UpdateData(FALSE);
			if (nSBCode == SB_ENDSCROLL) 
			{
				SetGain();
			}
			
		break;
		case IDC_OFFSET_SLIDER:

			UpdateData(TRUE);
			m_nOffset = pControl->GetPos();
			m_sOffsetValue.Format("%4.0f", ScreenOffset(m_nOffset));

			UpdateData(FALSE);
			if (nSBCode == SB_ENDSCROLL) 
			{
				SetOffset();
			}
			
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDVCControlDlg::UpdateFromCamera()

{
	if (m_pCamera)
	{
		// Use STA command to get current values

		m_pCamera->UpdateValuesFromCamera();
	
		m_pCamera->GetGain(m_nGain);
		m_pCamera->GetLevel(m_nOffset);
		m_pCamera->GetShutterSpeed(m_nExposure);

		char mode[4];

		mode[0] = 0;

		m_pCamera->DVCGetMode(mode);
			
		m_sMode = mode;

		m_sCamera = m_pCamera->GetCameraType();

		UpdateBinValue();

	}
	else
	{
		m_nGain = 0;
		m_nOffset = 31;
		m_nExposure = 1;

	}

	m_sGainValue.Format("%4.1f", ScreenGain(m_nGain));
	m_sOffsetValue.Format("%4.1f", ScreenOffset(m_nOffset));
	m_sExposureValue.Format("%4.1f", ScreenExposure(m_nExposure));

	if (m_hWnd)
		UpdateData(FALSE);

}

BOOL CDVCControlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (m_wndExposureSlider.m_hWnd)
	{
		m_wndExposureSlider.SetRange(1,0xff,TRUE);
	}

	if (m_wndGainSlider.m_hWnd)
	{
		m_wndGainSlider.SetRange(1,0x9A,TRUE);
	}

	if (m_wndOffsetSlider.m_hWnd)
	{
		m_wndOffsetSlider.SetRange(1,0x9A,TRUE);
	}
	BuildCameraList();

	for (int i = 0; i< m_CameraList.GetSize();i++)
	{

		CCameraRecord *pRec = (CCameraRecord *) m_CameraList.GetAt(i);

		m_wndCameraCombo.AddString(pRec->szCamType);

	}

	//UpdateFromCamera();
	UpdateBinValue();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

double CDVCControlDlg::ScreenGain(const int nGain)

{
	return (double) nGain * (30.8 / 154.0) - 8.2;

}

double CDVCControlDlg::ScreenOffset(const int nOffset)

{
	return (double) nOffset * (47 / 154.0) - 15;
}

double CDVCControlDlg::ScreenExposure(const int nExposure)

{
	return  (double) nExposure * 0.32;
}

int CDVCControlDlg::CameraGain(const double dGain)

{
	return (int) ((dGain+8.2) * (154.0/30.8));
}

int CDVCControlDlg::CameraOffset(const double dOffset)

{
	return (int) ((dOffset+15) * (154.0/47));
}

int CDVCControlDlg::CameraExposure(const double dExposure)

{
	return (int) ((dExposure / 0.32));
}



void CDVCControlDlg::OnDeltaposOffsetSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_nOffset - pNMUpDown->iDelta  >= 0 && m_nOffset - pNMUpDown->iDelta <= 0x9A)
	{
		m_nOffset -= pNMUpDown->iDelta;
		m_sOffsetValue.Format("%4.1f", ScreenOffset(m_nOffset));
		UpdateData(FALSE);
		SetOffset();
	}
	
	*pResult = 1;

}

void CDVCControlDlg::OnDeltaposGainSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_nGain - pNMUpDown->iDelta >= 0 && m_nGain - pNMUpDown->iDelta <= 0x9A)
	{
		m_nGain -= pNMUpDown->iDelta;
		m_sGainValue.Format("%4.1f", ScreenGain(m_nGain));
		UpdateData(FALSE);
		SetGain();
	}	
	*pResult = 0;
}

void CDVCControlDlg::OnDeltaposExposeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if ( m_nExposure - pNMUpDown->iDelta >= 0 && m_nExposure - pNMUpDown->iDelta <= 0xff)
	{
		m_nExposure -= pNMUpDown->iDelta;
		m_sExposureValue.Format("%4.1f", ScreenExposure(m_nExposure));
		UpdateData(FALSE);
		SetExposure();
	}	
	*pResult = 0;
}

void CDVCControlDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDVCControlDlg::ClearCameraList()

{
	CCameraRecord* pCamRec = NULL;

	for (int idx = 0; idx < m_CameraList.GetSize(); idx++)
	{
		pCamRec = (CCameraRecord*) m_CameraList.GetAt(idx);
		if (pCamRec)
		{
			delete pCamRec;
		}
	}

	m_CameraList.RemoveAll();
}

void CDVCControlDlg::BuildCameraList()
{

	CString ConfigPath = m_pCamera->GetCamConfigPath();

	// Scan all the files in the installed camaras directory and build a list
	HANDLE hFile;
	char szWildCard[_MAX_PATH];
	sprintf(szWildCard, "%s\\*.cfg", ConfigPath);

    WIN32_FIND_DATA FindFileData;
	hFile = FindFirstFile(szWildCard, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{	
		return;
	}
	
	ClearCameraList();

	BOOL bMoreToRead = TRUE;
	while (bMoreToRead)
	{

		CString strCamType;
		CString strCamClass;
		
		char szCamFileName[_MAX_PATH];
		sprintf(szCamFileName, "%s\\%s", ConfigPath, FindFileData.cFileName);
		
		
		ConfigDataSet cfg;

		cfgset_init(&cfg);
		cfgset_load(&cfg, szCamFileName);

		char *test;

		if (test = cfgset_lookup(&cfg, "camera_class"))
		{
			strCamClass = test ;


			if (strCamClass == "\"DVC\"")
			{
					//TRACE("File %s Type %s Clases %s\n", 
				//	szCamFileName, strCamType, strCamClass);
				

				if (test = cfgset_lookup(&cfg, "cameratype"))
				{

					strCamType = test;

					if (strCamType.GetLength())
					{
						CCameraRecord *pCamRec = NULL;
						pCamRec = new CCameraRecord;

						strcpy(pCamRec->szCamFile, szCamFileName);
						strcpy(pCamRec->szCamType, strCamType);
						strcpy(pCamRec->szCamClass, strCamClass);
				
						m_CameraList.Add(pCamRec);
					}

				}
			}
		}

		cfgset_destroy(&cfg);

		bMoreToRead = FindNextFile(hFile, &FindFileData);
	}

	FindClose(hFile);
}

void CDVCControlDlg::OnOpenCamera() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CCameraRecord *pRec;

	int index = m_wndCameraCombo.GetCurSel();

	if (index >= 0)
	{
		pRec = (CCameraRecord *) m_CameraList.GetAt(index);

		m_pCamera->Setup(m_pCamera->GetCamConfigPath(), pRec->szCamFile);

		if (m_pFire)
		{
			m_pFire(m_pFireTarget);
		}
	}

}

void CDVCControlDlg::OnApplyBinning() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	// Set the binning factors

	if (m_pCamera)
	{
		int xval = 1;
		int yval = 1;

		int hBin = m_wndBinningCombo.GetCurSel();

		switch (hBin) {

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


		if (m_pFire)
		{
			m_pFire(m_pFireTarget);
		}

		UpdateData(FALSE);
	}
}

void CDVCControlDlg::OnApplyCameraMode() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);


	if (m_pCamera)
	{

		char modestr[80];
		int which = m_wndModeCombo.GetCurSel();

		if (which  > 0)
		{
			m_wndModeCombo.GetLBText(which , modestr);

			m_pCamera->DVCSetMode(modestr);


			if (m_pFire)
			{
				m_pFire(m_pFireTarget);
			}
		}	
		
		// Adjust exposure slider

	}
}

void CDVCControlDlg::OnSelchangeBinningCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	

}

void CDVCControlDlg::OnSelchangeCameraCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlDlg::OnSelchangeModeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
}

void CDVCControlDlg::UpdateBinValue()
{
	if (!m_pCamera)
		return;

	int xval = m_pCamera->GetBinX();
	int yval = m_pCamera->GetBinY();

	int m_nHbin = 0;

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
		
		m_wndBinningCombo.SetCurSel(m_nHbin);

	}
}

void CDVCControlDlg::OnRefreshButton() 
{
	// TODO: Add your control notification handler code here
	UpdateFromCamera();
}

void CDVCControlDlg::OnEnableRoi() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	StopIfGoing();

	if (m_bRoiEnabled)
	{

		// Send enable roi command to camera

		// send size commands to camera

		// 
	}

	StartIfGoing();

}

void CDVCControlDlg::OnSpeed1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetSpeed();		
}

void CDVCControlDlg::OnSpeed2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetSpeed();		
}

void CDVCControlDlg::OnSpeed4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetSpeed();		
}

void CDVCControlDlg::OnSpeed8() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetSpeed();	
}

void CDVCControlDlg::SetSpeed()
{
	if (!m_pCamera)
		return;
	
	StopIfGoing();

	char cmdstring[12];
	int speedval = 1;

	// DVC slow is power of 2
	for (int i=0;i<m_nSpeed;i++)
		speedval *= 2;

	sprintf(cmdstring,"SLW %02d\n", speedval);

	m_pCamera->SerialCommand(cmdstring);


	StartIfGoing();
}

void CDVCControlDlg::OnGrabButton() 
{
	// TODO: Add your control notification handler code here
	if (m_pGrab && m_pFireTarget)
		m_pGrab(m_pFireTarget);
}

void CDVCControlDlg::OnLiveButton() 
{
	// TODO: Add your control notification handler code here
	if (m_pLive && m_pFireTarget)
		m_pLive(m_pFireTarget);
}


void CDVCControlDlg::StopIfGoing()
{
	if (m_pFireTarget && m_pIsLive && m_pLive)
	{
		m_bGoing = m_pIsLive(m_pFireTarget);

		if (m_bGoing)
		{
			m_pLive(m_pFireTarget);
			
			int timeout = 0;

			while (m_pIsLive(m_pFireTarget) && timeout < 10)
			{
				Sleep(100);
				timeout++;
			}

			if (timeout >= 10)
				m_bGoing = FALSE;
		}

	}
	else
		m_bGoing = FALSE;

}
void CDVCControlDlg::StartIfGoing()
{
	if (m_pFireTarget && m_pIsLive && m_pLive)
	{
		if (m_bGoing)
			m_pLive(m_pFireTarget);

	}

	m_bGoing = FALSE;
}
