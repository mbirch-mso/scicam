#include "EdtQtImageViewer.h"
#include <iostream>


EdtQtImageViewer::EdtQtImageViewer(void)
{
    scene = new EdtQtImageScene;

    view = NULL;
    auto_scale = false;
    base_zoom = 1.0;

}

EdtQtImageViewer::~EdtQtImageViewer(void)
{
}

void EdtQtImageViewer::Setup(EdtQtImageView *v)

{
    scene=v->Scene();

    
    view = v;

    v->setMouseTracking(true);

}

EdtQtImageItem *EdtQtImageViewer::ImageWindow(const int index)

{
    QList<QGraphicsItem *> list = scene->items();

    //std::cout << "Size of List: " << list.size() << std::endl;
    if (list.size() > 0)
        return (EdtQtImageItem *) list.at(index);
    return NULL;
}

EdtQtImageItem *EdtQtImageViewer::AddImageItem()

{
    EdtQtImageItem *item = new EdtQtImageItem();

    scene->addItem(item);

    item->setAcceptHoverEvents(true);

    image_items.append(item);

    return item;

}

EdtQtGraphicsItem *EdtQtImageViewer::AddGraphicsItem(EdtQtGraphicsItem::Shape shape)
{
    EdtQtGraphicsItem *gItem = new EdtQtGraphicsItem();
    gItem->SetShape(shape);
    scene->addItem(gItem);


    return gItem;
}

void EdtQtImageViewer::Refresh()

{
    int i;

    for (i=0;i<image_items.size();i++)
    {
        image_items.at(i)->UpdateIfNeeded();
    }


}

bool EdtQtImageViewer::UpdateNeeded()

{
    int i;

    for (i=0;i<image_items.size();i++)
    {
        if (image_items.at(i)->NeedsRedraw())
            return true;
    }

    return false;
}

double EdtQtImageViewer::GetMeanUpdate()

{
    int imgIdx;

    QList<QGraphicsItem *> list = scene->items();
    imgIdx = (list.size() - 1);

    EdtQtImageItem *item = ImageWindow(imgIdx);

    if (item)
    {
        return item->MeanDelta();
    }

    return 0;

}

void EdtQtImageViewer::GetCursorValue(QPoint &q, QString &tag)

{
    int x,y, value;
    int imgIdx;

    QList<QGraphicsItem *> list = scene->items();
    imgIdx = (list.size() - 1);

    EdtQtImageItem *item = ImageWindow(imgIdx);

    if (item == NULL)
    {
        q.setX(0);
        q.setY(0);
        tag = "";
        return;
    }

    char buf[32];

    buf[0] = 0;

    EdtImage *image = item->GetDataImage();

    if (image)
    {
        x = scene->get_last_mouse_xpos();
        y = scene->get_last_mouse_ypos();

        q.setX(x);
        q.setY(y);


        int values[4];

        int nvalues = image->GetPixelV((int) q.x(), (int) q.y(), values);

        if (nvalues == 3)
            sprintf(buf, "(%d,%d,%d)", values[0], values[1], values[2]);
        else // nvalues == 1 or...?
            sprintf(buf, "%d", values[0]);

    }

    tag = buf;


}

bool EdtQtImageViewer::GetAutoScale()

{
    return auto_scale;
}

void EdtQtImageViewer::SetAutoScale(const bool ascale)

{
    auto_scale = ascale;
    // FIX - set new state on items
    QList<QGraphicsItem *> list = scene->items();
    int i;
    for (i=0;i<list.size();i++)
    {
    }

}

void EdtQtImageViewer::SetZoom(const double scale)

{
    base_zoom = scale;

    QTransform t;

    t.scale(base_zoom, base_zoom);

    view->setTransform(t);


}

double EdtQtImageViewer::GetZoom()

{
    return base_zoom;
}

void EdtQtImageViewer::Scale(const double scale)

{
    base_zoom *= scale;

    view->scale(scale,scale);
}

void EdtQtImageViewer::SetToImageSize(EdtImage *pimg)

{
    scene->setSceneRect(0,0,pimg->GetWidth(), pimg->GetHeight());

    view->SetZoom(1.0);
}
