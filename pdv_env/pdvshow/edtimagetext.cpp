// EdtImageText.cpp : implementation file
//

#include "stdafx.h"

#include "EdtImageText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdtImageText

CEdtImageText::CEdtImageText()
{
	EnableAutomation();

	m_pImage = NULL;
	m_nCenterX = 0;
	m_nCenterY = 0;
	m_bDecimal = TRUE;

	m_borderWidth = 2;
}

CEdtImageText::~CEdtImageText()
{
}

void CEdtImageText::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CEdtImageText, CWnd)
	//{{AFX_MSG_MAP(CEdtImageText)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CEdtImageText, CWnd)
	//{{AFX_DISPATCH_MAP(CEdtImageText)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IEdtImageText to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7E2B5354-2BE6-11D2-8DF1-00A0C932D513}
static const IID IID_IEdtImageText =
{ 0x7e2b5354, 0x2be6, 0x11d2, { 0x8d, 0xf1, 0x0, 0xa0, 0xc9, 0x32, 0xd5, 0x13 } };

BEGIN_INTERFACE_MAP(CEdtImageText, CWnd)
	INTERFACE_PART(CEdtImageText, IID_IEdtImageText, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtImageText message handlers

void CEdtImageText::SetImageHandle(EdtImage * pImage)
{
	m_pImage = pImage;
}

void CEdtImageText::SetCenterPoint(CPoint point)
{
	m_nCenterX = point.x;
	m_nCenterY = point.y;
}

void CEdtImageText::SetDecimal(bool state)
{
	m_bDecimal = state;
}

void CEdtImageText::RefreshData()
{
	Invalidate();
	UpdateWindow();
}

void CEdtImageText::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcBounds;

	// TODO: Add your message handler code here
	
	GetClientRect(&rcBounds);
	int nMax, cMax, nChars, nSpaces;
	CString s, sSpaces;
	CString sPixelFormat, sRowIndexFormat;
	CSize pixelSize;
	
	
	dc.FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	if (m_pImage)
	{

		nMax = cMax = m_pImage->GetMaxValue();

		if (m_pImage->GetNColors()== 1)
		{
			if (m_pImage->GetWidth() > nMax)
				nMax = m_pImage->GetWidth();
			else if (m_pImage->GetHeight() > nMax)
				nMax = m_pImage->GetHeight();
		}
	}
	else
	{
		nMax = 1000;
	}

    if (m_bDecimal)
    {
        s.Format("%d", nMax);
        nChars = s.GetLength();
    }
    else
    {
        s.Format("%x", cMax);
        nChars = s.GetLength();
        if (nChars < 2) nChars = 2;
        s.Format("%d", nMax);
        nSpaces = s.GetLength() - nChars;
        sSpaces.Format("%*s", nSpaces, "");
    }

	if (!m_pImage ||(m_pImage->GetNColors() == 1))
	{
        if (m_bDecimal)
            sPixelFormat.Format("%%%dd ",nChars);
        else sPixelFormat.Format("%s%%0%dx ", sSpaces, nChars);

        sRowIndexFormat.Format("%%%dd ",nChars); // row index is always decimal
        s.Format(sRowIndexFormat,nMax);
	}
	else
	{
        if (m_bDecimal)
            sPixelFormat.Format("%%%dd,%%%dd,%%%dd ",nChars,nChars,nChars);
        else sPixelFormat.Format("%%0%dx,%%0%dx,%%0%dx ", nChars, nChars, nChars);

        sRowIndexFormat.Format("%%%dd,%%%dd,%%%dd ",nChars,nChars,nChars);
        s.Format(sRowIndexFormat,nMax,nMax,nMax);

	}
		
	pixelSize = dc.GetOutputTextExtent(s);

	pixelSize += CSize(m_borderWidth*2, m_borderWidth*2);

	if (pixelSize.cx < 1 || pixelSize.cy < 1)
		return;

	m_nColumns = (rcBounds.Width() / pixelSize.cx) - 1;
	m_nRows = (rcBounds.Height() / pixelSize.cy) - 1;

	if (m_nColumns < 1 || m_nRows < 1)
		return;
	
	// put in labels on left and top
	
	int nStartRow = m_nCenterY - (m_nRows / 2);
	int nStartCol = m_nCenterX - (m_nColumns / 2);

	int nEndRow = nStartRow + m_nRows;
	int nEndCol = nStartCol + m_nColumns;

	int nSrcRow = nStartRow;
	int nSrcCol = nStartCol;

	int row, col;

	for (row = 1;row <= m_nRows; row++, nSrcRow ++)
	{
		// write row index
		s.Format("%4d",nSrcRow);
		dc.ExtTextOut(m_borderWidth,row * pixelSize.cy,ETO_OPAQUE,NULL,s,NULL);
		dc.TextOut(m_borderWidth,row * pixelSize.cy,s);

	}

	for (col = 1;col <= m_nColumns; col++, nSrcCol ++)
	{
		// write row index
		s.Format("%4d",nSrcCol);
		dc.ExtTextOut(col * pixelSize.cx,m_borderWidth,ETO_OPAQUE,NULL,s,NULL);
		dc.TextOut(col * pixelSize.cx,m_borderWidth,s);

	}
	// Add delimiter lines
	dc.MoveTo(0,pixelSize.cy);
	dc.LineTo(rcBounds.Width(), pixelSize.cy);

	dc.MoveTo(pixelSize.cx, 0);
	dc.LineTo(pixelSize.cx, rcBounds.Height());

	CRect textrect = rcBounds;

	CPoint uleft = textrect.TopLeft();

	CPoint bright = textrect.BottomRight();

	textrect.SetRect(uleft.x + pixelSize.cx + m_borderWidth,
		uleft.y + pixelSize.cy + m_borderWidth, bright.x, bright.y);


	// Get the text from the image

	nSrcRow = nStartRow;
	nSrcCol = nStartCol;

	if (m_pImage)
	{
		for (row = 1;row <= m_nRows;row++, nSrcRow++)
		{
			nSrcCol = nStartCol;
			if (nSrcRow >= 0 && nSrcRow < m_pImage->GetHeight())
			{
				for (col = 1;col <= m_nColumns;col++,nSrcCol++)
				{
					if (nSrcCol >= 0 && nSrcCol < m_pImage->GetWidth())
					{
						int Value = 0;
						m_pImage->GetPixel(nSrcCol, nSrcRow, Value);
									
						if (m_pImage->GetNColors() == 1)
							s.Format(sPixelFormat, Value);
						else
							s.Format(sPixelFormat, 
								GetRValue(Value), 
								GetGValue(Value), 
								GetBValue(Value));	
						
						if (nSrcRow == m_nCenterY && nSrcCol == m_nCenterX)
						{
							COLORREF oldcolor;
							oldcolor = dc.SetTextColor(RGB(255,0,0));
							dc.TextOut(col * pixelSize.cx + m_borderWidth/2,row * pixelSize.cy + m_borderWidth/2,s);
							dc.SetTextColor(oldcolor);
						}
						else
							dc.TextOut(col * pixelSize.cx + m_borderWidth/2,row * pixelSize.cy + m_borderWidth/2,s);
						
					}
				}
			}
		}
	}

	// Do not call CWnd::OnPaint() for painting messages
}
