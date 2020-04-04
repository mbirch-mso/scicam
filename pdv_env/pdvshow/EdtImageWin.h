#if !defined(AFX_EDTIMAGEWIN_H__0956FE92_65FF_11D3_8BBF_00104B357776__INCLUDED_)
#define AFX_EDTIMAGEWIN_H__0956FE92_65FF_11D3_8BBF_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EdtImageWin.h : header file
//

#include "edtimage.h"

#include "ImageWindowData.h"

// User message to let the parent know it's been changed

#define WM_ZOOM_STATE_UPDATE (WM_USER + 300)

/////////////////////////////////////////////////////////////////////////////
// CEdtImageWin window

// "Zoom Modes" - what to do when a mouse button is clicked

#define ZOOM_MODE_NORMAL 0
#define ZOOM_MODE_INCREASE 1
#define ZOOM_MODE_DECREASE 2
#define ZOOM_MODE_CENTER 3
#define ZOOM_MODE_LINE_TRACK 4
#define ZOOM_MODE_RECT_TRACK 5

#define USE_SCROLL_CTL

#if _MSC_VER >= 1400 // VS2005 changed result of OnNCHitTest
#define HITTEST_RET LRESULT
#else
#define HITTEST_RET UINT
#endif



/////////////////////////////////////////////////////////////////////////////
// CGripper
// This class is for the resizing gripper control
class CGripper : public CScrollBar
{
    // Construction
public:
    CGripper(){};
    // Implementation
public:
    virtual ~CGripper(){};
    // Generated message map functions
protected:
    //{{AFX_MSG(CGripper)
    afx_msg HITTEST_RET OnNcHitTest(CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};




class DLLEXPORT CEdtImageWin : public CStatic
{
    // Construction
public:
    CEdtImageWin();

    // Attributes
public:


    double GetMeanDelay()
    {

        return m_wData.GetMeanDelay();
    }

    double GetMaxDelay()
    {
        return m_wData.GetMaxDelay();
    }

    ImageWindowData m_wData;

    CWnd *m_wndStatus;

    // Operations
public:
    void NotifyMouseAction(UINT message, UINT nFlags, CPoint point);
    bool m_bNotifyParentMouseMove;
    void SetNotifyParentMouseMove(bool bState);
    void UpdateDisplay();

    /////////////////////////////////////////////////////
    // Wrapper for generic ImageWindowData
    /////////////////////////////////////////////////////

    bool GetEndPoints(EdtPoint &pt1, EdtPoint &pt2)
    {
        return m_wData.GetEndPoints(pt1,pt2);
    }

    void SetEndPoints(const EdtPoint pt1, const EdtPoint pt2)
    {
        m_wData.SetEndPoints(pt1,pt2);
    }

    bool GetRegion(EdtBox &region)
    {
        return m_wData.GetRegion(region);
    }

    void SetRegion(const EdtBox region)
    {
        m_wData.SetRegion(region);
    }

    EdtImage *GetImage() 
    { return m_wData.GetImage();}

    bool IsRectTracking()
    { return (m_nMouseZoomMode == ZOOM_MODE_RECT_TRACK);}

    bool IsLineTracking()
    { return (m_nMouseZoomMode == ZOOM_MODE_LINE_TRACK);}

    bool IsDragging()
    { return m_wData.m_bDragging;}

    void SetDragging(bool bState)
    { m_wData.m_bDragging = bState;}


    void SetLineActive(bool bState)
    { 
        bool was = m_wData.m_bLineActive;
        m_wData.SetLineActive(bState);
        if (was)
            UpdateDisplay();
        UpdateDisplay();
    }

    bool IsLineActive()
    { return m_wData.m_bLineActive;}

    void SetRectActive(bool bState)
    { 
        bool was = m_wData.m_bRectActive;
        m_wData.SetRectActive(bState);
        if (was)
            UpdateDisplay();
    }

    bool IsRectActive()
    { return m_wData.m_bRectActive;}

    void SetRectTracking(bool bState)
    { 
        m_wData.SetRectTracking(bState);
        if (bState)
            SetMouseZoomMode(ZOOM_MODE_RECT_TRACK);
        else 
            SetMouseZoomMode(ZOOM_MODE_NORMAL);
    }

    void SetLineTracking(bool bState)
    { 
        m_wData.SetLineTracking(bState);
        if (bState)
            SetMouseZoomMode(ZOOM_MODE_LINE_TRACK);
        else 
            SetMouseZoomMode(ZOOM_MODE_NORMAL);
    }

    bool PtInImage(const EdtPoint pt)
    { return m_wData.PtInImage(pt);}

    EdtImage * GetBaseImage()
    { return m_wData.GetBaseImage();}

    EdtImage * GetSubImage()
    { return m_wData.GetSubImage();}

    void SetImageHandle(EdtImage *pImage, const char * ImageName,
        const bool bReset = FALSE);


    int GetDisplayWidth(EdtImage *pImage = NULL)
    {
        if (pImage)
            return m_wData.GetDisplayWidth(pImage);
        else
            return m_wData.GetDisplayWidth();
    }

    int GetDisplayHeight(EdtImage *pImage = NULL)
    {
        if (pImage)
            return m_wData.GetDisplayHeight(pImage);
        else
            return m_wData.GetDisplayHeight();
    }

    /////////////////////////////////////////////////////

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEdtImageWin)
    //}}AFX_VIRTUAL

    // Implementation
public:
    bool m_bResizeParent;
    void ResetImageHandle(EdtImage *pImage, const char *Name);


    void UpdateZoomStatus();
    void ChangeZoomUpdate(double delta, CPoint pt);
    void GetImageCenterPoint(CPoint &pt);
    int GetZoomMode();
    void ConvertToDisplayCoordinates(CRect &rect);
    void ConvertToDisplayCoordinates(CPoint &pt);
    void ResetZoom();
    double m_dNominalZoom;
    double m_dDeltaZoom;
    void ChangeZoom(double dChange, CPoint &center);

    int m_nMouseZoomMode;
    int GetMouseZoomMode();
    void SetMouseZoomMode(int nMode);
    void GotoScroll(int nX, int nY);
    bool m_bScrollBarsActive;
    void Update();


    CGripper m_wndGripper;

    void SizeToImage();


    //	bool m_bAutoScale;

    void SetZoom(const int nCornerX, 
        const int nCornerY,
        const double dScaleX,
        const double dScaleY)

    {
        m_wData.SetZoom(nCornerX, 
            nCornerY,
            dScaleX,
            dScaleY);
    }

    void GetZoomValues(int &nSourceX, int &nSourceY, double &dScaleX, double &dScaleY)
    {
        m_wData.GetZoomValues(nSourceX, 
            nSourceY,
            dScaleX,
            dScaleY);
    }

    void ConvertToImageCoordinates(CPoint &point)
    {
        m_wData.ConvertToImageCoordinates(point);
    }

    void SetScale(const double ScaleX, const double ScaleY = 0.0)
    {
        m_wData.SetScale(ScaleX, ScaleY);
    }


    int GetWidth()
    {
        return m_wData.GetWidth();
    }

    int GetHeight()
    {
        return m_wData.GetHeight();
    }


    void SetDisplayScale(EdtScaledLut *pDisplayScale)
    {
        m_wData.SetDisplayScale(pDisplayScale);
    }

    EdtScaledLut *GetDisplayScale() 
    {
        return m_wData.GetDisplayScale();
    }	


    void SetPaletteInfrared() 
    {
        m_wData.SetPaletteInfrared();
    }

    void SetPaletteLinear() 
    {
        m_wData.SetPaletteLinear();
    }

    void RedrawImage(CDC &dc, CRect updaterect, EdtImage *pImage);

    static void RedrawCallback(void *pthis, EdtImage *pImage);

    void AdjustScrollbars();
    int m_nScrollBorder;

    void AddScrollbars();

#ifdef USE_SCROLL_CTL
    CScrollBar m_hScroll;
    CScrollBar m_vScroll;
#endif

    int m_nMargin;
    void SizeFrame(bool bResizeParent);

    virtual ~CEdtImageWin();

    // Generated message map functions
protected:
    //{{AFX_MSG(CEdtImageWin)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void HScroll(UINT nSBCode, UINT nPos);
    afx_msg void VScroll(UINT nSBCode, UINT nPos);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTIMAGEWIN_H__0956FE92_65FF_11D3_8BBF_00104B357776__INCLUDED_)
