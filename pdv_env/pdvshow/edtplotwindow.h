#if !defined(AFX_EDTPLOTWINDOW_H__8C1D618B_2AFB_11D2_97D9_00104B357776__INCLUDED_)
#define AFX_EDTPLOTWINDOW_H__8C1D618B_2AFB_11D2_97D9_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EdtPlotWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdtPlotWindow window


#define WM_PLOTWINDOW_BUTTON (WM_USER + 200)

class _declspec(dllexport) CEdtPlotWindow : public CWnd
{
// Construction
public:
	CEdtPlotWindow();

// Attributes
public:
	BOOL Create(int**, int, int, int nBands = 1);
	BOOL Reset(int**, int, int, int nBands = 1);

protected:

	CWnd*	m_pParent;			// Parent window
	CRect	m_rectWnd;			// Size of the window


	CRect   m_rLeftCaption;
	CRect	m_rBottomCaption;
	CSize	m_sizeGraph;		// Size of graph
	CRect	m_rectGraph;		// Plotting rectangle

	int**	m_pData;			// Data to plotted
	int	m_nPeriods;			// Number of plot periods
	int	m_lMaxValue;		// Maximum value to be plotted 
	BOOL	m_bClosing;			// Indicates that the dialog is closing down
	
	// Globals set by GetGraphMetrics
	int		m_nVertTicks;		// Number of vertical ticks desired
	int		m_nHorzTicks;		// Number of horizonal ticks desired	  
	short	m_nXChar;			// Width of character based on font and display 
	short	m_nYChar;			// Height of character based on font and display 
	float	m_fXAspect;			// x Aspect of display
	float	m_fYAspect;			// y Aspect of display	
	CPoint	m_ptOriginGraph;	// Origin of graph
	double	m_dPixelsPerX;		// Pixels per horizontal X
	double	m_dPixelsPerXTick;	// Pixels per horizontal graph tick
	int		m_nPeriodsPerX;		// Plot periods per horizontal X
	double	m_dPixelsPerY;		// Pixels per vertical Y
	double	m_dPixelsPerYTick;	// Pixels per vertical graph tick
	double	m_dValuePerYTick;	// Numeric range covered by each vertical tick 
	double	m_dValuePerXTick;	// Numeric range covered by each horizontal tick 
	int		m_nXMax;			// Maximum x pixel value
	int		m_nYMax; 			// Maximum y pixel value
	BOOL	m_bOmitCaption;		// Window is too small to display y axis captions 
	BOOL	m_bOmitTicks;		// Window is too small to display tick marks 
	BOOL	m_bOmitGraph;		// Window is too small to display graph

	// Printer sizes of characters, client area, and other print support variables
	short	m_nPXChar; 
	short	m_nPYChar; 
	short	m_nPXClient;
	short	m_nPYClient;
	short	m_nPXChars;			// Characters per line
	short	m_nPYLines;			// Lines per page 
	short	m_nLeftMargin;		// Width of left margin in device units
	short	m_nRightMargin;		// Width of right margin in device units
	short	m_nTopMargin;		// Height of top margin in device units 
	short	m_nBottomMargin;	// Height of bottom margin in device units 
	int		m_pSaved;			// Saves the printer device context for later restore
	CFont*	m_pFont;			// Pointer to print font
	BOOL	m_bPrinting;		// Printing flag 
	BOOL	m_bAborted;			// Printing aborted flag

     
// Implementation
protected:
	int m_nColors;
	void PlotAxes(CPaintDC*);
	void PlotCaptions(CPaintDC* pDC);
	void PlotGraph(CPaintDC*);
	double GetTickValue();
	void GetDisplayRect(CPaintDC*, CRect& rc);
	BOOL GetGraphMetrics(CPaintDC*);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtPlotWindow)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nLastLineX;
	int GetNDigits(int n);
	double m_dPixelsPerPeriod;
	virtual ~CEdtPlotWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEdtPlotWindow)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CEdtPlotWindow)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTPLOTWINDOW_H__8C1D618B_2AFB_11D2_97D9_00104B357776__INCLUDED_)
