#if !defined(AFX_HISTDIALOG_H__8C1D618D_2AFB_11D2_97D9_00104B357776__INCLUDED_)
#define AFX_HISTDIALOG_H__8C1D618D_2AFB_11D2_97D9_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HistDialog.h : header file
//

#include "edtimage.h"
#include "EdtHistogram.h"
#include "EdtPlotWindow.h"



/////////////////////////////////////////////////////////////////////////////
// CHistDialog dialog

class DLLEXPORT CHistDialog : public CDialog
{
// Construction
public:
	void UpdateStatistics();
	LRESULT OnPlotButton(WPARAM wparam, LPARAM lparam);
	int * m_pBitCounts[1];
	BOOL m_bVisible;
	EdtImgStats m_Stats[EDT_MAX_COLOR_PLANES];
	void Compute(EdtImage *pImage);
	void RefreshData();
	EdtHistogram m_Hist;
	EdtImage * m_pImage;
	CHistDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistDialog)
	enum { IDD = IDD_HIST_DIALOG };
	CStatic	m_wndCursorValues;
	CStatic	m_wndSMedian[3];
	CStatic	m_wndSNPixels;
	CStatic	m_wndSMinPixel[3];
	CStatic	m_wndSSD[3];
	CStatic	m_wndSMean[3];
	CStatic	m_wndSMaxPixel[3];
	CString	m_sMaxPixel[3];
	CString	m_sMean[3];
	CString	m_sMedian[3];
	CString	m_sMinPixel[3];
	CString	m_sNPixels;
	CString	m_sSD[3];
	CEdtPlotWindow	m_wndHistWindow;
	int		m_nDoPixels;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistDialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistDialog)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPixelValues();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CHistDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTDIALOG_H__8C1D618D_2AFB_11D2_97D9_00104B357776__INCLUDED_)
