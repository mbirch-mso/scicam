// DataPanelDlg.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "DataPanelDlg.h"

#include "EdtImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataPanelDlg dialog


CDataPanelDlg::CDataPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataPanelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataPanelDlg)
	m_nFrameNumber = 0;
	//}}AFX_DATA_INIT

	m_nPoints = 128;
	m_dataType = TypeUSHORT;
	m_dataVector = NULL;
	m_columns = 8;
	m_columnWidth = 64;
	m_dataFormat ="%X";

	m_pData = NULL;
	m_nDataSize = 256;
	
	m_nColumnsInserted = 0;

}


void CDataPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataPanelDlg)
	DDX_Control(pDX, IDC_FRAME_NUMBER, m_wndFrameNumber);
	DDX_Control(pDX, IDC_DATA_LIST, m_wndDataList);
	DDX_Text(pDX, IDC_FRAME_NUMBER, m_nFrameNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataPanelDlg, CDialog)
	//{{AFX_MSG_MAP(CDataPanelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataPanelDlg message handlers

void CDataPanelDlg::UpdateVector(void *pNewData)
{

	if (!m_pData)
		InitData();

	for (int i = 0; i < m_nPoints; i++ )
	{
		int row = i / m_columns;
		int col = i % m_columns;

		switch (m_dataType)
		{
		case TypeBYTE:
			{
				byte *pCurrent = (byte *) m_pData + i;
				byte *pNew = (byte *) pNewData;

				if (pNew)
				{
					if (*pCurrent != pNew[i])
					{
						*pCurrent = pNew[i];
						UpdateTableItem(row, col, pCurrent, TRUE);
					}

				}
				else 
				{
					UpdateTableItem(row, col, pCurrent, TRUE);
				}
			}

			break;

		case TypeUSHORT:
			{
				unsigned short *pCurrent = (unsigned short *) m_pData + i;
				unsigned short *pNew = (unsigned short *) pNewData;

				if (pNew)
				{
					if (*pCurrent != pNew[i])
					{
						*pCurrent = pNew[i];
						UpdateTableItem(row, col, pCurrent, TRUE);
					}

				}
				else 
				{
					UpdateTableItem(row, col, pCurrent, TRUE);
				}
			}
			break;

		case TypeSHORT:
			{
				short *pCurrent = (short *) m_pData + i;
				short *pNew = (short *) pNewData;
				if (pNew)
				{
					if (*pCurrent != pNew[i])
					{
						*pCurrent = pNew[i];
						UpdateTableItem(row, col, pCurrent, TRUE);
					}

				}
				else 
				{
					UpdateTableItem(row, col, pCurrent, TRUE);
				}
			}
			break;

		case TypeINT:
			{
				long *pCurrent = (long *) m_pData + i;
				long *pNew = (long *) pNewData + i;

				if (pNew)
				{
					if (*pCurrent != pNew[i])
					{
						*pCurrent = pNew[i];
						UpdateTableItem(row, col, pCurrent, TRUE);
					}

				}
				else 
				{
					UpdateTableItem(row, col, pCurrent, TRUE);
				}
			}
			break;

		case TypeFLOAT:
			{
				float *pCurrent = (float *) m_pData + i;
				float *pNew = (float *) pNewData;

				if (pNew)
				{
					if (*pCurrent != pNew[i])
					{
						*pCurrent = pNew[i];
						UpdateTableItem(row, col, pCurrent, TRUE);
					}

				}
				else 
				{
					UpdateTableItem(row, col, pCurrent, TRUE);
				}
			}
			break;

		case TypeDOUBLE:
			{
				double *pCurrent = (double *) m_pData + i;
				double *pNew = (double *) pNewData;

				if (pNew)
				{
					if (*pCurrent != pNew[i])
					{
						*pCurrent = pNew[i];
						UpdateTableItem(row, col, pCurrent, TRUE);
					}

				}
				else 
				{
					UpdateTableItem(row, col, pCurrent, TRUE);
				}	
			}
			break;
		}
	}
}


int GetTypeBytes(const byte nType)
{
	int nPixelSize = 1;

	switch (nType)
	{
	case EDT_TYPE_CHAR:
	case EDT_TYPE_BYTE:
		nPixelSize =1;
		break;

	case EDT_TYPE_SHORT:
	case EDT_TYPE_USHORT:
		nPixelSize = 2;
		break;

	case EDT_TYPE_RGB:
	case EDT_TYPE_BGR:
		nPixelSize = 3;
		break;

	case EDT_TYPE_RGBA:
	case EDT_TYPE_BGRA:
		nPixelSize = 4;
		break;

	case EDT_TYPE_DOUBLE:
		nPixelSize = 8;
		break;

	default:
		nPixelSize = 4;
	}

	return nPixelSize;

}

void CDataPanelDlg::SetValues(int nPoints, int nType, int nColumns, int nColumnWidth,
							  LPCSTR sFormat)
{
	m_nPoints =nPoints;
	m_dataType = nType;

	m_nElementSize = GetTypeBytes(nType);

	int nDataSize = nPoints * m_nElementSize;


	if (nDataSize != m_nDataSize)
	{
		if (m_pData)
			free(m_pData);

		m_pData = (void *) malloc(nDataSize);

		m_nDataSize = nDataSize;
	}

	if (m_pData && m_nDataSize)
		memset(m_pData,0, m_nDataSize);

	m_columns = nColumns;
	if (nColumnWidth)
		m_columnWidth = nColumnWidth;
	if (sFormat)
		m_dataFormat = sFormat;

	if (m_hWnd)
		UpdateTable();


}

void CDataPanelDlg::UpdateTable()
{

	int i;

	if (!m_hWnd)
		return;

	// Set n columns

	m_wndDataList.DeleteAllItems();
	
	int nColumnCount = m_nColumnsInserted;

	// Delete all of the columns.
	for (i=0;i < nColumnCount;i++)
	{
	   m_wndDataList.DeleteColumn(0);
	}

	if (m_columns < 1)
		m_columns = 1;

	m_wndDataList.InsertColumn(0, " ",LVCFMT_LEFT, m_columnWidth);

	for (i=0;i<m_columns;i++)
	{
		CString s;
		s.Format(" %d ", i);

		m_wndDataList.InsertColumn(i+1,s, LVCFMT_RIGHT, m_columnWidth,i+1);

	}

	m_nColumnsInserted = m_columns;

	// Set n rows

	int rows = m_nPoints / m_columns;
	int row;

	if (m_nPoints % m_columns)
		rows++;

	for (row=0;row<rows;row	++)
	{
		CString s;
		
		s.Format("%-3d:",row * m_columns);
		m_wndDataList.InsertItem(row,s);

	}

	UpdateVector(NULL);

}

void CDataPanelDlg::UpdateTableItem(int row, int column, byte *pData, BOOL bChanged)
{
		CString s;

	if (m_dataFormat.GetLength())
		s.Format(m_dataFormat, *pData);
	else
		s.Format("%x",*pData);

	UpdateTableItem(row,column, s);
}

void CDataPanelDlg::UpdateTableItem(int row, int column, unsigned short *pData, BOOL bChanged)
{
	CString s;

	if (m_dataFormat.GetLength())
		s.Format(m_dataFormat, *pData);
	else
		s.Format("%x",*pData);

	UpdateTableItem(row,column, s);

}

void CDataPanelDlg::UpdateTableItem(int row, int column, short *pData, BOOL bChanged)
{
	CString s;

	if (m_dataFormat.GetLength())
		s.Format(m_dataFormat, *pData);
	else
		s.Format("%x",*pData);

	UpdateTableItem(row,column, s);
}

void CDataPanelDlg::UpdateTableItem(int row, int column, long *pData, BOOL bChanged)
{
	CString s;

	if (m_dataFormat.GetLength())
		s.Format(m_dataFormat, *pData);
	else
		s.Format("%x",*pData);

	UpdateTableItem(row,column, s);
}

void CDataPanelDlg::UpdateTableItem(int row, int column, float *pData, BOOL bChanged)
{
	CString s;

	if (m_dataFormat.GetLength())
		s.Format(m_dataFormat, *pData);
	else
		s.Format("%g",*pData);

	UpdateTableItem(row,column, s);
}

void CDataPanelDlg::UpdateTableItem(int row, int column, double *pData, BOOL bChanged)
{
	CString s;

	if (m_dataFormat.GetLength())
		s.Format(m_dataFormat, *pData);
	else
		s.Format("%g",*pData);

	UpdateTableItem(row,column, s);
}

BOOL CDataPanelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateTable();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataPanelDlg::UpdateTableItem(int row, int column, LPCSTR text)
{

	if (m_hWnd && text)
      m_wndDataList.SetItemText(row, column+1, text);
	
	
}

   

void CDataPanelDlg::InitData()
{
	if (!m_pData)
	{
		m_pData = (void *) malloc(m_nDataSize);
		memset(m_pData,0, m_nDataSize);
	}
		
}

void CDataPanelDlg::SetFrameNumber(int n)
{
	if (m_wndFrameNumber.m_hWnd)
	{

		CString s;
		
		s.Format("%d", n);

		m_wndFrameNumber.SetWindowText(s);

	}

}
