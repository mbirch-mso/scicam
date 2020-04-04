#if !defined(AFX_WHITEBALANCESETUP_H__1817DE62_D12A_4925_A033_7A7B6032E006__INCLUDED_)
#define AFX_WHITEBALANCESETUP_H__1817DE62_D12A_4925_A033_7A7B6032E006__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhiteBalanceSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhiteBalanceSetup dialog

class CWhiteBalanceSetup : public CDialog
{
// Construction
public:
	CWhiteBalanceSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWhiteBalanceSetup)
	enum { IDD = IDD_WHITE_BALANCE_SETUP };
	BOOL	m_bAutoRange;
	int		m_nMaxValue;
	int		m_nMinValue;
	BOOL	m_bUseRatios;
	double	m_dSamplePercent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhiteBalanceSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhiteBalanceSetup)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHITEBALANCESETUP_H__1817DE62_D12A_4925_A033_7A7B6032E006__INCLUDED_)
