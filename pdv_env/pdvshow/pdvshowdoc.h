// pdvshowDoc.h : interface of the CPdvshowDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDVSHOWDOC_H__7E2B5366_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
#define AFX_PDVSHOWDOC_H__7E2B5366_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "EdtImage.h"

class CPdvshowDoc : public CDocument
{
protected: // create from serialization only
	CPdvshowDoc();
	DECLARE_DYNCREATE(CPdvshowDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdvshowDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bWasLoaded;
	PdvInput * m_pCamera;
	void AlignWithCamera();
	void CaptureLastImage();
	void CaptureImage();
	int m_nY1;
	int m_nX1;
	int m_nY2;
	int m_nX2;

	CString stash_name;
	bool stashed;


	CEdtImageWin * GetImageWindow();
	EdtImage * GetBaseImage();
	EdtImage * GetImage();
	EdtImage * m_pImage;
	virtual ~CPdvshowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPdvshowDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPdvshowDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDVSHOWDOC_H__7E2B5366_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
