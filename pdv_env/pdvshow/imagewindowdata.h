// ImageWindowData.h: interface for the ImageWindowData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(EDT_IMAGEWINDOWDATA_H)
#define EDT_IMAGEWINDOWDATA_H


#include "ZoomData.h"

#include "WindowPaint.h"


#define NDELAYS 20

typedef void (*redraw_cb_type)(void *pobj, EdtImage *pImage);

class ImageWindowData  
{

public:

    byte m_pRgbLuts[3][256];


    ImageWindowData();
    virtual ~ImageWindowData();

    WindowPaint *m_pPainter;

    EdtImage *	m_pImage;

    EdtImage *	m_pActiveImage; // Subimage


    ZoomData m_Zoom;

    EdtImage * m_pDisplayImage; // Backing image for display - in window
    // system format 

    EdtScaledLut *m_pDisplayScale; // 

    bool				m_bOwnDisplayScale;
    bool				m_bOwnDisplayImage;

    bool		m_bAdjustScaleForZoom; //

    EdtWindow	m_hWnd;
    EdtWindow   m_wndStatus;     // Window to which status messages are sent

    EdtBox m_SubRegion;
    EdtBox m_RawSubRegion;

    EdtPoint	m_Point1;
    EdtPoint	m_Point2;

    EdtPoint	m_RawPoint1;
    EdtPoint	m_RawPoint2;

    EdtPoint   m_Corner2;
    EdtPoint   m_Corner1;

    EdtPoint   m_RawCorner2;
    EdtPoint   m_RawCorner1;

    bool	m_bRectTracking;
    bool		m_bLineTracking;

    bool		m_bLineActive;
    bool		m_bRectActive;
    bool		m_bScaleLut;

    int			m_nFrameNumber;
    int			m_nSleepValue;

    int			m_nScrollWidth;
    int			m_nScrollHeight;

    int			m_nHScrollPos;
    int			m_nVScrollPos;

    EdtImage * m_pLastDisplayed;

    bool        m_bNeedsRefresh;

    bool	m_bDoDirect;



public:

    void SetZoom(const int nCornerX, 
	const int nCornerY,
	const double dScaleX,
	const double dScaleY);

    void ConvertToImageCoordinates(CPoint &point);
    void SetScale(const double ScaleX, const double ScaleY = 0.0);
    int GetHeight();
    int GetWidth();

    void SetScrollbarSizes(int width, int height)
    {
	//		if (m_pPainter)
	//			m_pPainter->SetScrollbarSizes(width, height);

	m_nScrollWidth = width;
	m_nScrollHeight = height;

    }


    bool m_bDragging;


    bool GetEndPoints(EdtPoint &pt1, EdtPoint &pt2);

    void SetEndPoints(const EdtPoint pt1, const EdtPoint pt2);

    bool GetRegion(EdtBox &region);

    void SetRegion(const EdtBox region);

    void SetWindow(const EdtWindow newwind)
    {
	m_hWnd = newwind;
    }

    EdtWindow GetWindow () const
    {
	return m_hWnd;
    }

    EdtImage *GetImage();


    bool IsRectTracking();
    bool IsLineTracking();

    void SetRectTracking(bool bState);
    void SetLineTracking(bool bState);

    bool IsRectActive();
    bool IsLineActive();

    void SetRectActive(bool bState);
    void SetLineActive(bool bState);

    bool PtInImage(const EdtPoint pt);

    EdtImage * GetBaseImage();
    EdtImage * GetSubImage();

    void SetImageHandle(EdtImage *pImage, 
	const char * ImageName,
	bool bReset = FALSE);

    void GetZoomValues(int &nSourceX, int &nSourceY, double &dZoomX, double &dZoomy);

    ZoomData &GetZoom()
    {
	return m_Zoom;
    }


    int m_nPumpStarted;
    int m_nPumpFinished;

    double m_Delays[NDELAYS];
    int m_nPumpPtr;

    double GetMeanDelay()
    {
	int i;
	double result = 0.0;
	for (i=0;i<NDELAYS;i++)
	    result += m_Delays[i];

	result /= (double) NDELAYS;

	return result;
    }

    double GetMaxDelay()

    {
	int i;
	double result = 0.0;
	for (i=0;i<NDELAYS;i++)
	    if (m_Delays[i] > result)
		result = m_Delays[i];

	return result;

    }

    void AddDelayValue(double delay)

    {
	m_Delays[m_nPumpPtr] = delay;
	m_nPumpPtr = (m_nPumpPtr + 1) % NDELAYS;
    }

    void SetPainter(WindowPaint *pPainter)
    {
	m_pPainter = pPainter;
    }


    EdtImage *GetDisplayImage()
    {
	return m_pDisplayImage;
    }


    void SetDisplayImage(EdtImage *pImage)

    {
	if (m_pDisplayImage && m_bOwnDisplayImage)
	{
	    delete m_pDisplayImage;
	    m_pDisplayImage = NULL;
	}

	m_pDisplayImage = pImage;
	m_bOwnDisplayImage = FALSE;
    }


    virtual void SetDisplayScale(EdtScaledLut *pDisplayScale);

    EdtScaledLut *GetDisplayScale() const
    {
	return m_pDisplayScale;
    }


    void SetPaletteInfrared() 
    {
	if (m_pPainter)
	    m_pPainter->SetPaletteInfrared();
    }

    void SetPaletteLinear() 
    {
	if (m_pPainter)
	    m_pPainter->SetPaletteLinear();
    }

    virtual void Draw(EdtImage *pImage, EdtDC hDC, CRect *pRect, bool erase = false);

    void ResetImageHandle(EdtImage *pImage, const char * ImageName);

    void ConvertToImageCoordinates(EdtBox &box);
    void ConvertToDisplayCoordinates(EdtBox &box);
    void ConvertToDisplayCoordinates(EdtPoint &pt);
    void SetCorner(const int nX, const int nY);

    void SetRGBLuts(void **pLuts)
    {

	// FIX
	if (m_pDisplayScale)
	    m_pDisplayScale->SetLutElements(pLuts);

	SetNeedsRefresh(TRUE);

    }



    void SetupDisplay(EdtImage *pSrcImage,
	EdtImage *pDisplayImage = NULL,
	EdtScaledLut *pDisplayScale = NULL,
	bool bReset = FALSE);

    virtual void SetupDraw(
	EdtImage *pImage,
	EdtBox *pRect);

    int GetScrollWidth() const
    {
	return m_nScrollWidth;
    }

    int GetScrollHeight() const
    {
	return m_nScrollHeight;
    }



    int GetDisplayCornerY();
    int GetDisplayCornerX();
    int GetDisplayHeight();
    int GetDisplayWidth();

    int GetDisplayHeight(EdtImage *pImage);
    int GetDisplayWidth(EdtImage *pImage);

    void SetLastDisplayed(EdtImage *pImage)
    {
	m_pLastDisplayed = pImage;
    }

    EdtImage *GetLastDisplayed()
    {
	return m_pLastDisplayed;
    }

    bool NeedsRefresh(EdtImage *pSrc);

    void SetNeedsRefresh(bool state)
    {
	m_bNeedsRefresh= state;
    }

    void SetRedrawCB(redraw_cb_type cb, void *target)
    {
	redraw_cb = cb;
	redraw_obj = target;
    }

    redraw_cb_type redraw_cb;
    void *redraw_obj;

};

#endif // !defined(AFX_IMAGEWINDOWDATA_H__23009331_27F7_11D3_8B96_00104B357776__INCLUDED_)
