// DlgPixelText.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"

#include "EdtImageTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdtImageTextDlg dialog


CEdtImageTextDlg::CEdtImageTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEdtImageTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdtImageTextDlg)
	m_nStartX = 0;
	m_nStartY = 0;
	m_bFollowCursor = TRUE;
    m_nFormat = 0;
	//}}AFX_DATA_INIT
}


void CEdtImageTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdtImageTextDlg)
	DDX_Control(pDX, IDC_PIXEL_VALUE_WINDOW, m_wndImageText);
	DDX_Text(pDX, IDC_STARTX, m_nStartX);
	DDX_Text(pDX, IDC_STARTY, m_nStartY);
	DDX_Check(pDX, IDC_FOLLOW_CURSOR, m_bFollowCursor);
    DDX_Radio(pDX, IDC_DECIMAL, m_nFormat);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		
	}
}


BEGIN_MESSAGE_MAP(CEdtImageTextDlg, CDialog)
	//{{AFX_MSG_MAP(CEdtImageTextDlg)
	ON_BN_CLICKED(IDC_FOLLOW_CURSOR, OnFollowCursor)
	ON_BN_CLICKED(IDC_GOTOXY, OnGotoxy)
    ON_BN_CLICKED(IDC_DECIMAL, OnDecimal)
    ON_BN_CLICKED(IDC_HEXIDECIMAL, OnHexidecimal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtImageTextDlg message handlers

BOOL CEdtImageTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEdtImageTextDlg::OnFollowCursor() 
{
	UpdateData(TRUE);	
}

void CEdtImageTextDlg::OnGotoxy() 
{
	UpdateData(TRUE);

	m_wndImageText.SetCenterPoint(CPoint(m_nStartX, m_nStartY));

	m_wndImageText.RefreshData();

}

void CEdtImageTextDlg::OnDecimal() 
{
	UpdateData(TRUE);

	m_wndImageText.SetDecimal(TRUE);
    m_nFormat = 0;

	m_wndImageText.RefreshData();
}

void CEdtImageTextDlg::OnHexidecimal() 
{
	UpdateData(TRUE);	

	m_wndImageText.SetDecimal(FALSE);
    m_nFormat = 1;

	m_wndImageText.RefreshData();
}

