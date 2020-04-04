// pdvshowView.cpp : implementation of the CPdvshowView class
//

#include "stdafx.h"
#include "pdvshow.h"

#include "pdvshowDoc.h"
#include "pdvshowView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdvshowView

IMPLEMENT_DYNCREATE(CPdvshowView, CView)

BEGIN_MESSAGE_MAP(CPdvshowView, CView)
	//{{AFX_MSG_MAP(CPdvshowView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_ZOOM_STATE_UPDATE, OnZoomStatusUpdate)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdvshowView construction/destruction

CPdvshowView::CPdvshowView()
{
	// TODO: add construction code here

}

CPdvshowView::~CPdvshowView()
{
}

BOOL CPdvshowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPdvshowView drawing

void CPdvshowView::OnDraw(CDC* pDC)
{
	CPdvshowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
//	EdtImage *pImage = pDoc->GetImage();

//	m_wndImageWindow.SetImageHandle(pImage);

}

/////////////////////////////////////////////////////////////////////////////
// CPdvshowView printing

BOOL CPdvshowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPdvshowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPdvshowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPdvshowView diagnostics

#ifdef _DEBUG
void CPdvshowView::AssertValid() const
{
	CView::AssertValid();
}

void CPdvshowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPdvshowDoc* CPdvshowView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPdvshowDoc)));
	return (CPdvshowDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPdvshowView message handlers

int CPdvshowView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect;

	GetClientRect(&rect);
	m_wndImageWindow.Create(NULL,WS_CHILD|WS_VISIBLE,rect,this);

	CPdvshowApp * pApp = (CPdvshowApp * ) ::AfxGetApp();

	m_wndImageWindow.SetZoom(0,0,pApp->m_dDefaultZoom, pApp->m_dDefaultZoom);

	return 0;
}

BOOL CPdvshowView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CPdvshowView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CPdvshowDoc* pDoc = GetDocument();
	
	if (pDoc)
	{	
		EdtImage* pImage = pDoc->GetBaseImage();

	//	SizeToImage();

	//	SetScrollSizes(MM_TEXT, CSize(pImage->GetWidth(), pImage->GetHeight()));
		
		m_wndImageWindow.SetImageHandle(pImage,NULL);
		m_wndImageWindow.AdjustScrollbars();
		
		m_wndImageWindow.Invalidate(FALSE);

		m_wndImageWindow.UpdateWindow();
	}
	
}

void CPdvshowView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	m_wndImageWindow.MoveWindow(0,0,cx,cy);



}

void CPdvshowView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_wndImageWindow.SendMessage(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));

	((CMainFrame *) AfxGetMainWnd())->OnViewMouseMove(this, nFlags, point);

	CView::OnMouseMove(nFlags, point);
}

void CPdvshowView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	((CMainFrame *) AfxGetMainWnd())->OnViewLButtonDown(this, nFlags, point);

	m_wndImageWindow.SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
	
	CView::OnLButtonDown(nFlags, point);
}

void CPdvshowView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_wndImageWindow.SendMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));

	((CMainFrame *) AfxGetMainWnd())->OnViewLButtonUp(this, nFlags, point);
	
	CView::OnLButtonUp(nFlags, point);
}

void CPdvshowView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
	((CMainFrame *) AfxGetMainWnd())->CloseViewHooks(this);
}

void CPdvshowView::OnActivateView(bool bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog)
		((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog->SetWindow(&m_wndImageWindow);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CPdvshowView::SizeToImage()
{
	CPdvshowDoc* pDoc = GetDocument();
	
	if (pDoc)
	{	
		EdtImage* pImage = pDoc->GetBaseImage();
		CWnd *pFrm = GetParent();
		
		if (!pFrm)
			return;

		WINDOWPLACEMENT wp;
		pFrm->GetWindowPlacement(&wp);

		CRect cp = wp.rcNormalPosition;


		cp.right = cp.left +
			pImage->GetWidth();
		cp.right = cp.left +
			pImage->GetHeight();

		pFrm->MoveWindow(&cp);
		cp -= cp.TopLeft();

		MoveWindow(&cp);
	}

}

void CPdvshowView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_wndImageWindow.SendMessage(WM_VSCROLL, MAKEWPARAM(nSBCode,nPos), (LPARAM)pScrollBar->m_hWnd);
	
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPdvshowView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_wndImageWindow.SendMessage(WM_HSCROLL, MAKEWPARAM(nSBCode,nPos), (LPARAM)pScrollBar->m_hWnd);
	
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPdvshowView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_wndImageWindow.SendMessage(WM_RBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
	
	CView::OnRButtonUp(nFlags, point);
}

void CPdvshowView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_wndImageWindow.SendMessage(WM_RBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
	
	CView::OnRButtonDown(nFlags, point);
}

LRESULT CPdvshowView::OnZoomStatusUpdate(WPARAM wparam, LPARAM lparam)

{

	if (((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog)
	{
		((CMainFrame *) AfxGetMainWnd())->m_pZoomDialog->UpdateFromWindow();
	}

	return 0;
}

void CPdvshowView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	GetParentFrame()->ShowWindow(SW_SHOWMAXIMIZED);

}
