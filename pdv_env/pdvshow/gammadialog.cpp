// GammaDialog.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

//#include "edtcamdll.h"
#include "GammaDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CGammaDialog dialog


CGammaDialog::CGammaDialog(CWnd* pParent /*=NULL*/)
: CDialog(CGammaDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CGammaDialog)
    m_dGammaValue = 1.0;
    //}}AFX_DATA_INIT
}


void CGammaDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGammaDialog)
    DDX_Control(pDX, IDC_GAMMA_SLIDER, m_wndGammaSlider);
    DDX_Text(pDX, IDC_EDIT1, m_dGammaValue);
    DDV_MinMaxDouble(pDX, m_dGammaValue, 1.e-002, 5.);
    //}}AFX_DATA_MAP

    if (pDX->m_bSaveAndValidate)
	m_wndGammaSlider.SetPos((int) (m_dGammaValue * 1000));

}


BEGIN_MESSAGE_MAP(CGammaDialog, CDialog)
    //{{AFX_MSG_MAP(CGammaDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnSetGamma)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGammaDialog message handlers

void CGammaDialog::OnSetGamma() 
{
    // TODO: Add your control notification handler code here

    UpdateData(TRUE);

    // Compute gamma table

    double dgam;

    unsigned char gamma_table[3][256];

    for (int i = 0; i< 256; i++)
    {
	dgam = 255.0 * pow((double)i/255.0, 1.0 / m_dGammaValue);

	gamma_table[0][i] = gamma_table[1][i] = gamma_table[2][i] = (unsigned char) dgam;

    }

    HDC hdc;
    HWND root = ::GetDesktopWindow();
    hdc = ::GetDC(root);

    bool rc = (bool)(SetDeviceGammaRamp(hdc, gamma_table) != 0);

    if (!rc)
    {

	CString err;
	DWORD nerr = GetLastError();

	AfxMessageBox("SetDeviceGammaRamp failed");
    }
}

void CGammaDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // TODO: Add your message handler code here and/or call default
    // TODO: Add your message handler code here and/or call default

    int nControl = pScrollBar->GetDlgCtrlID();
    CSliderCtrl * pControl = (CSliderCtrl *) GetDlgItem(nControl);
    CString s;
    int nGamma;

    static int incall = FALSE;

    switch(nControl) {

		case IDC_GAMMA_SLIDER:
		    nGamma= pControl->GetPos();

		    m_dGammaValue = nGamma * 0.001;


		    UpdateData(FALSE);

		    if (nSBCode == SB_ENDSCROLL) 
		    {
			OnSetGamma();
		    }


		    break;

    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CGammaDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    m_wndGammaSlider.SetRange(0,5000);

    // Set the slider range
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
