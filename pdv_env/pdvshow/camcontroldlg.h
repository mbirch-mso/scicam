// CCamControlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamControlDlg dialog

#include "edtdefines.h"

#include "PdvInput.h"


class DLLEXPORT CCamControlDlg : public CDialog
{
// Construction
public:
	bool SetLevel();
	bool SetGain();
	bool SetShutter();
	CCamControlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	bool	m_bInitialized;
	bool	m_bUpdating;
	bool	m_bUpdating1;

	bool	m_bContinuous;

	int		m_nShutter2;

	PdvInput *m_pCamera;

	void SetCamera(PdvInput *pCamera)
	{
		m_pCamera = pCamera;
	}

	bool m_bShutterChanged;
	bool m_bLevelChanged;
	bool m_bGainChanged;

	//{{AFX_DATA(CCamControlDlg)
	enum { IDD = IDD_CAM_CONTROL };
	CSpinButtonCtrl	m_wndSpinShutter;
	CSpinButtonCtrl	m_wndSpinLevel;
	CSpinButtonCtrl	m_wndSpinGain;
	CSliderCtrl	m_wndGainScroll;
	CSliderCtrl	m_wndLevelScroll;
	CSliderCtrl	m_wndSpeedScroll;
	CEdit	m_ctlShutter;
	CEdit	m_ctlGain;
	CEdit	m_ctlLevel;
	int		m_nGain;
	int		m_nLevel;
	int		m_nShutter;
	BOOL	m_bHardwareInvert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamControlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHardwareInvert();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditGain();
	afx_msg void OnKillfocusEditLevel();
	afx_msg void OnKillfocusEditShutter();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
