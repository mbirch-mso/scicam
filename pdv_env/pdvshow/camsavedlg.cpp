// CCamSaveDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "edtcam.h"

#include "resource.h"
#include "CamSaveDlg.h"
#include "camera.h"

#include "dirname.h"

#include "direct.h"
#include "io.h"
#include "sys\stat.h"
#include "fcntl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Global references
extern CCamera g_theCamera;

/////////////////////////////////////////////////////////////////////////////
// CCamSaveDlg dialog

CCamSaveDlg::CCamSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamSaveDlg::IDD, pParent)
{
	m_bInitialized = FALSE;

	//{{AFX_DATA_INIT(CCamSaveDlg)
	m_strCaptureDirectory = _T("");
	m_strCaptureFile = _T("");
	m_nCaptureIndex = 0;
	m_bCaptureMode = 0;
	m_nCaptureCount = 0;
	m_nCaptureInterval = 0;
	//}}AFX_DATA_INIT
}

void CCamSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamSaveDlg)
	DDX_Control(pDX, IDC_CAPTURE_INTERVAL, m_ctlCaptureInterval);
	DDX_Control(pDX, IDC_CAPTURE_MODE, m_ctlCaptureMode);
	DDX_Control(pDX, IDC_CAPTURE_COUNT, m_ctlCaptureCount);
	DDX_Control(pDX, IDC_CAPTURE_INDEX, m_ctlCaptureIndex);
	DDX_Control(pDX, IDC_CAPTURE_FILE, m_ctlCaptureFile);
	DDX_Control(pDX, IDC_CAPTURE_DIRECTORY, m_ctlCaptureDirectory);
	DDX_Text(pDX, IDC_CAPTURE_DIRECTORY, m_strCaptureDirectory);
	DDX_Text(pDX, IDC_CAPTURE_FILE, m_strCaptureFile);
	DDX_Text(pDX, IDC_CAPTURE_INDEX, m_nCaptureIndex);
	DDX_Check(pDX, IDC_CAPTURE_MODE, m_bCaptureMode);
	DDX_Text(pDX, IDC_CAPTURE_COUNT, m_nCaptureCount);
	DDX_Text(pDX, IDC_CAPTURE_INTERVAL, m_nCaptureInterval);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCamSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CCamSaveDlg)
	ON_BN_CLICKED(IDC_BROWSE_DIRECTORIES, OnBrowseDirectories)
	ON_EN_CHANGE(IDC_CAPTURE_COUNT, OnChangeCaptureCount)
	ON_EN_CHANGE(IDC_CAPTURE_DIRECTORY, OnChangeCaptureDirectory)
	ON_EN_CHANGE(IDC_CAPTURE_FILE, OnChangeCaptureFile)
	ON_EN_CHANGE(IDC_CAPTURE_INDEX, OnChangeCaptureIndex)
	ON_BN_CLICKED(IDC_CAPTURE_MODE, OnCaptureMode)
	ON_EN_CHANGE(IDC_CAPTURE_INTERVAL, OnChangeCaptureInterval)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamSaveDlg helper functions

//
//	dirstatUNC:
//		Tests for the presence of a directory (possibly UNC-named)
//		by creating a writeable Hokeyfile in the directory and
//		and removing it.   
//		NOTE: This technique is used since _stat() and _access()
//		functions can't handle UNC-named files.
//
BOOL CCamSaveDlg::dirstatUNC(CString& strDirPath)
{
	// _open is used here to test for directory presence since _stat
	// and _access functions can't handle UNC-named files.
	char szPath[MAX_PATH + 1];	
	sprintf(szPath, "%s\\%s", strDirPath, "HokeyTestFile");
	int iFile = _open(szPath, _O_CREAT, _S_IWRITE);
   	if (iFile == -1)
	{	return FALSE;
	}
	else
	{   _close(iFile);
		remove(szPath);
		return TRUE;
	}
}

BOOL CCamSaveDlg::ValidateDirectory(BOOL bLogError, CString& strCaptureDirectory)
{
	if (strCaptureDirectory.IsEmpty())
	{	return TRUE;
	}

	// See if the capture directory already exists and is writeable
	if (!dirstatUNC(strCaptureDirectory))
	{	if (!bLogError)
		{	return FALSE;
		}
		TCHAR szBuf[256];
		sprintf(szBuf, "The file capture directory you have specified (%s) does not exist. Do you wish to create it?", strCaptureDirectory); 
		int nResult = AfxMessageBox(szBuf, MB_YESNO | MB_ICONQUESTION);
		if (nResult != IDYES)
		{	m_ctlCaptureDirectory.SetFocus();
			return FALSE;
		}
		// Create the capture directory
		if (_mkdir(m_strCaptureDirectory))
		{	TCHAR szBuf[256];
			sprintf(szBuf, "The file capture directory you have specified (%s) could not be created. Failed with Error Code: (%d).", strCaptureDirectory, errno);
			AfxMessageBox(szBuf, MB_OK | MB_ICONEXCLAMATION);
			m_ctlCaptureDirectory.SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCamSaveDlg message handlers

BOOL CCamSaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	g_theCamera.GetCaptureFileDirectory(m_strCaptureDirectory);
	g_theCamera.GetCaptureFileName(m_strCaptureFile);
	g_theCamera.GetCaptureFileIndex(m_nCaptureIndex);
	g_theCamera.GetCaptureFileCount(m_nCaptureCount);
	g_theCamera.GetCaptureFileInterval(m_nCaptureInterval);
	g_theCamera.GetAutoFileCapture(m_bCaptureMode);

	UpdateData(FALSE);

	m_ctlCaptureMode.SetFocus();
	m_bInitialized = TRUE;

	return FALSE;
}

void CCamSaveDlg::OnOK() 
{
	UpdateData(TRUE);

	g_theCamera.SetAutoFileCapture(m_bCaptureMode);

	if (!g_theCamera.SetCaptureFileDirectory(m_strCaptureDirectory))
	{	g_theCamera.GetCaptureFileDirectory(m_strCaptureDirectory);
		UpdateData(FALSE);
		m_ctlCaptureDirectory.SetFocus();
		return;
	}
	else
	{	if (m_bCaptureMode && m_strCaptureDirectory.IsEmpty())	
		{	AfxMessageBox("You must specify a valid file capture directory.", MB_OK | MB_ICONEXCLAMATION);
			m_ctlCaptureDirectory.SetFocus();
			return;
		}
		// See if the capture directory already exists and is writeable
		if (!ValidateDirectory(TRUE, m_strCaptureDirectory))
		{	return;
		}
	}

	if (!g_theCamera.SetCaptureFileName( m_strCaptureFile))
	{	g_theCamera.GetCaptureFileName(m_strCaptureFile);
		UpdateData(FALSE);
		m_ctlCaptureFile.SetFocus();
		return;
	}
	else
	{	if (m_bCaptureMode && m_strCaptureFile.IsEmpty())	
		{	AfxMessageBox("You must specify a valid base file name.", MB_OK | MB_ICONEXCLAMATION);
			m_ctlCaptureFile.SetFocus();
			return;
		}
	}
	
	if (!g_theCamera.SetCaptureFileIndex(m_nCaptureIndex))
	{	g_theCamera.GetCaptureFileIndex(m_nCaptureIndex);
		UpdateData(FALSE);
		m_ctlCaptureIndex.SetFocus();
		return;
	}

	if (!g_theCamera.SetCaptureFileCount(m_nCaptureCount))
	{	g_theCamera.GetCaptureFileCount(m_nCaptureCount);
		UpdateData(FALSE);
		m_ctlCaptureCount.SetFocus();
		return;
	}

	if (!g_theCamera.SetCaptureFileInterval(m_nCaptureInterval))
	{	g_theCamera.GetCaptureFileInterval(m_nCaptureInterval);
		UpdateData(FALSE);
		m_ctlCaptureInterval.SetFocus();
		return;
	}

	g_theCamera.StoreCaptureProfile();

	CDialog::OnOK();
}

void CCamSaveDlg::OnCancel() 
{
	g_theCamera.RestoreCaptureProfile();

	CDialog::OnCancel();
}

void CCamSaveDlg::OnBrowseDirectories() 
{
	CFileDialog dlgFile(FALSE);

	UpdateData(TRUE);

	// See if the capture directory already exists and is writeable
	CString strCaptureDirectory = m_strCaptureDirectory;
	if (!ValidateDirectory(FALSE, strCaptureDirectory))
	{	g_theCamera.GetCaptureFileDirectory(strCaptureDirectory);
	}
	TCHAR szCaptureDirectory[_MAX_PATH];
	strcpy(szCaptureDirectory, strCaptureDirectory);

	TCHAR szFilePath[_MAX_PATH];
	strcpy(szFilePath, m_strCaptureFile);

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	dlgFile.m_ofn.lpstrInitialDir = szCaptureDirectory;
	dlgFile.m_ofn.nMaxCustFilter = 0;
	dlgFile.m_ofn.lpstrDefExt = _T("");
	dlgFile.m_ofn.nFilterIndex = 0;
	dlgFile.m_ofn.lpstrFilter = _T("");
	dlgFile.m_ofn.lpstrTitle = _T("Select Directory and Base Name");
	dlgFile.m_ofn.lpstrFile = szFilePath;
	dlgFile.m_ofn.nMaxFile = _MAX_PATH;
	dlgFile.m_ofn.lpstrFileTitle = NULL;
	dlgFile.m_ofn.nMaxFileTitle = 0;
	BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
	if (bResult != IDOK)
	{	return;
	}

	TCHAR szDirectory[_MAX_PATH];
	GetDirPath(szFilePath, szDirectory, _MAX_PATH);
	m_strCaptureDirectory = szDirectory;
	if (!g_theCamera.SetCaptureFileDirectory( m_strCaptureDirectory))
	{	g_theCamera.GetCaptureFileDirectory(m_strCaptureDirectory);
		UpdateData(FALSE);
		m_ctlCaptureDirectory.SetFocus();
		return;
	}
	else
	{	if (m_bCaptureMode && m_strCaptureDirectory.IsEmpty())	
		{	AfxMessageBox("You must specify a valid file capture directory.", MB_OK | MB_ICONEXCLAMATION);
			m_ctlCaptureDirectory.SetFocus();
			return;
		}
		// See if the capture directory already exists and is writeable
		if (!ValidateDirectory(TRUE, m_strCaptureDirectory))
		{	return;
		}
	}
	TCHAR szFileName[_MAX_PATH];
	GetFileName(szFilePath, szFileName, _MAX_PATH);
	m_strCaptureFile = szFileName;
	if (!g_theCamera.SetCaptureFileName(m_strCaptureFile))
	{	g_theCamera.GetCaptureFileName(m_strCaptureFile);
		UpdateData(FALSE);
		m_ctlCaptureFile.SetFocus();
		return;
	}
	else
	{	if (m_bCaptureMode && m_strCaptureFile.IsEmpty())	
		{	AfxMessageBox("You must specify a valid base file name.", MB_OK | MB_ICONEXCLAMATION);
			m_ctlCaptureFile.SetFocus();
			return;
		}
	}

	UpdateData(FALSE);
}

void CCamSaveDlg::OnChangeCaptureCount() 
{
	if (!m_bInitialized)
	{	return;
	}
	
	UpdateData(TRUE);	
	if (!g_theCamera.SetCaptureFileCount( m_nCaptureCount))
	{	g_theCamera.GetCaptureFileCount(m_nCaptureCount);
		UpdateData(FALSE);
		m_ctlCaptureCount.SetFocus();
		return;
	}

	g_theCamera.GetCaptureFileCount(m_nCaptureCount);

	UpdateData(FALSE);
}

void CCamSaveDlg::OnChangeCaptureDirectory() 
{
	if (!m_bInitialized)
	{	return;
	}
	
	UpdateData(TRUE);
	if (!g_theCamera.SetCaptureFileDirectory( m_strCaptureDirectory))
	{	g_theCamera.GetCaptureFileDirectory(m_strCaptureDirectory);
		UpdateData(FALSE);
		m_ctlCaptureDirectory.SetFocus();
		return;
	}
	g_theCamera.GetCaptureFileDirectory(m_strCaptureDirectory);

	UpdateData(FALSE);
}

void CCamSaveDlg::OnChangeCaptureFile() 
{
	if (!m_bInitialized)
	{	return;
	}
	
	UpdateData(TRUE);
	if (!g_theCamera.SetCaptureFileName( m_strCaptureFile))
	{	g_theCamera.GetCaptureFileName(m_strCaptureFile);
		UpdateData(FALSE);
		m_ctlCaptureFile.SetFocus();
		return;
	}
	g_theCamera.GetCaptureFileName(m_strCaptureFile);

	UpdateData(FALSE);
}

void CCamSaveDlg::OnChangeCaptureIndex() 
{
	if (!m_bInitialized)
	{	return;
	}
	
	UpdateData(TRUE);	
	if (!g_theCamera.SetCaptureFileIndex(m_nCaptureIndex))
	{	g_theCamera.GetCaptureFileIndex(m_nCaptureIndex);
		UpdateData(FALSE);
		m_ctlCaptureIndex.SetFocus();
		return;
	}

	g_theCamera.GetCaptureFileIndex(m_nCaptureIndex);

	UpdateData(FALSE);	
}

void CCamSaveDlg::OnChangeCaptureInterval() 
{
	if (!m_bInitialized)
	{	return;
	}
	
	UpdateData(TRUE);	
	if (!g_theCamera.SetCaptureFileInterval(m_nCaptureInterval))
	{	g_theCamera.GetCaptureFileInterval(m_nCaptureInterval);
		UpdateData(FALSE);
		m_ctlCaptureInterval.SetFocus();
		return;
	}

	g_theCamera.GetCaptureFileInterval(m_nCaptureInterval);

	UpdateData(FALSE);
}

void CCamSaveDlg::OnCaptureMode() 
{
	UpdateData(TRUE);

	g_theCamera.SetAutoFileCapture(m_bCaptureMode);
	
}
	



