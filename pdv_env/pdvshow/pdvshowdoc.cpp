// pdvshowDoc.cpp : implementation of the CPdvshowDoc class
//

#include "stdafx.h"
#include "pdvshow.h"

#include "pdvshowDoc.h"
#include "pdvShowView.h"

#include "mainfrm.h"

#include "imagefileraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdvshowDoc

IMPLEMENT_DYNCREATE(CPdvshowDoc, CDocument)

BEGIN_MESSAGE_MAP(CPdvshowDoc, CDocument)
	//{{AFX_MSG_MAP(CPdvshowDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPdvshowDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CPdvshowDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IPdvshow to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7E2B535A-2BE6-11D2-8DF1-00A0C932D513}
static const IID IID_IPdvshow =
{ 0x7e2b535a, 0x2be6, 0x11d2, { 0x8d, 0xf1, 0x0, 0xa0, 0xc9, 0x32, 0xd5, 0x13 } };

BEGIN_INTERFACE_MAP(CPdvshowDoc, CDocument)
	INTERFACE_PART(CPdvshowDoc, IID_IPdvshow, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdvshowDoc construction/destruction

CPdvshowDoc::CPdvshowDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();

	m_pImage = NULL;

	m_pCamera = NULL;

	CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

	if (pApp)
		m_pCamera = pApp->GetCamera();

	m_bWasLoaded = false;

}

CPdvshowDoc::~CPdvshowDoc()
{
	AfxOleUnlockApp();
}

BOOL CPdvshowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	AlignWithCamera();

	if (m_pImage)
		m_pCamera->CaptureImage(m_pImage);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPdvshowDoc serialization

void CPdvshowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPdvshowDoc diagnostics

#ifdef _DEBUG
void CPdvshowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPdvshowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPdvshowDoc commands

EdtImage * CPdvshowDoc::GetImage()
{
	// Check to see if a subimage is defined
	// on the main view's window
	CEdtImageWin *pImageWindow = GetImageWindow();

	if (pImageWindow && pImageWindow->GetImage())
	{
		return pImageWindow->GetImage();
	}
	else

		return m_pImage;
}

EdtImage * CPdvshowDoc::GetBaseImage()
{

	return m_pImage;

}

CEdtImageWin * CPdvshowDoc::GetImageWindow()
{
	// Note - this kind of goes against document view 
	// but its better to have the important stuff in a 
	// reusable class like CEdtImageWindow

	POSITION pos = GetFirstViewPosition();

	if (!pos)
		return NULL;

	
	CPdvshowView *pView = (CPdvshowView *)GetNextView(pos);

	ASSERT(pView);

	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CPdvshowView)));

	return &pView->m_wndImageWindow;


}



void CPdvshowDoc::CaptureImage()
{
	// Check image type and size 
	AlignWithCamera();

	m_pCamera->CaptureImage(m_pImage);

	UpdateAllViews(NULL);

}

#include "imagefilemgr.h"

BOOL CPdvshowDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	if (m_pImage)
	{
	
		EdtImageFileMgr.LoadEdtImage(lpszPathName, m_pImage);

	} 
	else
	{
		
		m_pImage = EdtImageFileMgr.LoadEdtImage(lpszPathName, NULL);

		if (!m_pImage)

			return FALSE;
	}

	m_bWasLoaded = TRUE;

	return TRUE;
}



BOOL CPdvshowDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	BOOL rc = FALSE;

	CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

	if (m_pImage)
	{
		char imagetype[MAX_PATH];

        GetFileSuffix(lpszPathName,imagetype,511);

		if (m_pCamera->IsCapturingImages())
		{
			EdtImage *pImage = m_pCamera->GetLastImage();

			if (pImage && (pImage != m_pImage))
			{
				m_pImage->Copy(pImage);
			}
		}

        EdtImageFileMgr.SetPureRaw(TRUE);

        rc = (EdtImageFileMgr.SaveImage(lpszPathName, m_pImage, imagetype) == 0);

	}
	else
	{
		rc = CDocument::OnSaveDocument(lpszPathName);
		
	}

    pApp->m_strFileName = lpszPathName;

	return rc;
}

BOOL CPdvshowDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)

{
	bool rc;
    char imagetype[MAX_PATH];
    CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

	stashed = FALSE;

    if (lpszPathName)
    {
        pApp->GetNextFileName(lpszPathName);
        GetFileSuffix(lpszPathName,imagetype,511);

	    rc = (CDocument::DoSave(lpszPathName, bReplace) != 0);

	    if (stashed)
		    SetPathName(stash_name);
    }
    else rc = (CDocument::DoSave(lpszPathName, bReplace) != 0);

	return rc;

}


void CPdvshowDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class

	CEdtImageWin *pWindow = GetImageWindow();

	if (((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog)
	{
		if (pWindow == ((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog->GetWindow())
		{
			((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog->SetWindow(NULL);
		}
	}

	CDocument::OnCloseDocument();
}

void CPdvshowDoc::CaptureLastImage()
{
	EdtImage *pImage ;

	pImage = m_pCamera->GetLastImage();

	if (pImage && (pImage != m_pImage))
	{
		m_pImage->Copy(pImage);
	}

	UpdateAllViews(NULL);


}

void CPdvshowDoc::AlignWithCamera()
{

	if (m_pImage)
	{
		if (m_pImage->GetWidth() != m_pCamera->GetWidth() || 
			m_pImage->GetHeight() != m_pCamera->GetHeight() ||
			m_pImage->GetType() != m_pCamera->GetType())
		{

			delete m_pImage;

			m_pImage = NULL;
		}
	}

	if (!m_pImage)
	{

		m_pImage = m_pCamera->GetCameraImage();

		//m_pImage->SetDepth(m_pCamera->GetDepth());

		m_pImage->Clear();

		CEdtImageWin *pWindow = GetImageWindow();

		//pWindow->SetRemappingDisplay(TRUE);

		if (m_pCamera->GetDepth() > 8 &&
			m_pCamera->GetDepth() < 24) {
			m_pImage->SetMinMax(0,(1 << m_pCamera->GetDepth()) - 1);
		}
	}
}
