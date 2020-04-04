#ifndef EDTQTGRAPHICSITEM_H
#define EDTQTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>



class EdtQtGraphicsItem: public QGraphicsObject
{

    Q_OBJECT

public:
    enum Shape { Line, Rect, Ellipse, PolyLine };

    EdtQtGraphicsItem(QGraphicsItem *w = NULL);
    virtual ~EdtQtGraphicsItem();

    virtual void UpdateIfNeeded();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual QRectF boundingRect() const;

    virtual void SetShape(Shape shape);
    virtual void setPen(const QPen &pen);
    virtual void setBrush(const QBrush &brush);
    virtual void SetSize(const int width,const int height);
    virtual void SetWidth(const int width);
    virtual void SetHeight(const int height);
    virtual void SetStartPos(const int xpos, const int ypos);
    virtual void SetEndPos(const int xpos,const int ypos);
    virtual void SetNeedsRedraw(const bool state);

    bool isImage() const;

    bool NeedsRedraw();

    bool needs_redraw;



protected:
    bool is_image;

private:
    Shape m_shape;
    QPen m_pen;
    QBrush m_brush;
    int m_width, m_height;
    QPoint m_startPos, m_endPos;


};

#endif //EDTQTGRAPHICSITEM_H
