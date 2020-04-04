// pdvshow.h : main header file for the PDVSHOW application
//

#if !defined(AFX_PDVSHOW_H__7E2B535D_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
#define AFX_PDVSHOW_H__7E2B535D_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


//#include "camera.h"
#include "PdvInput.h"

#include "dirname.h"

#include "EdtImageWin.h"


#ifdef USE_DIRECT_DRAW

#include "dd.h"
extern CDirectDraw g_dd;

#endif

#include "LiveDisplay.h"

//extern _declspec(dllimport) PdvInput g_theCamera;

/////////////////////////////////////////////////////////////////////////////
// CPdvshowApp:
// See pdvshow.cpp for the implementation of this class
//

class CPdvshowApp : public CWinApp
{
public:

	double m_dDefaultZoom;

	void ResetAllDocs();
	LiveDisplay * GetPump();
	PdvInput * GetCamera();
	void RestoreCaptureProfile();
	CString m_strModulePath;
	int m_Dbg;
	//int m_nUnitNumber;
	//int m_nChannelNumber;
	CString m_strDefaultCaptureDir;
	BOOL InitPump();
	CEdtImageWin * FindImageWindow(ImageWindowData *pWindowData);
	CEdtImageWin * FindTopWindow();
	void UpdateAllDocViews();
	CEdtImageWin * FindImageWindow(EdtImage *pImage);
	CMultiDocTemplate * m_pDocTemplate;

	LiveDisplay * m_pPump;
	PdvInput *m_pCamera;

	void GoLive();

	bool m_bAutoNumber;
	CString m_strFileName;

	bool GetAutoNumber() const {return m_bAutoNumber;}
	void SetAutoNumber(const int val) {m_bAutoNumber = (val != 0);}
    int SplitFileName(LPCTSTR fname, char *basename, char *extension, int max_size);
    void GetNextFileName(CString& lpszPathName);
        void GetNextFileName(LPCTSTR& lpszPathName);

	CPdvshowApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdvshowApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation

	//{{AFX_MSG(CPdvshowApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDVSHOW_H__7E2B535D_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
