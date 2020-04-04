/** ********************************
* 
* @file EdtQtImageItem.h
*
* Copyright (c) Engineering Design Team 2009
* All Rights Reserved
*/
#ifndef EDT_QT_IMAGEWINDOW_H
#define EDT_QT_IMAGEWINDOW_H

#include "edtinc.h"
#include "edtimage/EdtImage.h"
#include "edtimage/ZoomData.h"
#include "edtimage/EdtScaledLut.h"
#include "edtimage/EdtRect.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#include "EdtQtGraphicsItem.h"

class bounding_box {

public:
    int x1, y1, x2, y2;
    bool active;

    bounding_box() {x1 = y1 = x2 = y2 = 0; active = false;}

    void rect_from_box(EdtRect *bp)
    {
        // get upper corner
        if (x1 >= x2)
        {
            bp->x = x2;
            bp->width = x1 - x2 + 1;
        }
        else
        {
            bp->x = x1;
            bp->width = x2 - x1 + 1;

        }
        if (y1 >= y2)
        {
            bp->y = y2;
            bp->height = y1 - y2 + 1;
        }
        else
        {
            bp->y = y1;
            bp->height = y2 - y1 + 1;

        }
    }

    int width() const
    {
        return (abs(x1 - x2) + 1);
    }

    int height() const
    {
        return (abs(y1 - y2) + 1);
    }

};



class EdtQtImageItem : public EdtQtGraphicsItem
{

    Q_OBJECT


public:

    int last_display_type; /// Last image data type 


    EdtImage *pDataImage; /// Image to display
    EdtImage *pDisplayImage; /// 8 or 24 bit displayed image

    bool TransformActive;
    bool Mono2Color;

    mutex_t m_lock;
    bool do_locks;
    int  m_nlocks;

    QImage *q_image;
    QPixmap *q_pixmap;

    EdtLut *pLut;

    bool do_lut;

    double last_time;
    double sum;
    int n_updates;

    QImage paint_image;
    QVector<QRgb> colors;

  
    EdtImage *pActiveImage; // Image for processing - possibly sub with selection
    
    EdtImage *pSubImage;
    bool has_subimage;
    QRectF subimage_box;

private:
    bool paint_enabled ;

public:


    EdtQtImageItem(QGraphicsItem *w = NULL);
    virtual ~EdtQtImageItem();

    EdtImage * GetImage();
 
    EdtImage * GetRawImage();
 
    void InitVariables();

    void SetMono2Color(const bool state);

    bool GetMono2Color() const;

    void SetTransformActive();

    void ClearTransform(void);

    void UpdateDisplayImage(bool resetLut=false);
 
    void SetDataImage(EdtImage *img);

    EdtImage *GetDataImage();
    EdtImage *ActiveImage();

    bool IsTransformActive();
   
    void Lock() ;

    void Unlock() ;

    bool NeedsRedraw();
  
    void UpdateIfNeeded();
   
    double MeanDelta();

	void TransformSetup(const bool resetLut = false);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    QRectF boundingRect() const;

    const QVector<QRgb> & GetColorTable();

    void SetColorTable(const QVector<QRgb> & colortable);

    void SetRegion(QRectF box);

    void ClearRegion();

    void EnablePaint() { paint_enabled = true ; }
    void DisablePaint() { paint_enabled = false ; }

    EdtLut *lut();

   

    public slots:

         void setLut(EdtLut *l);
};


#endif
