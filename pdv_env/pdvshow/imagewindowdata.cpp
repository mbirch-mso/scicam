// ImageWindowData.cpp: implementation of the ImageWindowData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "edtimage.h"
#include "edtdefines.h"
#include "imagewindowdata.h"
// #include "edtdisplayscale.h"

#include <stdio.h>

#define no_printf 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageWindowData::ImageWindowData()
{
    m_wndStatus = (EdtWindow) 0;
    m_pImage = NULL;
    m_pActiveImage = NULL;

    m_nSleepValue = 1;
    m_nFrameNumber = 0;

    m_bLineTracking = FALSE;
    m_bLineActive = FALSE;

    m_bRectTracking = FALSE;
    m_bRectActive = FALSE;

    memset(&m_SubRegion,0,sizeof(m_SubRegion));
    memset(&m_Point1,0,sizeof(m_Point1));
    memset(&m_Point2,0,sizeof(m_Point2));
    memset(&m_Corner1,0,sizeof(m_Corner1));
    memset(&m_Corner2,0,sizeof(m_Corner2));

    memset(&m_RawSubRegion,0,sizeof(m_RawSubRegion));
    memset(&m_RawPoint1,0,sizeof(m_RawPoint1));
    memset(&m_RawPoint2,0,sizeof(m_RawPoint2));
    memset(&m_RawCorner1,0,sizeof(m_RawCorner1));
    memset(&m_RawCorner2,0,sizeof(m_RawCorner2));

    m_bDragging = FALSE;

    m_hWnd = (EdtWindow) 0;

    m_nPumpFinished = 0;
    m_nPumpStarted = 0;

    m_nPumpPtr = 0;
    memset(m_Delays,0,sizeof(m_Delays));


    m_pPainter = NULL;

    m_nScrollWidth = 0;
    m_nScrollHeight = 0;

    m_pDisplayScale = NULL;
    m_bOwnDisplayScale = FALSE;

    m_pDisplayImage = NULL;
    m_bOwnDisplayImage = FALSE;

    m_pLastDisplayed = NULL;

#ifdef WIN32

    m_bAdjustScaleForZoom = FALSE;

#else

    m_bAdjustScaleForZoom = TRUE;

#endif

    m_bDoDirect = TRUE;

    redraw_cb = NULL;
    redraw_obj = NULL;

}

ImageWindowData::~ImageWindowData()
{
    if (m_pActiveImage)
    {
        delete m_pActiveImage;
        m_pActiveImage = NULL;
    }
    if (m_pDisplayImage)
    {
        if (m_bOwnDisplayImage)
            delete m_pDisplayImage;
        m_pDisplayImage = NULL;
    }

    if (m_pDisplayScale)
    {
        if (m_bOwnDisplayScale)
        {
            delete m_pDisplayScale;
            m_bOwnDisplayScale = FALSE;
        }
        m_pDisplayScale = NULL;
    }

}

void ImageWindowData::SetImageHandle(EdtImage * pImage, 
                                     const char * ImageName,
                                     bool bReset)

{
    if (pImage != m_pImage || bReset)
    {
        m_pImage = pImage;

        if (pImage)
        {

            // Set up the painter
            SetupDisplay(m_pImage,NULL,NULL,bReset);

            if (m_pActiveImage)
            {
                int width = m_SubRegion.right - m_SubRegion.left;
                int height = m_SubRegion.bottom - m_SubRegion.top;

                m_pActiveImage->AttachToParent(m_pImage,
                    m_SubRegion.left, m_SubRegion.top,
                    width, height);
            }
        }

        SetNeedsRefresh(TRUE);

    }
}



bool ImageWindowData::GetEndPoints(EdtPoint &pt1, EdtPoint &pt2)
{
    if (m_bLineActive)
    {
        pt1 = m_Point1;
        pt2 = m_Point2;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void ImageWindowData::SetEndPoints(const EdtPoint pt1, const EdtPoint pt2)
{
    m_Point1 = pt1;
    m_Point2 = pt2;
}

bool ImageWindowData::GetRegion(EdtBox &region)
{
    region = m_SubRegion;
    return TRUE;
}

void ImageWindowData::SetRegion(const EdtBox region)
{
    m_SubRegion = region;
}

EdtImage *ImageWindowData::GetImage()
{
    if (m_pActiveImage)
        return m_pActiveImage;
    else
        return m_pImage;
}

EdtImage * ImageWindowData::GetSubImage()
{
    return m_pActiveImage;

}

EdtImage * ImageWindowData::GetBaseImage()
{
    return m_pImage;
}


bool ImageWindowData::PtInImage(const EdtPoint pt)
{
    EdtImage *pImage = GetBaseImage();

    if (!pImage)
        return FALSE;

    if (pt.x < 0 || pt.x >= pImage->GetWidth() ||
        pt.y < 0 || pt.y >= pImage->GetHeight())
        return FALSE;

    return TRUE;
}

void ImageWindowData::SetLineTracking(bool bState)
{
    m_bLineTracking = bState;
    if (bState)
    {
        m_bLineActive = TRUE;
    }

}

void ImageWindowData::SetRectTracking(bool bState)
{
    m_bRectTracking = bState;
    if (bState)
    {
        SetRectActive(TRUE);
    }

}

bool ImageWindowData::IsLineTracking()
{
    return m_bLineTracking;
}

bool ImageWindowData::IsRectTracking()
{
    return m_bRectTracking;
}


void ImageWindowData::SetLineActive(bool bState)
{
    m_bLineActive = bState;
}

void ImageWindowData::SetRectActive(bool bState)
{
    m_bRectActive = bState;

    if (!bState)
    {
        if (m_pActiveImage)
        {
            delete m_pActiveImage;
            m_pActiveImage = NULL;
        }
    }
    else
    {
        int width = m_SubRegion.right - m_SubRegion.left;
        int height = m_SubRegion.bottom - m_SubRegion.top;
        if (width > 0 && height > 0)
        {
            if (!m_pActiveImage)
            {
                m_pActiveImage =  new EdtImage;
                m_pActiveImage->SetType(m_pImage->GetType());
            }

            m_pActiveImage->AttachToParent(m_pImage,
                m_SubRegion.left, m_SubRegion.top,
                width, height);

        }		
    }
}

bool ImageWindowData::IsLineActive()
{
    return m_bLineActive;
}

bool ImageWindowData::IsRectActive()
{
    return m_bRectActive;
}

int ImageWindowData::GetWidth()
{
    if (GetBaseImage())
        return (int) (GetBaseImage()->GetWidth() * m_Zoom.GetZoomX());
    else
        return 0;
}

int ImageWindowData::GetHeight()
{
    if (GetBaseImage())
        return (int) (GetBaseImage()->GetHeight() * m_Zoom.GetZoomX());
    else
        return 0;
}

void ImageWindowData::SetScale(const double ScaleX, const double ScaleY)
{

    if (ScaleX > 0.0)
    {

        m_Zoom.SetZoomXY(ScaleX, ScaleY);

        if (m_pDisplayScale && m_bAdjustScaleForZoom)
            m_pDisplayScale->SetZoom(&m_Zoom);

        SetNeedsRefresh(TRUE);

    }

}


void ImageWindowData::SetZoom(const int nCornerX, 
                              const int nCornerY,
                              const double ScaleX,
                              const double ScaleY)
{
    if (ScaleX > 0.0)
    {

        m_Zoom.SetZoom(nCornerX, nCornerY, ScaleX, ScaleY);

        //	if (m_pPainter)
        //		m_pPainter->m_Zoom.SetZoom(nCornerX, nCornerY,ScaleX, ScaleY);

        if (m_pDisplayScale && m_bAdjustScaleForZoom)
            m_pDisplayScale->SetZoom(&m_Zoom);

        SetNeedsRefresh(TRUE);

        no_printf("imagewindowdata: SetZoom(%d,%d,%f,%f);\n",
            nCornerX,nCornerY,ScaleX, ScaleY);

    }
}

void ImageWindowData::GetZoomValues(int &nSourceX, int &nSourceY, double &dZoomX, double &dZoomy)

{
    m_Zoom.GetZoomValues(nSourceX, nSourceY, dZoomX, dZoomy);
}

void ImageWindowData::SetCorner(const int nX, const int nY)
{

    m_Zoom.SetSourceXY(nX, nY);

    if (m_pDisplayScale && m_bAdjustScaleForZoom)
        m_pDisplayScale->SetZoom(&m_Zoom);

}

void ImageWindowData::ConvertToImageCoordinates(CPoint & point)
{

    if (m_Zoom.IsActive())
    {

        point.x = (int) (point.x / m_Zoom.GetZoomX());
        point.y = (int) (point.y / m_Zoom.GetZoomY());

        point.x += m_Zoom.GetSourceX();
        point.y += m_Zoom.GetSourceY();

    }

}

void ImageWindowData::ConvertToDisplayCoordinates(EdtPoint &point)
{

    if (m_Zoom.IsActive())
    {


        point.x -= m_Zoom.GetSourceX();
        point.y -= m_Zoom.GetSourceY();
        point.x = (int) (point.x * m_Zoom.GetZoomX());
        point.y = (int) (point.y * m_Zoom.GetZoomY());

    }
}

void ImageWindowData::ConvertToDisplayCoordinates(EdtBox &box)
{

    if (m_Zoom.IsActive())
    {
        box.left -= m_Zoom.GetSourceX();
        box.top -= m_Zoom.GetSourceY();
        box.right -= m_Zoom.GetSourceX();
        box.bottom -= m_Zoom.GetSourceY();

        box.left = (int) (box.left * m_Zoom.GetZoomX());
        box.top = (int) (box.top * m_Zoom.GetZoomY());
        box.right = (int) (box.right * m_Zoom.GetZoomX());
        box.bottom = (int) (box.bottom * m_Zoom.GetZoomY());



    }
}

void ImageWindowData::ConvertToImageCoordinates(EdtBox &box)
{

    if (m_Zoom.IsActive())
    {

        box.left = (int) (box.left / m_Zoom.GetZoomX());
        box.top = (int) (box.top / m_Zoom.GetZoomY());
        box.right = (int) (box.right / m_Zoom.GetZoomX());
        box.bottom = (int) (box.bottom / m_Zoom.GetZoomY());

        box.left += m_Zoom.GetSourceX();
        box.top += m_Zoom.GetSourceY();
        box.right += m_Zoom.GetSourceX();
        box.bottom += m_Zoom.GetSourceY();

    }
}

void ImageWindowData::ResetImageHandle(EdtImage *pImage, const char *ImageName)
{

    SetImageHandle(pImage, ImageName, TRUE);

}

void ImageWindowData::Draw(EdtImage *pImage, EdtDC hDC, EdtBox *pRect, bool erase)
{
    ASSERT(m_pPainter);

    //printf("painter is %x window is %x dc is %x\n", m_pPainter,
    //   hWindow, hDC);

    if (m_pPainter)
        m_pPainter->Draw(pImage, this, hDC, pRect, erase);

}


void ImageWindowData::SetDisplayScale(EdtScaledLut *pDisplayScale)

{
    if (pDisplayScale == m_pDisplayScale)
        return;

    if (m_pDisplayScale && m_bOwnDisplayScale)
    {
        delete m_pDisplayScale;
        m_pDisplayScale = NULL;
    }

    m_pDisplayScale = pDisplayScale;
    m_bOwnDisplayScale = FALSE;

    SetNeedsRefresh(TRUE);

}



//This display image stuff should be OS dependent


void ImageWindowData::SetupDisplay(EdtImage *pSrcImage,
                                   EdtImage *pDisplayImage,
                                   EdtScaledLut *pDisplayScale,
                                   bool bReset)

{

    // Decide if we can display directly

    //printf("SetupDisplay  \n");


    if (m_pPainter->IsDirectlyDisplayable(pSrcImage) && m_bDoDirect)
    {
        //printf("Directly displayable..\n");
    }
    else
    {
        if (!m_pDisplayImage)
        {
            if (pDisplayImage)
            {
                m_pDisplayImage = pDisplayImage;
                m_bOwnDisplayImage = FALSE;
            }
            else
            {
                printf("Getting Displayable Image\n");
                m_pDisplayImage = m_pPainter->GetDisplayableImage(this, pSrcImage);
            }

        }

        if (!m_pDisplayScale)
        {
            if (pDisplayScale)
            {
                m_pDisplayScale = pDisplayScale;
                m_bOwnDisplayScale = FALSE;
            }
            else
            {
                m_pDisplayScale = new EdtScaledLut;


                m_bOwnDisplayScale = TRUE;
            }

            m_pDisplayScale->Setup(pSrcImage, m_pDisplayImage);

            m_pDisplayScale->SetLinear(
                pSrcImage->GetMinValue(),
                pSrcImage->GetMaxValue(),
                -1);
        }
        else
            m_pDisplayScale->Setup(pSrcImage, m_pDisplayImage);

    }

}

// Display using the pointer to a displayable image

void ImageWindowData::SetupDraw(EdtImage *pImage,
                                EdtBox *pRect)

{

    EdtImage *pDisplayImage = pImage;

    no_printf("SetupDraw DisplayScale %p DisplayImage %p\n",
        m_pDisplayScale, m_pDisplayImage);

    if (m_pDisplayScale && m_pDisplayImage)
    {


        if (NeedsRefresh(pImage))
        {
            no_printf("Needs refresh adjust %d\n", m_bAdjustScaleForZoom);

            if (m_pDisplayImage->GetWidth() != pImage->GetWidth() ||
                m_pDisplayImage->GetHeight() != pImage->GetHeight())
                m_pDisplayImage->Clear();

            if (m_bAdjustScaleForZoom)
            {
                m_pDisplayScale->SetZoomHandle(&m_Zoom);

                m_pDisplayScale->Transform(pImage, m_pDisplayImage,
                    m_Zoom.GetSourceX(),
                    m_Zoom.GetSourceY(),
                    m_pDisplayImage->GetWidth(),
                    m_pDisplayImage->GetHeight(),
                    0,0);

            }
            else
            {
                m_pDisplayScale->Transform(pImage, m_pDisplayImage,
                    0,0,
                    m_pDisplayImage->GetWidth(),
                    m_pDisplayImage->GetHeight(),
                    m_pDisplayScale->GetZoomHandle()->GetSourceX(),
                    m_pDisplayScale->GetZoomHandle()->GetSourceY()
                    );

            }

            SetLastDisplayed(pImage);

            pImage->SetDataClean();
            SetNeedsRefresh(FALSE);

        }

        pDisplayImage = m_pDisplayImage;

    }

    m_pDisplayImage = pDisplayImage;


}


int ImageWindowData::GetDisplayWidth()
{
    return GetWidth();
}

int ImageWindowData::GetDisplayHeight()
{
    return GetHeight();
}

int ImageWindowData::GetDisplayWidth(EdtImage *pImage)
{
    if (m_Zoom.IsActive())
        return (int)( m_Zoom.GetZoomX() * (pImage->GetWidth() /* - m_Zoom.m_nSourceX */));
    else
        return pImage->GetWidth();
}

int ImageWindowData::GetDisplayHeight(EdtImage *pImage)
{
    if (m_Zoom.IsActive())
        return (int)( m_Zoom.GetZoomY() * (pImage->GetHeight() /* - m_Zoom.m_nSourceY */));
    else
        return pImage->GetHeight();
}

int ImageWindowData::GetDisplayCornerX()
{
    if (m_Zoom.IsActive())
        return m_Zoom.GetSourceX();
    else
        return 0;
}

int ImageWindowData::GetDisplayCornerY()
{
    if (m_Zoom.IsActive())
        return m_Zoom.GetSourceY();
    else
        return 0;
}


bool ImageWindowData::NeedsRefresh(EdtImage * pSrc)
{

    no_printf("Needs Refresh pSrc = %p this = %p last = %p datachanged = %d needs %d\n",
        pSrc,m_pDisplayImage,m_pLastDisplayed, pSrc->IsDataChanged(), m_bNeedsRefresh);

    // always true if flag set

    if (m_bNeedsRefresh)
        return TRUE;

    if (pSrc == m_pDisplayImage)
        return FALSE;

    if (pSrc != m_pLastDisplayed)
        return TRUE;

    if (pSrc->IsDataChanged())
        return TRUE;

    return FALSE;

}
