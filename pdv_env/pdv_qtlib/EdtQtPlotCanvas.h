#ifndef EdtPlotCanvas_H
#define EdtPlotCanvas_H

#include <QMouseEvent>
#include <QQueue>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>

class EdtQtPlotView;

using namespace std;

class EdtQtPlotCanvas : public QGraphicsObject
{
    Q_OBJECT

public:
    EdtQtPlotCanvas(QGraphicsItem *parent = NULL);
    EdtQtPlotCanvas(const QRectF &rect, QGraphicsItem *parent = NULL);
    EdtQtPlotCanvas(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = NULL);
    virtual ~EdtQtPlotCanvas();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget = NULL);
    QRectF boundingRect() const;

    void setRect(const QRectF &rect);
    QRectF rect() const;
    void setAreaRect(const QRectF &rect);
    QRectF areaRect() const;

    void setBrush(const QBrush &brush);
    QBrush brush() const;
    void setPen(const QPen &brush);
    QPen pen() const;
        
    void setPlotArea(const QRectF &rect);
    QRectF plotArea() const;
   
    virtual void rescale();

    void setCursorLineEnabled(const bool state);
    bool cursorLineEnabled();

    virtual void setCursorLinePos(QPointF pt);

    virtual void setPlot(EdtQtPlotView *p);
    EdtQtPlotView *plot();

signals:

    void MouseMoved(QPointF pt, QVector<int> values);

protected:

    QRectF box;
    QRectF area_box;
    QRectF plot_box;

    QBrush the_brush;
    QPen the_pen;

    QGraphicsLineItem * cursor_line;

    void setScale(double xmin, double ymin, double xmax, double ymax);

    EdtQtPlotView *plot_view;

};

#endif //EdtQtPlot
