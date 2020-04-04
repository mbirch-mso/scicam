// RecordLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"

#include "RecordLogDlg.h"

#include "dispatch/ErrorHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordLogDlg dialog


CRecordLogDlg::CRecordLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordLogDlg)
	m_sLogComment = _T("");
	m_sLogFileName = _T("");
	//}}AFX_DATA_INIT

	m_bModified = FALSE;

}


void CRecordLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordLogDlg)
	DDX_Text(pDX, IDC_LOG_COMMENT, m_sLogComment);
	DDX_Text(pDX, IDC_LOG_FILE_NAME, m_sLogFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordLogDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordLogDlg)
	ON_BN_CLICKED(IDC_ADD_COMMENT, OnAddComment)
	ON_BN_CLICKED(IDC_ADD_TIMESTAMPED, OnAddTimestamped)
	ON_BN_CLICKED(IDC_CLEAR_LOG_FILE, OnClearLogFile)
	ON_BN_CLICKED(IDC_SAVE_LOG_FILE, OnSaveLogFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordLogDlg message handlers

void CRecordLogDlg::OnAddComment() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_sLogComment += "\n";

	AddCommentString(m_sLogComment);

}

void CRecordLogDlg::AddCommentString(const char * fmt, ...)
{
	CRichEditCtrl *pCtrl = (CRichEditCtrl *) GetDlgItem(IDC_LOGWINDOW);


	if (pCtrl)
	{
		//
		va_list stack;

		va_start(stack,fmt);

		char buffer[1024];

		vsprintf(buffer,fmt, stack);

		pCtrl->ReplaceSel(buffer);

		m_bModified = TRUE;
	}
}

void CRecordLogDlg::Timestamp(const char * fmt, ...)
{
	va_list stack;

	va_start(stack,fmt);

	char buffer[1024];

	vsprintf(buffer,fmt, stack);


	CTime t = CTime::GetCurrentTime();
	
	CString ts = t.Format("%H:%M:%S : ");

	AddCommentString(ts);

	AddCommentString(buffer);

	

}

void CRecordLogDlg::OnAddTimestamped() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_sLogComment += "\n";

	Timestamp(m_sLogComment);

	

}

void CRecordLogDlg::OpenLogFile(const char * pFileName)
{


	if (m_sLogFileName.GetLength() && m_bModified)
	{
		SaveLogFile();
	}
	
	ClearBuffer();
	
	if (pFileName)
	{
		
		m_sLogFileName = pFileName;

		if (m_sLogFileName.GetLength() == 0)
			return;

		if (m_LogFile = fopen(pFileName,"r"))
		{
			char buffer[1025];

			int bytesread;
			
			ClearBuffer();

			while (bytesread = fread(buffer,1,1024,m_LogFile))
			{
				buffer[bytesread] = 0;

				AddCommentString(buffer);
			}
			
			fclose(m_LogFile);
			
		}
	}

	UpdateData(FALSE);
}

void CRecordLogDlg::SaveLogFile()
{
	if (m_sLogFileName.GetLength())
	{
		
		CString data;

		if (m_LogFile = fopen(m_sLogFileName,"w"))
		{
			CRichEditCtrl *pCtrl = (CRichEditCtrl *) GetDlgItem(IDC_LOGWINDOW);

			pCtrl->GetWindowText(data);
			
			int bytesread = fwrite(data.GetBuffer(1),1,data.GetLength(), m_LogFile);

			if (bytesread != data.GetLength())
			{
				SysError.SysErrorMessage("Error writing to %s", m_sLogFileName);
			}
		
			m_bModified = FALSE;
		}
		else
		{
			SysError.SysErrorMessage("Error Opening File %s", m_sLogFileName);
		}
		
		if (m_LogFile)
			fclose(m_LogFile);

	}
}

void CRecordLogDlg::ClearBuffer()
{
		
	CRichEditCtrl *pCtrl = (CRichEditCtrl *) GetDlgItem(IDC_LOGWINDOW);

	if (pCtrl)
	{
		//
		pCtrl->SetSel(0,-1);
		pCtrl->ReplaceSel("");

	}

}

void CRecordLogDlg::OnClearLogFile() 
{
	// TODO: Add your control notification handler code here
	ClearBuffer();
}

void CRecordLogDlg::OnSaveLogFile() 
{
	// TODO: Add your control notification handler code here
	SaveLogFile();
}
