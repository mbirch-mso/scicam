// CCamProgramDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "edtcam.h"

#include "resource.h"
#include "CamProgramDlg.h"
#include "PdvInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Global references
//extern CCamera g_theCamera;

/////////////////////////////////////////////////////////////////////////////
// CCamProgramDlg dialog

CCamProgramDlg::CCamProgramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamProgramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamProgramDlg)
	m_nIshex = -1;
	//}}AFX_DATA_INIT

	m_pCamera = NULL;
}


void CCamProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamProgramDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nIshex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamProgramDlg, CDialog)
	//{{AFX_MSG_MAP(CCamProgramDlg)
	ON_EN_CHANGE(IDC_CAM_INPUT, OnChangeCamInput)
	ON_EN_KILLFOCUS(IDC_CAM_INPUT, OnKillfocusCamInput)
	ON_EN_MAXTEXT(IDC_CAM_INPUT, OnMaxtextCamInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamProgramDlg message handlers

BOOL CCamProgramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_CAM_INPUT);
	pEdit->LimitText(); 
	pEdit->SetWindowText("");
	pEdit->SetFocus();
	if (m_nIshex < 0)
		m_nIshex = FALSE;
	UpdateData(FALSE);
	return FALSE;
}

void CCamProgramDlg::OnChangeCamInput() 
{
	// Read input control
	char str[256];
	GetDlgItemText(IDC_CAM_INPUT, str, sizeof(str)); 
	
	// Update output control if newline entered
 	size_t n = strlen(str);
	if (str[n-1] == '\r' || str[n-1] == '\n')
	{	
		OnMaxtextCamInput();
	}	
}

void CCamProgramDlg::OnKillfocusCamInput() 
{
	// Clear input control
	GetDlgItem(IDC_CAM_INPUT)->SetWindowText(""); 
}

void CCamProgramDlg::OnMaxtextCamInput() 
{
	// Read input control
	char szCommand[256];
	char dpystr[256];
	size_t length=0;
	
	GetDlgItemText(IDC_CAM_INPUT, szCommand, sizeof(szCommand));
	
	UpdateData(TRUE);
	if (m_nIshex == 1)
	{
		int i, nb=0;
		char *p;
		char tmpCommand[256];
		char revCommand[256];
		char *rp = &revCommand[0];
		char *fp;
		int nibbles = 0;
		int idx = 0;

	  	if (strlen(szCommand) < 0)
			return;
	
		p = &szCommand[strlen(szCommand)-1];
			
		while (p >= szCommand)
		{
			if (*p == '\r' || *p == '\n' || *p == '\0')
			{
				if (nibbles > 0)
					*rp++ = '0';
				--p;
			}
			else if (*p == '	' || *p == ' ')
			{
				if (nibbles == 1)
					*rp++ = '0';
				nibbles = 0;
				*rp++ = ' ';
				--p;
			}
			else if (isxdigit(*p))
			{
				*rp++ = *p--;
				
				if (++nibbles > 1)
				{
					++nb;
					nibbles = 0;
					*rp++ = ' ';
				}
			}
			else
			{
				char msg[256];
				sprintf(msg, "Serial command failed -- hex mode selected, non-hex character encountered in string");
				AfxMessageBox(msg, MB_OK|MB_ICONEXCLAMATION,0);
				return;
			}
			idx++;
			*rp = '\0';
		}

		if (nibbles > 0)
		{
			*rp++ = '0';
		    *rp = '\0';
		}
		
		fp = &revCommand[strlen(revCommand)-1];
		for (i=0; i<(int)strlen(revCommand); i++)
		{
			tmpCommand[i] = *fp--;
		}
		tmpCommand[i] = '\0';
		strcpy(szCommand, tmpCommand);
	}

	length = strlen(szCommand);
	dpystr[0] = '\r';
	dpystr[1] = '\n';
	dpystr[2] = '>';
	strcpy(dpystr+3, szCommand);
	strcat(dpystr, "\r\n");
	strcat(szCommand, m_pCamera->SerialTerminator()); // was \r\n
	
	// Clear input control
	GetDlgItem(IDC_CAM_INPUT)->SetWindowText(""); 
	
	// Update output control
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_CAM_OUTPUT);
	int len = pEdit->GetWindowTextLength(); 
	pEdit->SetSel(len, len);
	if (len)
	{	pEdit->ReplaceSel((LPCSTR) dpystr);
	}
	else
	{	pEdit->ReplaceSel((LPCSTR) dpystr+2);
	}
	
	// send the input to the camera
	if (m_nIshex != 1)
		szCommand[strlen(szCommand)] = '\0';

	char outbuf[1000];
	
	if (m_pCamera->SendCommand(szCommand, m_nIshex, outbuf))
	{

		AppendText(outbuf);

		while (m_pCamera->ReadResponse(outbuf,m_nIshex))
			AppendText(outbuf);

	}
	else
		AppendText(outbuf);
}

void CCamProgramDlg::SetSerialFormatButton(int flag)
{
	m_nIshex = flag;
		UpdateData(FALSE);
}

void CCamProgramDlg::AppendText(const char *s)

{
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_CAM_OUTPUT);

	int len;
	len = pEdit->GetWindowTextLength(); 
	pEdit->SetSel(len, len);
	pEdit->ReplaceSel(s);
}
