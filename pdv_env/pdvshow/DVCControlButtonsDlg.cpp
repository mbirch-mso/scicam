// DVCControlButtonsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DVCControlButtonsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDVCControlButtonsDlg dialog


CDVCControlButtonsDlg::CDVCControlButtonsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDVCControlButtonsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDVCControlButtonsDlg)
	m_sLabel1 = _T("");
	m_sLabel2 = _T("");
	m_sLabel3 = _T("");
	m_sLabel4 = _T("");
	m_sLabel5 = _T("");
	m_sLabel6 = _T("");
	//}}AFX_DATA_INIT
}


void CDVCControlButtonsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDVCControlButtonsDlg)
	DDX_Text(pDX, IDC_LABEL_EDIT1, m_sLabel1);
	DDX_Text(pDX, IDC_LABEL_EDIT2, m_sLabel2);
	DDX_Text(pDX, IDC_LABEL_EDIT3, m_sLabel3);
	DDX_Text(pDX, IDC_LABEL_EDIT4, m_sLabel4);
	DDX_Text(pDX, IDC_LABEL_EDIT5, m_sLabel5);
	DDX_Text(pDX, IDC_LABEL_EDIT6, m_sLabel6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDVCControlButtonsDlg, CDialog)
	//{{AFX_MSG_MAP(CDVCControlButtonsDlg)
	ON_BN_CLICKED(IDC_EXAMINE1, OnExamine1)
	ON_BN_CLICKED(IDC_EXAMINE2, OnExamine2)
	ON_BN_CLICKED(IDC_EXAMINE3, OnExamine3)
	ON_BN_CLICKED(IDC_EXAMINE4, OnExamine4)
	ON_BN_CLICKED(IDC_EXAMINE5, OnExamine5)
	ON_BN_CLICKED(IDC_EXAMINE6, OnExamine6)
	ON_BN_CLICKED(IDC_LOAD_BUTTON1, OnLoadButton1)
	ON_BN_CLICKED(IDC_LOAD_BUTTON2, OnLoadButton2)
	ON_BN_CLICKED(IDC_LOAD_BUTTON3, OnLoadButton3)
	ON_BN_CLICKED(IDC_LOAD_BUTTON4, OnLoadButton4)
	ON_BN_CLICKED(IDC_LOAD_BUTTON5, OnLoadButton5)
	ON_BN_CLICKED(IDC_LOAD_BUTTON6, OnLoadButton6)
	ON_BN_CLICKED(IDC_SAVE_CURRENT1, OnSaveCurrent1)
	ON_BN_CLICKED(IDC_SAVE_CURRENT2, OnSaveCurrent2)
	ON_BN_CLICKED(IDC_SAVE_CURRENT3, OnSaveCurrent3)
	ON_BN_CLICKED(IDC_SAVE_CURRENT4, OnSaveCurrent4)
	ON_BN_CLICKED(IDC_SAVE_CURRENT5, OnSaveCurrent5)
	ON_BN_CLICKED(IDC_SAVE_CURRENT6, OnSaveCurrent6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVCControlButtonsDlg message handlers

void CDVCControlButtonsDlg::OnExamine1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnExamine2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnExamine3() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnExamine4() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnExamine5() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnExamine6() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnLoadButton1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnLoadButton2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnLoadButton3() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnLoadButton4() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnLoadButton5() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnLoadButton6() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnSaveCurrent1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnSaveCurrent2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnSaveCurrent3() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnSaveCurrent4() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnSaveCurrent5() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnSaveCurrent6() 
{
	// TODO: Add your control notification handler code here
	
}

void CDVCControlButtonsDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
