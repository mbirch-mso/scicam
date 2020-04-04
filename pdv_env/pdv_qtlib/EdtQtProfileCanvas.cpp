#include "EdtQtProfileCanvas.h"


EdtQtProfileCanvas::EdtQtProfileCanvas(QGraphicsItem *parent)
    : EdtQtPlotCanvas(parent)
{
    Setup();
}

EdtQtProfileCanvas::~EdtQtProfileCanvas()
{

}

EdtQtProfileCanvas::EdtQtProfileCanvas(const QRectF &rect, QGraphicsItem *parent)
    : EdtQtPlotCanvas(rect, parent)
{
    Setup();
}

EdtQtProfileCanvas::EdtQtProfileCanvas(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : EdtQtPlotCanvas(x,y,width,height, parent)

{
    Setup();
}

void EdtQtProfileCanvas::Setup()

{
    int c;
        
    setAcceptHoverEvents(true);
    line = QLineF(0,0, 512,512);
    m_dataCurve[0] = new EdtQtPlotCurve(this);
    m_dataCurve[0]->setPen(QPen(QColor(Qt::red)));
    m_dataCurve[1] = new EdtQtPlotCurve(this);
    m_dataCurve[1]->setPen(QPen(QColor(Qt::green)));
    m_dataCurve[2] = new EdtQtPlotCurve(this);
    m_dataCurve[2]->setPen(QPen(QColor(Qt::blue)));


}



void EdtQtProfileCanvas::setImage(EdtImage *p_image)
{
    m_image = p_image;
    
    update_display();
}

EdtImage *EdtQtProfileCanvas::Image()
{
    return m_image;
}

void EdtQtProfileCanvas::Update()
{

}

void EdtQtProfileCanvas::setPoints(EdtPoint pt1, EdtPoint pt2)

{
    point1 = pt1;
    point2 = pt2;
}

void EdtQtProfileCanvas::setPoint1(EdtPoint pt1)

{
    point1 = pt1;
}

void EdtQtProfileCanvas::setPoint2(EdtPoint pt2)

{
    point2 = pt2;
}

EdtPoint EdtQtProfileCanvas::Point1()

{
    return point1;
}

EdtPoint EdtQtProfileCanvas::Point2()

{
    return point2;
}


void EdtQtProfileCanvas::update_display()

{
    update_values();

    QVector<QPointF> points[4];
    int values[3];
    if (!m_image)
        return;
    
    int ncolors = m_image->GetNColors();
    int i;
    int c;

    if (profile.GetNPixels() == 0)
        return;

    for (i=0;i<profile.GetNPixels();i++)
    {
        int x,
            y;
        
        profile.GetPointValuesAtIndex(i, x, y, values);
        for (c=0;c<ncolors;c++)
        {
            QPointF pt(i,values[c]);
            points[c].append(pt);
        }
    }

    for (c=0;c<ncolors;c++)
    {
        m_dataCurve[c]->setSamples(points[c]);
       
    }

}


void EdtQtProfileCanvas::setLine(QLineF new_line)
{
    line = new_line;
    update_display();
}

void EdtQtProfileCanvas::cursor_down_in_plot(QPointF pt)
{
    // plot vertical line at x value

    QVector<QPointF> data;

    int index = pt.x();

    int values[3];
    int xval;
    int yval;
    QString s;
    QVector<int> qvalues;

    int n = profile.GetPointValuesAtIndex(index, xval, yval, values);

    int i;
    for (i=0;i<n;i++)
        qvalues.append(values[i]);

    QPointF imagept = QPointF(xval,yval);

    MouseMoved(imagept, qvalues);

    setCursorLinePos(pt);
 
}

void EdtQtProfileCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)

{
    cursor_down_in_plot(ev->pos());
}


void EdtQtProfileCanvas::mousePressEvent(QGraphicsSceneMouseEvent *ev)

{
    cursor_down_in_plot(ev->pos());
}

void EdtQtProfileCanvas::set_plot_size()

{
    if (m_image)
    {
        m_yMax = m_image->GetMaxValue();
    }
    else
        return;

    // clip line

    double x1 = line.x1();
    double y1 = line.y1();
    double x2 = line.x2();
    double y2 = line.y2();
 
    if (x1 < 0)
        x1 = 0;
    if (x1 > m_image->GetWidth()-1)
        x1 = m_image->GetWidth()-1;
    if (x2 < 0)
        x2 = 0;
    if (x2 > m_image->GetWidth()-1)
        x2 = m_image->GetWidth()-1;

    if (y1 < 0)
        y1 = 0;
    if (y1 > m_image->GetHeight()-1)
        y1 = m_image->GetHeight()-1;
    if (y2 < 0)
        y2 = 0;
    if (y2 > m_image->GetHeight()-1)
        y2 = m_image->GetHeight()-1;

    profile.SetPoints(x1,y1, x2, y2);
    length = profile.GetNPixels();

    setScale(0,0,length, m_yMax);


}

void EdtQtProfileCanvas::update_values()

{
    set_plot_size();
    profile.Compute(m_image);
}