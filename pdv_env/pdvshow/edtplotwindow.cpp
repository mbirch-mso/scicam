// EdtPlotWindow.cpp : implementation file
//

#include "stdafx.h"

#include "EdtPlotWindow.h"
#include "math.h"
#include "edt_utils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdtPlotWindow

CEdtPlotWindow::CEdtPlotWindow()
{
    m_bPrinting = FALSE;
    m_nVertTicks = 10;
    m_pData = NULL;
    m_nHorzTicks = m_nPeriods = 0;
    m_lMaxValue = 0;
    m_bClosing = FALSE;

    m_nLastLineX = -1;

    EnableAutomation();
}

CEdtPlotWindow::~CEdtPlotWindow()
{
}

void CEdtPlotWindow::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.

    CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CEdtPlotWindow, CWnd)
    //{{AFX_MSG_MAP(CEdtPlotWindow)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CEdtPlotWindow, CWnd)
    //{{AFX_DISPATCH_MAP(CEdtPlotWindow)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IEdtPlotWindow to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8C1D618A-2AFB-11D2-97D9-00104B357776}
static const IID IID_IEdtPlotWindow =
{ 0x8c1d618a, 0x2afb, 0x11d2, { 0x97, 0xd9, 0x0, 0x10, 0x4b, 0x35, 0x77, 0x76 } };

BEGIN_INTERFACE_MAP(CEdtPlotWindow, CWnd)
    INTERFACE_PART(CEdtPlotWindow, IID_IEdtPlotWindow, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtPlotWindow message handlers

int CEdtPlotWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
	return -1;

    // TODO: Add your specialized creation code here

    return 0;
}

void CEdtPlotWindow::OnSize(UINT nType, int cx, int cy) 
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here

}

BOOL CEdtPlotWindow::Reset(int** pData, int lPeriods, int lMaxValue, int nColors)
{
    if (m_bClosing)
    {	
	return FALSE;
    }
    m_pData = pData;
    m_nHorzTicks = m_nPeriods = lPeriods;
    m_lMaxValue = lMaxValue;
    m_nColors = nColors;
    return TRUE;
}

BOOL CEdtPlotWindow::Create(int** pData, int lPeriods, int lMaxValue, int nColors)

{

    return Reset(pData,lPeriods, lMaxValue, nColors);

}

void CEdtPlotWindow::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (m_bClosing)
    {	
	return;
    }

    if (!m_pData)
    {	
	return;
    }

    if (!GetGraphMetrics(&dc))
    {	
	return;
    }

    // black pen and white background	
    CBrush* pbrushOld = (CBrush*) dc.SelectStockObject(WHITE_BRUSH);
    CPen penNew(PS_SOLID, 2, (COLORREF) 0);
    CPen* ppenOld = (CPen*) dc.SelectObject(&penNew);

    dc.Rectangle(m_rectGraph);
    if (!m_bOmitGraph)
    {	PlotAxes(&dc);
    PlotCaptions(&dc);
    PlotGraph(&dc);
    }

    // reset dc object
    dc.SelectObject(pbrushOld);
    dc.SelectObject(ppenOld);
}

double CEdtPlotWindow::GetTickValue()
{
    double dMult = 1;

    // Determine values for vertical ticks 
    for (int i = 0;; i++)
    {
	if (m_lMaxValue <= .05 * dMult) return .005 * dMult;
	if (m_lMaxValue <= .10 * dMult) return .010 * dMult; 
	if (m_lMaxValue <= .15 * dMult) return .015 * dMult; 
	if (m_lMaxValue <= .20 * dMult) return .020 * dMult; 
	if (m_lMaxValue <= .25 * dMult) return .025 * dMult;
	if (m_lMaxValue <= .30 * dMult) return .030 * dMult;
	if (m_lMaxValue <= .35 * dMult) return .035 * dMult;
	if (m_lMaxValue <= .40 * dMult) return .040 * dMult;
	if (m_lMaxValue <= .45 * dMult) return .045 * dMult;
	if (m_lMaxValue <= .50 * dMult) return .050 * dMult;
	if (m_lMaxValue <= .55 * dMult) return .055 * dMult;
	if (m_lMaxValue <= .60 * dMult) return .060 * dMult;
	if (m_lMaxValue <= .65 * dMult) return .065 * dMult;
	if (m_lMaxValue <= .70 * dMult) return .070 * dMult;
	if (m_lMaxValue <= .75 * dMult) return .075 * dMult;
	dMult *= 10;
    }
}

void CEdtPlotWindow::GetDisplayRect(CPaintDC* pDC, CRect& rc)
{
    if (pDC->IsPrinting())
    {
	rc.left = rc.top = 0;
	rc.right  = pDC->GetDeviceCaps(HORZRES);
	rc.bottom = pDC->GetDeviceCaps(VERTRES);
    }
    else
    {
	GetClientRect(&rc);
	m_rectWnd = rc;
	//rc = m_rectWnd;
    }
}

//
// Get info about the size of the graph 
// if window is too small to display text bOmitText is set true
// if window is too small to display graph function returns a FALSE
// 
BOOL CEdtPlotWindow::GetGraphMetrics(CPaintDC* pDC)
{
    GetDisplayRect(pDC, m_rectGraph);

    int w, wadj;
    int h, hadj;

    int nMaxCharsX = GetNDigits(m_nPeriods) + 1;

    m_fXAspect = (float) edt_max(1, pDC->GetDeviceCaps(ASPECTX));
    m_fYAspect = (float) edt_max(1, pDC->GetDeviceCaps(ASPECTY)); 

    CSize sizeText = pDC->GetTextExtent("0", 1);
    m_nXChar = (short) edt_max(1, sizeText.cx);
    m_nYChar = (short) edt_max(1, sizeText.cy);

    int nLeftMargin = 5 * m_nXChar;
    int nRightMargin = nLeftMargin;
    int nTopMargin = 2 * m_nYChar;
    int nBottomMargin = nTopMargin;

    if (m_bPrinting)	// size plotting rectangle for printer
    {
	// Create a smaller rectangle for the graph background so that
	// there is room for printing text at top and bottom and keep
	// the graph as square as possible
	w = m_rectGraph.Width();
	h = m_rectGraph.Height();
	int nw = w - (2 * nLeftMargin + 2 * nRightMargin); 
	int nh = h - (nTopMargin + nBottomMargin + 20 * m_nYChar);
	nw = edt_min(nw, (int) ((float) nh / m_fXAspect * m_fYAspect));
	nh = edt_min(nh, (int) ((float) nw / m_fXAspect * m_fYAspect));
	wadj = (w - nw) / 2;
	hadj = nTopMargin + 4 * m_nYChar;
	m_rectGraph.left += wadj;
	m_rectGraph.right = m_rectGraph.left + nw;
	m_rectGraph.top += hadj;
	m_rectGraph.bottom -= hadj;
    }
    else			// size plotting rectangle for display
    {
	// Create a smaller rectangle for the graph background so that a
	// grey border exists least 5 pixels wide also keep the graph
	// from being deeper than it is wide. 
	//w = m_rectGraph.Width();
	//if (w < 25) return FALSE;	// window too small for plotting
	//wadj = edt_min(5, w / 20);
	//m_rectGraph.left += wadj;
	//m_rectGraph.right -= wadj;
    }

    // Determine minimum size for graph

    m_bOmitCaption = m_bOmitTicks = m_bOmitGraph = FALSE;

    // First try captions, and tick marks
    // Allow 6 chars for Vertical Captions and 3 chars for space on right and
    // 8 chars for body.  Allow 3 chars for space at bottom and top of graph.
    m_sizeGraph.cx = m_nXChar * 17;
    m_sizeGraph.cy = m_nYChar * (3 + m_nVertTicks);
    if (m_sizeGraph.cx > m_rectGraph.Width()) m_bOmitCaption = TRUE;  
    if (m_sizeGraph.cy > m_rectGraph.Height()) m_bOmitCaption = TRUE;

    if (m_bOmitCaption)
    {	// Try no captions, but with tick marks
	if (m_rectGraph.Height() < 100) m_bOmitTicks = TRUE;
    }

    // Try no graph
    if (m_rectGraph.Width() < 40 || m_rectGraph.Height() < 40) m_bOmitGraph = TRUE;

    m_sizeGraph.cx = m_rectGraph.Width();
    m_sizeGraph.cy = m_rectGraph.Height();

    int nExtraWidth = 9 * m_nXChar;
    int nExtraHeight = 3 * m_nYChar; 
    int nExtraRight = (3 * m_nXChar) / 2;
    m_ptOriginGraph.x = m_rectGraph.left + (m_nXChar * 13 / 2);
    m_ptOriginGraph.y = m_rectGraph.bottom - m_nYChar * 2;

    if (m_bOmitCaption || m_bOmitTicks)
    {	nExtraWidth = 16; 
    nExtraRight = nExtraWidth / 2;
    m_ptOriginGraph.x = m_rectGraph.left + 8; 
    nExtraHeight = 2 * m_nYChar; 
    m_ptOriginGraph.y = m_rectGraph.bottom - m_nYChar;
    }

    // Determine pixels covered by each horizontal Y
    int nGraphWidth = m_sizeGraph.cx - nExtraWidth;
    m_dPixelsPerX = (double) nGraphWidth / m_nPeriods;

    // Adjust periods plotted per pixel and horizonal ticks so all data can be plotted

    // Divide graph width into 5 character blocks
    // 

    m_nHorzTicks = edt_min(m_nPeriods, (nGraphWidth / (m_nXChar  * nMaxCharsX)));
    if (m_nPeriods % m_nHorzTicks)
    {
	int n = m_nPeriods / m_nHorzTicks;
	n++;
	m_nHorzTicks = m_nPeriods / n;
    }

    //m_nHorzTicks = m_nPeriods; 
    m_nPeriodsPerX = 1;
    if (m_dPixelsPerX < 0.0)
    {	while (m_dPixelsPerX < 0.0)
    {	m_nPeriodsPerX *= 2;
    m_nHorzTicks /= 2;
    m_dPixelsPerX = (double) nGraphWidth / m_nHorzTicks;
    }
    m_dPixelsPerXTick = (double) nGraphWidth / m_nHorzTicks;

    // Recycle through histogram and redetermine the maximum value
    m_lMaxValue = 0;
    for (int nColor = 0; nColor < m_nColors;nColor++)
    {
	for (int idx = 0; idx < m_nPeriods; idx+=m_nPeriodsPerX)
	{	int lValue = 0;
	for (int idy = idx; idy < m_nPeriodsPerX; idy++)
	{	
	    lValue += m_pData[nColor][idy];
	}
	m_lMaxValue = edt_max(m_lMaxValue, lValue);
	}
    }
    }
    else
    {	
	m_dPixelsPerXTick = (double) nGraphWidth / m_nHorzTicks;
    }

    m_dPixelsPerPeriod = (double) nGraphWidth / m_nPeriods;

    // Determine value covered by each vertical X
    m_dPixelsPerYTick = (double) ((m_sizeGraph.cy - nExtraHeight) / m_nVertTicks);
    m_dValuePerYTick = GetTickValue();
    m_dPixelsPerY = m_dPixelsPerYTick / m_dValuePerYTick;

    if (m_nHorzTicks < 1)
	m_nHorzTicks = 1;

    m_dValuePerXTick = (double) m_nPeriods / (double) m_nHorzTicks;

    // Center graph in any extra horizontal and vertical space that remains
    int nExtraSpace = edt_max(0, m_rectGraph.right - (m_ptOriginGraph.x +
	(int) ((m_nPeriods / m_nPeriodsPerX) * m_dPixelsPerXTick) + nExtraRight));
    m_ptOriginGraph.x += nExtraSpace / 2; 

    // Determine x and y maximum in pixel coordinates
    m_nXMax = m_ptOriginGraph.x + (int) (nGraphWidth);
    m_nYMax = m_ptOriginGraph.y - (int) (m_nVertTicks * m_dPixelsPerYTick); 

    return TRUE;
}

void CEdtPlotWindow::PlotAxes(CPaintDC* pDC)
{	
    int nHeight = (int) (m_dPixelsPerYTick * m_nVertTicks);
    int nWidth  = (int) (m_dPixelsPerXTick * m_nHorzTicks);
    int i;

    // Draw Axes
    pDC->MoveTo(m_ptOriginGraph);
    pDC->LineTo(m_ptOriginGraph.x, m_ptOriginGraph.y - nHeight);
    pDC->MoveTo(m_ptOriginGraph);
    pDC->LineTo(m_ptOriginGraph.x + nWidth, m_ptOriginGraph.y);

    CRect graphregion(m_ptOriginGraph, CPoint(m_nXMax, m_nYMax));
    pDC->Rectangle(&graphregion);

    // Draw Vertical Ticks (only if we are printing text)
    if (!m_bOmitTicks)
    {	
	int nTickSize = 1;

	int nVPos = m_ptOriginGraph.y;
	int xLeft  = m_ptOriginGraph.x - nTickSize;
	int xRight = m_ptOriginGraph.x + nTickSize + 1;

	for (i = 1; i <= m_nVertTicks; i++)
	{
	    nVPos = m_ptOriginGraph.y - (int) (i * m_dPixelsPerYTick);
	    pDC->MoveTo(xLeft, nVPos);
	    pDC->LineTo(xRight, nVPos);
	}

	// Draw Horizontal hash marks
	CPen penThin(PS_DOT, 1, (COLORREF) RGB(0,0,0));
	CPen* ppenOld = (CPen*) pDC->SelectObject(&penThin);

	nVPos = m_ptOriginGraph.y;
	xLeft  = m_ptOriginGraph.x + 1;
	xRight = m_ptOriginGraph.x + 1 + (int) (m_nHorzTicks * m_dPixelsPerXTick);
	for (i = 1; i <= m_nVertTicks; i++)
	{	
	    nVPos = m_ptOriginGraph.y - (int) (i * m_dPixelsPerYTick);
	    pDC->MoveTo(xLeft, nVPos);
	    pDC->LineTo(xRight, nVPos);
	}

	// reset dc object
	pDC->SelectObject(ppenOld);

    }

    // Draw Horizontal Ticks (if there is enough resolution and we are printing ticks)
    int nTickIncrement = 1;
    if (m_nHorzTicks > 16)
    {	nTickIncrement = 5;
    }
    if (m_nHorzTicks > 1 && m_dPixelsPerXTick > 1 && !m_bOmitTicks)
    {
	int nTickSize = 1;

	int nHPos = m_ptOriginGraph.x + 1;
	int yTop = m_ptOriginGraph.y - nTickSize;
	int yBottom = m_ptOriginGraph.y + nTickSize + 1;

	for (int i = nTickIncrement; i <= m_nHorzTicks; i += nTickIncrement)
	{ 
	    nHPos = m_ptOriginGraph.x + 1 + (int) (i * m_dPixelsPerXTick);
	    pDC->MoveTo(nHPos, yTop);
	    pDC->LineTo(nHPos, yBottom);
	}
    }
}

void CEdtPlotWindow::PlotCaptions(CPaintDC* pDC)
{
    if (m_bOmitCaption) return;
    int i;

    char buf[80];
    int n;
    double dValue = m_dValuePerYTick;

    int nTickSize = 2;

    pDC->SetTextAlign(TA_RIGHT);
    pDC->SetTextColor(RGB(0,0,0));
    pDC->SetBkColor(RGB(255,255,255)); 

    char* szFmt;

    double dNewValue, dFrac;
    dFrac = modf(dValue, &dNewValue); 	
    if (dFrac != 0)
    {
	dFrac = modf(dFrac *= 10, &dNewValue);
	if (dFrac == 0)
	{	szFmt = "%0.1f";
	}
	else
	{  
	    dFrac = modf(dFrac *= 10, &dNewValue);
	    if (dFrac == 0) 
	    {	szFmt = "%0.2f";
	    } 
	    else
	    {	szFmt = "%0.3f";
	    }
	}
    }
    else
    {	szFmt = "%0.0f";
    }

    for (i = 0; i < m_nVertTicks; i++)
    {
	if (dValue <  1000 || (dValue == 1000 && i == m_nVertTicks - 1))
	    n = sprintf(buf, szFmt, dValue); 
	else if (dValue <  10000)
	    n = sprintf(buf, "%0.2fK", dValue / 1000);
	else if (dValue <  100000)
	    n = sprintf(buf, "%0.1fK", dValue / 1000);
	else if (dValue <  1000000)
	    n = sprintf(buf, "%0.0fK", dValue / 1000);
	else if (dValue <  10000000)
	    n = sprintf(buf, "%0.2fM", dValue / 1000000);
	else if (dValue <  100000000)
	    n = sprintf(buf, "%0.1fM", dValue / 1000000);				
	else
	    n = sprintf(buf, "%0.0fM", dValue / 1000000);	

	int nPos = m_ptOriginGraph.y - (int) ((i + 1) * m_dPixelsPerYTick);
	pDC->TextOut(m_ptOriginGraph.x - 2 * nTickSize, nPos - m_nYChar / 2, buf, n);

	dValue += m_dValuePerYTick;
    }

    dValue = m_dValuePerXTick;

    for (i = 0; i < m_nHorzTicks; i++)
    {
	if (dValue <  1000 || (dValue == 1000 && i == m_nHorzTicks - 1))
	    n = sprintf(buf, "%3.0f", dValue); 
	else if (dValue <  10000)
	    n = sprintf(buf, "%0.2fK", dValue / 1000);
	else if (dValue <  100000)
	    n = sprintf(buf, "%0.1fK", dValue / 1000);
	else if (dValue <  1000000)
	    n = sprintf(buf, "%0.0fK", dValue / 1000);
	else if (dValue <  10000000)
	    n = sprintf(buf, "%0.2fM", dValue / 1000000);
	else if (dValue <  100000000)
	    n = sprintf(buf, "%0.1fM", dValue / 1000000);				
	else
	    n = sprintf(buf, "%0.0fM", dValue / 1000000);	

	int nPos = m_ptOriginGraph.x + (int) ((i + 1) * m_dPixelsPerXTick);

	pDC->TextOut(nPos, 
	    m_ptOriginGraph.y + 6, buf, n);

	dValue += m_dValuePerXTick;
    }

    return;
} 

void CEdtPlotWindow::PlotGraph(CPaintDC* pDC)
{

    COLORREF pencolor;

    for (int nColor=0; nColor< m_nColors; nColor++)
    {
	switch (nColor)
	{
	case 0:
	    pencolor = RGB(255,0,0);
	    break;
	case 1:
	    pencolor = RGB(0,255,0);
	    break;
	case 2:
	    pencolor = RGB(0,0,255);
	    break;
	default:
	    pencolor = RGB(0,0,0);

	}

	if (m_nPeriods > 16)
	{
	    CPen penThin(PS_SOLID, 1, pencolor);
	    CPen* ppenOld = (CPen*) pDC->SelectObject(&penThin);

	    double dLastX = m_ptOriginGraph.x + 1;
	    int iLastX = (int)dLastX;
	    double dLastY = 0;
	    int iLastY = 0;
	    bool bFirstTime = TRUE;
	    for (int idx = 0; idx < m_nPeriods;)
	    {	int lValue = 0;
	    for (int idy = 0; idy < m_nPeriodsPerX; idy++, idx++)
	    {	
		lValue += m_pData[nColor][idx];
	    }
	    dLastY = lValue * m_dPixelsPerY;
	    iLastY = m_ptOriginGraph.y - (int(dLastY) + 2);
	    if (bFirstTime)
	    {	bFirstTime = FALSE;
	    pDC->MoveTo(iLastX, iLastY);
	    }
	    else
	    {	pDC->LineTo(iLastX, iLastY);
	    }
	    dLastX += m_dPixelsPerX;
	    iLastX = (int)dLastX;		
	    }

	    // reset dc object
	    pDC->SelectObject(ppenOld);
	}
	else
	{
	    CPen penThin(PS_SOLID, 1, (COLORREF) RGB(0,0,0));
	    CBrush brushFill((COLORREF) pencolor);
	    CPen* ppenOld = (CPen*) pDC->SelectObject(&penThin);
	    CBrush* pbrushOld = (CBrush*) pDC->SelectObject(&brushFill);

	    int iLastX = m_ptOriginGraph.x;
	    int iLastY = m_ptOriginGraph.y;
	    for (int i = 0; i < m_nPeriods; i++)
	    {	double dNextX = m_ptOriginGraph.x + 1 + (int) ((i+1) * m_dPixelsPerPeriod);
	    int iNextX = (int)dNextX;
	    double dNextY = m_pData[nColor][i] * m_dPixelsPerY;
	    int iNextY = m_ptOriginGraph.y - int(dNextY);
	    pDC->Rectangle(iLastX, iLastY, iNextX, iNextY);
	    iLastX = (int)dNextX;
	    }

	    // reset dc object
	    pDC->SelectObject(pbrushOld);
	    pDC->SelectObject(ppenOld);
	}
    }
}

void CEdtPlotWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    CWnd * Parent = GetParent();

    m_nLastLineX = -1;

    if (Parent)
    {
	CRect graphregion(m_ptOriginGraph, CPoint(m_nXMax, m_nYMax));
	graphregion.NormalizeRect();

	CClientDC dc(this);
	dc.DPtoLP(&point);

	if (graphregion.PtInRect(point))
	{
	    // convert x value to graph value 

	    int DataX = (int) ((point.x - m_ptOriginGraph.x)/ m_dPixelsPerX);
	    Parent->SendMessage(WM_PLOTWINDOW_BUTTON, 
		nFlags, DataX);

	    // Draw an inverted line

	    dc.SetROP2(R2_NOT);
	    int nHeight = (int) (m_dPixelsPerYTick * m_nVertTicks);
	    dc.MoveTo(point.x, m_ptOriginGraph.y);
	    dc.LineTo(point.x, m_ptOriginGraph.y - nHeight);

	    m_nLastLineX = point.x;

	}


    }

    CWnd::OnLButtonDown(nFlags, point);
}

int CEdtPlotWindow::GetNDigits(int n)
{
    int nCount = 1;

    while (n)
    {
	n /= 10;
	nCount++;
    }

    return nCount;

}

void CEdtPlotWindow::OnLButtonUp(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default


    if (m_nLastLineX != -1)
    {
	CClientDC dc(this);
	dc.DPtoLP(&point);

	// Draw an inverted line

	dc.SetROP2(R2_NOT);
	int nHeight = (int) (m_dPixelsPerYTick * m_nVertTicks);
	dc.MoveTo(m_nLastLineX, m_ptOriginGraph.y);
	dc.LineTo(m_nLastLineX, m_ptOriginGraph.y - nHeight);

	m_nLastLineX = -1;


    }

    CWnd::OnLButtonUp(nFlags, point);
}

void CEdtPlotWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    if (m_nLastLineX != -1)
    {

	CClientDC dc(this);
	dc.DPtoLP(&point);

	// Draw an inverted line

	dc.SetROP2(R2_NOT);
	int nHeight = (int) (m_dPixelsPerYTick * m_nVertTicks);
	dc.MoveTo(m_nLastLineX, m_ptOriginGraph.y);
	dc.LineTo(m_nLastLineX, m_ptOriginGraph.y - nHeight);

	CWnd * Parent = GetParent();

	m_nLastLineX = -1;

	if (Parent)
	{
	    CRect graphregion(m_ptOriginGraph, CPoint(m_nXMax, m_nYMax));
	    graphregion.NormalizeRect();

	    CClientDC dc(this);
	    dc.DPtoLP(&point);

	    if (graphregion.PtInRect(point))
	    {
		// convert x value to graph value 

		int DataX = (int) ((point.x - m_ptOriginGraph.x)/ m_dPixelsPerX);
		Parent->SendMessage(WM_PLOTWINDOW_BUTTON, 
		    nFlags, DataX);

		// Draw an inverted line

		dc.SetROP2(R2_NOT);
		int nHeight = (int) (m_dPixelsPerYTick * m_nVertTicks);
		dc.MoveTo(point.x, m_ptOriginGraph.y);
		dc.LineTo(point.x, m_ptOriginGraph.y - nHeight);

		m_nLastLineX = point.x;

	    }


	}
    }

    CWnd::OnMouseMove(nFlags, point);
}
