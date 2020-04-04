// KeyboardInput.cpp : implementation file
//

#include "stdafx.h"
#include "edtimage.h"
#include "KeyboardInput.h"
#include "dispatch/ErrorHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyboardInput

CKeyboardInput::CKeyboardInput()
{
}

CKeyboardInput::~CKeyboardInput()
{
}


BEGIN_MESSAGE_MAP(CKeyboardInput, CWnd)
	//{{AFX_MSG_MAP(CKeyboardInput)
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKeyboardInput message handlers

void CKeyboardInput::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

void CKeyboardInput::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);


}

void CKeyboardInput::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	CWnd *parent = GetParent();

	if (parent)
		parent->SendNotifyMessage(WM_KILLFOCUS,0,0);


	// TODO: Add your message handler code here
	
}

void CKeyboardInput::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd *parent = GetParent();
	
	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKeyboardInput::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd *parent = GetParent();
	
	TRACE("Keyboard KeyUp %c\n", nChar);

	if (parent)
		parent->SendNotifyMessage(WM_KEYUP, nChar, nRepCnt | nFlags << 16);
}
