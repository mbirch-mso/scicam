// BGSubtractDlg.cpp : implementation file
//

#include "stdafx.h"
#include "edtcamdll.h"
#include "BGSubtractDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBGSubtractDlg dialog


CBGSubtractDlg::CBGSubtractDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBGSubtractDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBGSubtractDlg)
	m_bEnableBGImage = FALSE;
	m_sBackgroundImage = _T("");
	//}}AFX_DATA_INIT
}


void CBGSubtractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBGSubtractDlg)
	DDX_Check(pDX, IDC_ENABLE_BGIMG, m_bEnableBGImage);
	DDX_Text(pDX, IDC_BGIMAGE_NAME, m_sBackgroundImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBGSubtractDlg, CDialog)
	//{{AFX_MSG_MAP(CBGSubtractDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBGSubtractDlg message handlers
