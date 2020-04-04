#if !defined(AFX_CAMSELECTDIALOG_H__ED541C72_66C1_11D3_8BBF_00104B357776__INCLUDED_)
#define AFX_CAMSELECTDIALOG_H__ED541C72_66C1_11D3_8BBF_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CamSelectDialog.h : header file
//

#include "edtdefines.h"

#include <afxcmn.h>

#include "PdvInput.h"

class CCameraRecord
{
public:
	char szCamFile[_MAX_PATH];
	char szCamType[_MAX_PATH];
	char szCamClass[_MAX_PATH];
} ;

/////////////////////////////////////////////////////////////////////////////
// CCamSelectDialog dialog

class DLLEXPORT CCamSelectDialog : public CDialog
{
// Construction
public:
	void SortAllEntries();
	HTREEITEM LookupClassString(const char *s);
	bool m_bEdited;
	bool m_bAllowNewEntries;
	void UpdateFromList();
	void ClearCamList();
	void SetListBox();
	void SetCameraPath(LPCSTR szNewPath);
	CCamSelectDialog(CWnd* pParent = NULL,
					LPCSTR szConfigPath = NULL ,
					LPCSTR szCamType  = NULL );   // standard constructor
	virtual ~CCamSelectDialog();

	CPtrArray  m_CamList;
	CPtrArray  m_ClassList;

	void BuildCameraList();

	bool ReadNextCameraType(LPTSTR&, CString&);
	CCameraRecord* LookupCamera(const CString&);

	void SetAllowNewEntries(const bool bValue)
	{
		m_bAllowNewEntries = bValue;
	}

	bool GetAllowNewEnties() const
	{
		return m_bAllowNewEntries;
	}

	PdvInput *m_pCamera;

	void SetCamera(PdvInput *pCamera)
	{
		m_pCamera = pCamera;
	}

    bool IsClink(PdvInput *pCamera = NULL);
    bool IsFox(PdvInput *pCamera = NULL);

	bool GetConfigFile(PdvInput *pCamera = NULL);

	bool SelectConfigFile(PdvInput *pCamera = NULL, bool bAllowNew = FALSE);


	// Dialog Data
	//{{AFX_DATA(CCamSelectDialog)
	enum { IDD = IDD_CAM_SELECT };
	CTreeCtrl	m_wndCameraList;
	CEdit	m_wndCamType;
	CEdit	m_wndCamConfig;
	CString	m_sConfigPath;
	CString	m_sCamType;
	CString	m_sCamFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamSelectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamSelectDialog)
	afx_msg void OnBrowsePath();
	afx_msg void OnKillfocusCamlist();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCamlist();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCamconfig();
	afx_msg void OnUpdateCamtype();
	afx_msg void OnSelchangedCamlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMSELECTDIALOG_H__ED541C72_66C1_11D3_8BBF_00104B357776__INCLUDED_)
