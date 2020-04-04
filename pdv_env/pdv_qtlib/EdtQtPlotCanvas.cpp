#include "EdtQtPlotCanvas.h"
#include "EdtQtPlotView.h"

#include <stdio.h>


EdtQtPlotCanvas::EdtQtPlotCanvas(QGraphicsItem *parent) :
    QGraphicsObject(parent)
{

    setRect(QRectF(0,0,640,480));
    cursor_line = NULL;
    plot_view = NULL;
    setPlotArea(rect());

}

EdtQtPlotCanvas::EdtQtPlotCanvas(const QRectF &rect, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    setRect(rect);
    cursor_line = NULL;
    plot_view = NULL;
    setPlotArea(rect);
    setAreaRect(rect); 
}

EdtQtPlotCanvas::EdtQtPlotCanvas(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    setRect(QRectF(x,y,width,height));
    cursor_line = NULL;
    plot_view = NULL;
    setPlotArea(rect());
    setAreaRect(rect());
}

EdtQtPlotCanvas::~EdtQtPlotCanvas() 
{

}

QRectF EdtQtPlotCanvas::boundingRect() const

{
    return box;
}

void EdtQtPlotCanvas::setRect(const QRectF &rect)
{
    box = rect;
}

QRectF EdtQtPlotCanvas::rect() const

{
    return box;
}

void EdtQtPlotCanvas::setAreaRect(const QRectF &rect)
{
    area_box = rect;
}

QRectF EdtQtPlotCanvas::areaRect() const

{
    return area_box;
}
void EdtQtPlotCanvas::setPlotArea(const QRectF &rect)
{
    plot_box = rect;

    float x1 = 0;

    if (!cursor_line)
    {
        cursor_line = new QGraphicsLineItem(this);
    }
    else
    {
        x1 = cursor_line->line().x1();
    }

    setCursorLinePos(rect.topLeft());
 
}

QRectF EdtQtPlotCanvas::plotArea() const

{
    return plot_box;
}

void EdtQtPlotCanvas::setBrush(const QBrush &b)
{
    the_brush = b;
}

QBrush EdtQtPlotCanvas::brush() const

{
    return the_brush;
}

void EdtQtPlotCanvas::setPen(const QPen &b)
{
    the_pen = b;
}

QPen EdtQtPlotCanvas::pen() const

{
    return the_pen;
}

void EdtQtPlotCanvas::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget )

{
    painter->fillRect(boundingRect(), brush());
    painter->drawRect(boundingRect());

}

void EdtQtPlotCanvas::setScale(double xmin, double ymin, double xmax, double ymax)

{
    if (xmax == xmin)
        xmax = xmin + 1;

    if (ymax == ymin)
        ymax = ymin + 1;

    double xwidth = xmax - xmin;
    double yheight = ymax - ymin;

    setPlotArea(QRectF(xmin,ymin, xwidth,yheight));

    rescale();

}
    
void EdtQtPlotCanvas::setPlot(EdtQtPlotView *p)

{
    plot_view = p;
}

EdtQtPlotView *EdtQtPlotCanvas::plot()

{
    return plot_view;
}


void EdtQtPlotCanvas::rescale()

{
    QTransform transform;

    double xscale = areaRect().width() / plot_box.width();
    double yscale = areaRect().height() / plot_box.height();

    double bottom = areaRect().height() + areaRect().y();

    transform.translate(areaRect().x(), bottom);
    transform.scale(xscale,-yscale);

    int c;

    setRect(plot_box);
    setTransform(transform);

    //for (c = 0; c< 3; c++)
    //    m_dataCurve[c]->setTransform(transform);


    EdtQtPlotView * parent = plot();
    if (parent)
    {
        parent->setAxisScale(0, plot_box.x(), (double)plot_box.right());
        parent->setAxisScale(2, plot_box.y(), (double)plot_box.bottom());
    }    
}

void EdtQtPlotCanvas::setCursorLineEnabled(const bool state)

{
    if (cursor_line)
        cursor_line->setVisible(state);

}

bool EdtQtPlotCanvas::cursorLineEnabled()

{
    return cursor_line->isVisible();
}

void EdtQtPlotCanvas::setCursorLinePos(QPointF pt)

{
    QLineF line = cursor_line->line();


    line.setLine(pt.x(),plot_box.y(),pt.x(), plot_box.bottom());

    cursor_line->setLine(line);

}
