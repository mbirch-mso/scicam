#if !defined(AFX_LEDBUTTON_H__78CDFF49_2C1A_11D4_B86B_003048100300__INCLUDED_)
#define AFX_LEDBUTTON_H__78CDFF49_2C1A_11D4_B86B_003048100300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LedButton.h : header file
//

#define LED_MAX_COLORS 8

/////////////////////////////////////////////////////////////////////////////
// CLedButton window

class DLLEXPORT CLedButton : public CButton
{
// Construction
public:
	CLedButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLedButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF m_Colors[LED_MAX_COLORS];
	int m_nSelectedColor;
	BOOL m_bLit;

	virtual ~CLedButton();

	void SetColor(COLORREF color, int index)
	{
		m_Colors[index] = color;
	}

	COLORREF &GetColor(int index)
	{
		return m_Colors[index];
	}
	void SetColor(COLORREF color)
	{
		m_Colors[m_nSelectedColor] = color;
	}

	COLORREF &GetColor()
	{
		return m_Colors[m_nSelectedColor];
	}

	int GetSelection()
	{
		return m_nSelectedColor;
	}

	void SetSelection(int nIndex)
	{
		if (nIndex == -1)
			SetLit(FALSE);
		else
		{
			ASSERT(nIndex >= 0 && nIndex < LED_MAX_COLORS);
			SetLit(TRUE);
			m_nSelectedColor = nIndex;
		}
	}

	BOOL GetLit()
	{
		return m_bLit;
	}

	void SetLit(BOOL bLit)
	{
		m_bLit = bLit;
	}


	// Generated message map functions
protected:
	//{{AFX_MSG(CLedButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDBUTTON_H__78CDFF49_2C1A_11D4_B86B_003048100300__INCLUDED_)
