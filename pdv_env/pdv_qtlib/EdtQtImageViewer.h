#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>

#include "EdtQtGraphicsItem.h"
#include "EdtQtImageItem.h"
#include "EdtQtImageView.h"
#include "EdtQtImageScene.h"

class EdtQtImageViewer
{

protected:

    EdtQtImageScene *scene;
    EdtQtImageView *view;
    
    double base_zoom;
    bool auto_scale;

     QList<EdtQtImageItem *> image_items;

public:


    EdtQtImageViewer(void);
    virtual ~EdtQtImageViewer(void);

    virtual void Setup(EdtQtImageView *v);

    EdtQtImageItem *ImageWindow(const int index = 0);

    virtual EdtQtImageItem *AddImageItem();

    EdtQtGraphicsItem *AddGraphicsItem(EdtQtGraphicsItem::Shape shape);

    void Refresh();

    bool UpdateNeeded();

    double GetMeanUpdate();

    void GetCursorValue(QPoint &pt, QString &tag);

    bool GetAutoScale();

    void SetAutoScale(const bool ascale);

    void SetZoom(const double scale);

    void Scale(const double scale);

    double GetZoom();

    void SetToImageSize(EdtImage *pImg);

};
