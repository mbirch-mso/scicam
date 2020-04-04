// LedButton.cpp : implementation file
//

#include "stdafx.h"
#include "edtimage.h"
#include "LedButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLedButton

CLedButton::CLedButton()
{
	m_bLit = TRUE;
	int i = 0;

	int red,green,blue;

	for (red = 0; red < 256; red += 255)
		for (green = 0; green < 256; green += 255)
			for (blue = 0; blue < 256; blue += 255)
			{
				m_Colors[i++] = RGB(red, green,blue);
			}

	m_nSelectedColor = 0;

}

CLedButton::~CLedButton()
{
}


BEGIN_MESSAGE_MAP(CLedButton, CButton)
	//{{AFX_MSG_MAP(CLedButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLedButton message handlers

void CLedButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	
	// Draw a circle
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	if (m_bLit)
	{

		CPen pen(PS_SOLID, 1, GetColor());
			
		CBrush brush(GetColor());
		CBrush *OldBrush;
		CPen *OldPen;

		OldPen = dc.SelectObject(&pen);
		OldBrush = dc.SelectObject(&brush);

		dc.Ellipse( &lpDrawItemStruct->rcItem);

		dc.SelectObject(OldPen);
		dc.SelectObject(OldBrush);
	}
	else
	{
		COLORREF bk = dc.GetBkColor();
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, bk);
	}

	dc.Detach();

}
