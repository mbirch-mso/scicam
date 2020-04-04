// DVCSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DVCSetupDlg.h"

#include "ConfigDataSet.h"

#include "DVCControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDVCSetupDlg dialog


CDVCSetupDlg::CDVCSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDVCSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDVCSetupDlg)
	m_sBinning = _T("");
	m_sCamera = _T("");
	m_sMode = _T("");
	m_nHeight = _T("");
	m_nWidth = _T("");
	//}}AFX_DATA_INIT
}


void CDVCSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDVCSetupDlg)
	DDX_Control(pDX, IDC_MODE_COMBO, m_wndModeCombo);
	DDX_Control(pDX, IDC_CAMERA_COMBO, m_wndCameraCombo);
	DDX_Control(pDX, IDC_BINNING_COMBO, m_wndBinningCombo);
	DDX_CBString(pDX, IDC_BINNING_COMBO, m_sBinning);
	DDX_CBString(pDX, IDC_CAMERA_COMBO, m_sCamera);
	DDX_CBString(pDX, IDC_MODE_COMBO, m_sMode);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDVCSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CDVCSetupDlg)
	ON_CBN_SELCHANGE(IDC_BINNING_COMBO, OnSelchangeBinningCombo)
	ON_CBN_SELCHANGE(IDC_CAMERA_COMBO, OnSelchangeCameraCombo)
	ON_CBN_SELCHANGE(IDC_MODE_COMBO, OnSelchangeModeCombo)
	ON_BN_CLICKED(IDC_OPEN_CAMERA, OnOpenCamera)
	ON_BN_CLICKED(IDC_APPLY_BINNING, OnApplyBinning)
	ON_BN_CLICKED(IDC_APPLY_CAMERA_MODE, OnApplyCameraMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVCSetupDlg message handlers

void CDVCSetupDlg::OnSelchangeBinningCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCSetupDlg::OnSelchangeCameraCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCSetupDlg::OnSelchangeModeCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCSetupDlg::ClearCameraList()

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

void CDVCSetupDlg::BuildCameraList()
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


			if (strCamClass == "DVC")
			{
					//TRACE("File %s Type %s Clases %s\n", 
				//	szCamFileName, strCamType, strCamClass);
				

				if (test = cfgset_lookup(&cfg, "camera_type"))
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

void CDVCSetupDlg::OnOpenCamera() 
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

void CDVCSetupDlg::OnApplyBinning() 
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

		m_nWidth = m_pCamera->GetWidth();
		m_nHeight = m_pCamera->GetHeight();


		if (m_pFire)
		{
			m_pFire(m_pFireTarget);
		}

		UpdateData(FALSE);
	}
}

void CDVCSetupDlg::OnApplyCameraMode() 
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

			if (m_ControlDlg)
			{
				m_ControlDlg->UpdateFromCamera();
			}
		}
			
	}
}

void CDVCSetupDlg::UpdateFromCamera(CPdvInput * pCamera)
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

		UpdateData(FALSE);

	}

}

BOOL CDVCSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (m_pCamera)
	{

		BuildCameraList();

		for (int i = 0; i< m_CameraList.GetSize();i++)
		{

			CCameraRecord *pRec = (CCameraRecord *) m_CameraList.GetAt(i);

			m_wndCameraCombo.AddString(pRec->szCamType);

		}

		UpdateFromCamera();

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
