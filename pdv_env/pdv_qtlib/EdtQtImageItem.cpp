/** ********************************
* 
* @file EdtQtImageItem.cpp
*
* Copyright (c) Engineering Design Team 2009
* All Rights Reserved
*/



#include "edtinc.h"

extern "C" {
#include "edt_types.h"
}

#include <iostream>

//#include "EdtQtImageScroller.h"

#include "EdtQtImageItem.h"

#include <QGraphicsScene>

#include "EdtQtImageScene.h"

#ifdef WIN32

#include <process.h>

#endif

#define ARROW_SIZE 8

/*************************************************************************************************
 * @fn	EdtQtImageItem::EdtQtImageItem(QGraphicsItem *w)
 *
 * @brief	Constructor.
 *
 * @param [in,out]	w	If non-null, the w.
 **************************************************************************************************/

EdtQtImageItem::EdtQtImageItem(QGraphicsItem *w) : EdtQtGraphicsItem(w)
{

    // initialize all variables

    InitVariables();

}

/*************************************************************************************************
 * @fn	void EdtQtImageItem::InitVariables()
 *
 * @brief	Initialises the variables.
 **************************************************************************************************/

void EdtQtImageItem::InitVariables()

{
    pDataImage = NULL;
    pDisplayImage = NULL;
    pLut = NULL;

    TransformActive = false;
    Mono2Color = false;

    last_display_type = 0;
    do_lut = false;

    do_locks = true;
    create_mutex(m_lock);
    m_nlocks = 0;

    q_image = NULL;
    q_pixmap = NULL;

    last_time = 0;
    sum = 0;
    n_updates = 0;

    int i;

    is_image = true;

    for (i=0;i<256;i++)
        colors.append(qRgb(i,i,i));

    //colors[0] = qRgb(0,0,255);
    //colors[255] = qRgb(255,0,0);
    // 
    
    setFlag(ItemIsSelectable, true);


    pActiveImage = NULL;
    pSubImage = NULL;

    has_subimage = false;
    paint_enabled = true;

}

/*************************************************************************************************
 * @fn	EdtQtImageItem::~EdtQtImageItem()
 *
 * @brief	Destructor.
 **************************************************************************************************/

EdtQtImageItem::~EdtQtImageItem()
{

}

/*************************************************************************************************
 * @fn	void EdtQtImageItem::Lock()
 *
 * @brief	Locks this object.
 **************************************************************************************************/

void EdtQtImageItem::Lock() 

{
    if (do_locks) {
        wait_mutex(m_lock);

        m_nlocks++;
    }
}

/*************************************************************************************************
 * @fn	void EdtQtImageItem::Unlock()
 *
 * @brief	Unlocks this object.
 **************************************************************************************************/

void EdtQtImageItem::Unlock() 

{
    if (m_nlocks > 0) {
        release_mutex(m_lock);

        m_nlocks--;
    }
}

/**********************************************************************************************//**
* @fn	EdtImage * EdtQtImageItem::GetImage()
*
* @brief	Gets the image.
*
* @return	null if it fails, else the image.
**************************************************************************************************/

EdtImage * EdtQtImageItem::GetImage()

{
    return pDataImage;
}

/**********************************************************************************************//**
* @fn	void EdtQtImageItem::SetMono2Color(const bool state)
*
* @brief	Sets a mono 2 color.
*
* @param	state	The state.
**************************************************************************************************/

void EdtQtImageItem::SetMono2Color(const bool state)
{
    Mono2Color = state;
}

/**********************************************************************************************//**
* @fn	bool EdtQtImageItem::GetMono2Color() const
*
* @brief	Gets the mono 2 color.
*
* @return	true if it succeeds, false if it fails.
**************************************************************************************************/

bool EdtQtImageItem::GetMono2Color() const
{
    return Mono2Color;
}

/**********************************************************************************************//**
* @fn	void EdtQtImageItem::SetTransformActive()
*
* @brief	Sets the transform active.
**************************************************************************************************/

void EdtQtImageItem::SetTransformActive()
{
    TransformActive = true;
    TransformSetup();
}

/**********************************************************************************************//**
* @fn	void EdtQtImageItem::ClearTransform(void)
*
* @brief	Clears the transform.
**************************************************************************************************/

void EdtQtImageItem::ClearTransform(void)
{
    TransformActive = false;
    TransformSetup();
}

/**********************************************************************************************//**
* @fn	void EdtQtImageItem::SetDataImage(EdtImage *img)
*
* @brief	Sets a data image.
*
* @param [in,out]	img	If non-null, the image.
**************************************************************************************************/

void EdtQtImageItem::SetDataImage(EdtImage *img)
{
    pDataImage = img;
    pActiveImage = img;

    if (has_subimage)
    {
        pSubImage->AttachToParent(pDataImage, 
            (int) subimage_box.x(), (int) subimage_box.y(), (int) subimage_box.width(), (int) subimage_box.height());
        pActiveImage = pSubImage;

    }
    needs_redraw = true;
}

/**********************************************************************************************//**
* @fn	EdtImage *EdtQtImageItem::GetDataImage()
*
* @brief	Gets the data image.
*
* @return	null if it fails, else the data image.
**************************************************************************************************/

EdtImage *EdtQtImageItem::GetDataImage()
{
    return pDataImage;
}

/**********************************************************************************************//**
* @fn	bool EdtQtImageItem::IsTransformActive()
*
* @brief	Queries if a transform is active.
*
* @return	true if a transform is active, false if not.
**************************************************************************************************/

bool EdtQtImageItem::IsTransformActive()
{
    return TransformActive;
}

/**********************************************************************************************//**
* @fn	bool EdtQtImageItem::NeedsRedraw()
*
* @brief	Gets the needs redraw.
*
* @return	true if it succeeds, false if it fails.
**************************************************************************************************/

bool EdtQtImageItem::NeedsRedraw()
{
    return needs_redraw;
}



/**********************************************************************************************//**
* @fn	void EdtQtImageItem::UpdateIfNeeded()
*
* @brief	Updates only if needed.
**************************************************************************************************/

void EdtQtImageItem::UpdateIfNeeded()
{

    if (needs_redraw)
    {
        UpdateDisplayImage();
        needs_redraw = false;
    }
}

/**********************************************************************************************//**
* @fn	double EdtQtImageItem::MeanDelta()
*
* @brief	Gets the mean delta - time between image updates.
*
* @return	mean
**************************************************************************************************/

double EdtQtImageItem::MeanDelta()
{
    double mean;

    if (n_updates)
    {
        mean = sum / (double) (n_updates);
    }
    else
        mean = 0;

    return mean;
}

QRectF EdtQtImageItem::boundingRect() const

{
    return QRectF(0,0,paint_image.width(), paint_image.height());
}

#include <QPainter>

void EdtQtImageItem::paint(QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget)

{

    if (paint_enabled)
    {
        double t = edt_timestamp();

        if (paint_image.width())
        {
            if (paint_image.format() == QImage::Format_RGB888)
            {
                QImage img = paint_image.rgbSwapped();

                painter->drawImage(QPoint(0,0),img);
            }
            else
            {
                painter->drawImage(0,0,paint_image);
            }

        }

        double diff = edt_timestamp() - t;
        sum += diff;

        n_updates ++;
    }
}

/**********************************************************************************************//**
* @fn	void EdtQtImageItem::TransformSetup()
*
* @brief	Transform setup.
*
**************************************************************************************************/

void EdtQtImageItem::TransformSetup(const bool resetLut)

{

    if (pDataImage && !(pDataImage->GetType() == EDT_TYPE_BYTE || 
        pDataImage->GetType() == TypeRGB || pDataImage->GetType() == TypeBGR))
        TransformActive = true;
    else
        TransformActive = false;

    if ((TransformActive) && pDataImage)
    {        
        // allocate image

        // Use input values if specified, otherwise use current width & height
        int the_width = pDataImage->GetWidth();
        int the_height = pDataImage->GetHeight();

        EdtDataType target_type;

        if (pDataImage->GetType() == TypeRGB)
            target_type = TypeRGB;
        else if (pDataImage->GetType() == TypeBGR)
            target_type = TypeBGR;
        else
            target_type = TypeBYTE;


        Lock();

        if (pDisplayImage && (pDisplayImage != pDataImage))
        {   
#if 0 // optimization attempt -- but it breaks when reconfiguring 8 to 10 bit so, skip it
            if (pDisplayImage->GetWidth() != the_width || pDisplayImage->GetHeight() != the_height ||
                  pDisplayImage->GetType() != last_display_type) 
#endif
            {
                pDisplayImage->Destroy();

                pDisplayImage->Create((Mono2Color || pDataImage->GetNColors() > 1)?target_type:TypeBYTE,
                    the_width, the_height);
            }

        }
        else
        {
            pDisplayImage = new EdtImage((Mono2Color || pDataImage->GetNColors() > 1)?target_type:TypeBYTE,
                the_width, the_height);
        }

        Unlock();

        if (TransformActive && resetLut)
        {
			if (!pLut)
				pLut = new EdtLut();
            pLut->Setup(pDataImage, pDisplayImage);
            do_lut = true;
        }
    }
    else
    {
        do_lut = false;
    }
}

/**********************************************************************************************//**
* @fn	void EdtQtImageItem::UpdateDisplayImage()
*
* @brief	Updates the display image.
**************************************************************************************************/

void EdtQtImageItem::UpdateDisplayImage(bool resetLut)

{
    TransformSetup(resetLut);

    if (do_lut)
    {
        Lock();
        pLut->Transform(pDataImage,pDisplayImage);
        Unlock();
    }
    else
        pDisplayImage = pDataImage;

    last_display_type = pDisplayImage->GetType();

    QImage img((uchar *) pDisplayImage->GetFirstPixel(), 
        pDisplayImage->GetWidth(), 
        pDisplayImage->GetHeight(), 
        (pDisplayImage->GetType() == EDT_TYPE_BYTE)? QImage::Format_Indexed8 : QImage::Format_RGB888);

    QPixmap pm;

    img.setColorTable(colors);

    paint_image = img;

    update();
}

const QVector<QRgb> & EdtQtImageItem::GetColorTable()

{
    return colors;
}

void EdtQtImageItem::SetColorTable(const QVector<QRgb> & colortable)

{
    colors = colortable;
}



EdtImage * EdtQtImageItem::ActiveImage()

{
    return pActiveImage;
}

void EdtQtImageItem::SetRegion(QRectF box)

{
    if (!pSubImage)
        pSubImage = new EdtImage;

    pSubImage->AttachToParent(pDataImage, (int) box.x(), (int) box.y(), (int) box.width(), (int) box.height());
    pActiveImage = pSubImage;
    has_subimage = true;

    subimage_box = box;
}

void EdtQtImageItem::ClearRegion()

{
    pActiveImage = pDataImage;
    has_subimage = false;
}

EdtLut * EdtQtImageItem::lut()

{
    return pLut;
}

void EdtQtImageItem::setLut(EdtLut *l)

{
    pLut = l;

    if (l)
        do_lut = true;

    UpdateDisplayImage();
}
