// dlg_set.cpp : implementation file
//

#include "stdafx.h"
//#include "edtcam.h"

#include "resource.h"
#include "CamSetupDlg.h"

#include "camera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamSetupDlg dialog


CCamSetupDlg::CCamSetupDlg(CWnd* pParent /*=NULL*/, LPCSTR strCurrent, CPtrArray *ptrs)
	: CDialog(CCamSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamSetupDlg)
	m_strCamType = _T("");
	//}}AFX_DATA_INIT

	if (strCurrent)
		m_strCamType = strCurrent;

	m_pCamList = ptrs;

}


void CCamSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamSetupDlg)
	DDX_Control(pDX, IDC_CAMLIST, m_ctlCamList);
	DDX_Text(pDX, IDC_CAMTYPE, m_strCamType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CCamSetupDlg)
	ON_LBN_KILLFOCUS(IDC_CAMLIST, OnKillfocusCamlist)
	ON_LBN_SELCHANGE(IDC_CAMLIST, OnSelchangeCamlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamSetupDlg message handlers

BOOL CCamSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Add list of supported camera types
	//
	CPtrArray* pCameraList = m_pCamList;
	if (pCameraList)
	{   for (int idx = 0; idx < pCameraList->GetSize(); idx++)
		{	CAMERA_RECORD* pCamRec = (CAMERA_RECORD*) pCameraList->GetAt(idx);	 
			if (pCamRec)
			{	int nIndex = (int) SendDlgItemMessage(IDC_CAMLIST, LB_ADDSTRING, 0, (LPARAM) (const char*) pCamRec->szCamType);
				if (nIndex != LB_ERR && nIndex != LB_ERRSPACE)
				{	SendDlgItemMessage(IDC_CAMLIST, LB_SETITEMDATA, nIndex, (LPARAM) idx);
				}
			}
		}
	}

	int nIndex = -1;

	if (!m_strCamType.IsEmpty())
	{	nIndex = (int) SendDlgItemMessage(IDC_CAMLIST, LB_FINDSTRING, 0, (LPARAM) (const char*) m_strCamType);
 		if (nIndex == LB_ERR)
 		{	nIndex = -1;
			m_strCamType = _T("");
		}
	}
//	if (nIndex >= 0)
	{	SendDlgItemMessage(IDC_CAMLIST, LB_SETCURSEL, nIndex, 0);
	}

	UpdateData(FALSE);

	CenterWindow();

	return TRUE;
}

void CCamSetupDlg::OnKillfocusCamlist() 
{
	OnSelchangeCamlist();
}

void CCamSetupDlg::OnSelchangeCamlist() 
{
	m_strCamType = _T("");

	int nIndex = (int) SendDlgItemMessage(IDC_CAMLIST, LB_GETCURSEL, 0, 0);
 	if (nIndex == CB_ERR)
 	{	UpdateData(FALSE);	
		return;
	}
	nIndex = (int) SendDlgItemMessage(IDC_CAMLIST, LB_GETITEMDATA, nIndex, 0);
 	if (nIndex == CB_ERR)
 	{	UpdateData(FALSE);	
		return;
	}
	
	CPtrArray* pCamList = m_pCamList;
	if (pCamList)
	{	
		CAMERA_RECORD* pCamRec = (CAMERA_RECORD*) pCamList->GetAt(nIndex);
		if (pCamRec)
		{	
			m_strCamType = pCamRec->szCamType;
		}
	}

	UpdateData(FALSE);
	
	return;	
}

void CCamSetupDlg::OnOK() 
{
	int nIndex = (int) SendDlgItemMessage(IDC_CAMLIST, LB_GETCURSEL, 0, 0);
 	if (nIndex == CB_ERR)
 	{	AfxMessageBox("Please select a camera.", MB_OK|MB_ICONSTOP,0);
		return;
	}
	nIndex = (int) SendDlgItemMessage(IDC_CAMLIST, LB_GETITEMDATA, nIndex, 0);
 	if (nIndex == CB_ERR)
 	{	AfxMessageBox("Please select a camera.", MB_OK|MB_ICONSTOP,0);
		return;
	}
	
	CPtrArray* pCamList = m_pCamList;
	if (pCamList)
	{	CAMERA_RECORD* pCamRec = (CAMERA_RECORD*) pCamList->GetAt(nIndex);
		if (pCamRec)
		{	
			m_strCamType = pCamRec->szCamType;
		}
	}	

	CDialog::OnOK();
}
