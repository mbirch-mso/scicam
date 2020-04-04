#if !defined(AFX_DATAPANELDLG_H__B08F3A63_594D_4CA9_8CBC_B5142E351773__INCLUDED_)
#define AFX_DATAPANELDLG_H__B08F3A63_594D_4CA9_8CBC_B5142E351773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataPanelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataPanelDlg dialog

class _declspec(dllexport) CDataPanelDlg : public CDialog
{
// Construction
public:
	int m_nColumnsInserted;
	void SetFrameNumber(int n);
	void InitData();
	void UpdateTableItem(int row, int column, LPCSTR text);
	void UpdateTableItem(int row, int column, byte *pNew, BOOL bChanged);
	void UpdateTableItem(int row, int column, unsigned short *pNew, BOOL bChanged);
	void UpdateTableItem(int row, int column, short *pNew, BOOL bChanged);
	void UpdateTableItem(int row, int column, long *pNew, BOOL bChanged);
	void UpdateTableItem(int row, int column, float *pNew, BOOL bChanged);
	void UpdateTableItem(int row, int column, double *pNew, BOOL bChanged);

	void UpdateTable();
	void SetValues(int nPoints, int nType, int nColumns, 
		int nColumnWidth = 0, LPCSTR sFormat = NULL);
	void UpdateVector(void *pNewData);
	CDataPanelDlg(CWnd* pParent = NULL);   // standard constructor

	short m_nPoints;
	short m_dataType;
	void * m_dataVector;
	short m_columns;
	short m_columnWidth;
	CString m_dataFormat;

	int m_nElementSize;

	int m_nDataSize;
	void *m_pData;
	

// Dialog Data
	//{{AFX_DATA(CDataPanelDlg)
	enum { IDD = IDD_DATA_PANEL };
	CEdit	m_wndFrameNumber;
	CListCtrl	m_wndDataList;
	int		m_nFrameNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataPanelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataPanelDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAPANELDLG_H__B08F3A63_594D_4CA9_8CBC_B5142E351773__INCLUDED_)
