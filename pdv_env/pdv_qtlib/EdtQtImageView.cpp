#include "EdtQtImageView.h"


#include <QMouseEvent>

EdtQtImageView::EdtQtImageView(QWidget *parent)
: QGraphicsView(parent)
{
    scene = new EdtQtImageScene;

    auto_scale = false;
    base_zoom = 1.0;
    setScene(scene);

    setMouseTracking(true);

    setAlignment(Qt::AlignLeft|Qt::AlignTop);
}

EdtQtImageView::~EdtQtImageView()
{
    delete scene;
}

void EdtQtImageView::Setup()

{


}

void EdtQtImageView::resizeEvent(QResizeEvent *event)

{
    if (auto_scale && ImageWindow(0))
    {
        fitInView(ImageWindow(0));
    }
}


EdtQtImageItem *EdtQtImageView::ImageWindow(const int index)

{

    QList<QGraphicsItem *> list = scene->items();

    //std::cout << "Size of List: " << list.size() << std::endl;
    if (list.size() > 0)
        return (EdtQtImageItem *) list.at(index);

    return NULL;
}

EdtQtImageItem *EdtQtImageView::AddImageItem()

{
    EdtQtImageItem *item = new EdtQtImageItem();

    scene->addItem(item);

    item->setAcceptHoverEvents(true);

    return item;

}

void EdtQtImageView::Refresh()

{
    QList<QGraphicsItem *> list = scene->items();
    int i;
    for (i=0;i<list.size();i++)
        ((EdtQtImageItem *) list.at(i))->UpdateIfNeeded();
}

double EdtQtImageView::GetMeanUpdate()

{
    EdtQtImageItem *item = ImageWindow(0);

    if (item)
    {
        return item->MeanDelta();
    }

    return 0;

}

void EdtQtImageView::GetCursorValue(QPoint &q, QString &tag)

{
    int x,y, value;
    int imgIdx;
    QList<QGraphicsItem *> list = scene->items();

    imgIdx = list.size() - 1;

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

        if (nvalues == 1)
            sprintf(buf, "%d", values[0]);
        else if (nvalues == 3)
            sprintf(buf, "(%d,%d,%d)", values[0], values[1], values[2]);

    }

    tag = buf;


}

bool EdtQtImageView::GetAutoScale()

{
    return auto_scale;
}

void EdtQtImageView::SetAutoScale(const bool ascale)

{
    auto_scale = ascale;
    // FIX - set new state on items
    QList<QGraphicsItem *> list = scene->items();
    int i;
    for (i=0;i<list.size();i++)
    {
    }

}

void EdtQtImageView::SetZoom(const double scale)

{
    base_zoom = scale;

    QTransform t;

    t.scale(base_zoom, base_zoom);

    setTransform(t);


}

double EdtQtImageView::GetZoom()

{
    return base_zoom;
}

void EdtQtImageView::Scale(const double dscale)

{
    base_zoom *= dscale;

    scale(dscale,dscale);
}

void  EdtQtImageView::SetDataImage(EdtImage *img)

{
    int imgIdx;

    QList<QGraphicsItem *> list = scene->items();
    imgIdx = (list.size() - 1);

    EdtQtImageItem *item = ImageWindow(imgIdx);

    if (!item)
    {
        item = AddImageItem();
    }
    item->SetDataImage(img);

}


EdtImage *EdtQtImageView::GetDataImage()

{
    int imgIdx;

    QList<QGraphicsItem *> list = scene->items();
    imgIdx = (list.size() - 1);

    EdtQtImageItem *item = ImageWindow(imgIdx);

    return item->GetDataImage();

}
