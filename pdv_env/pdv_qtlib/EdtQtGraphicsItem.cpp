#include "EdtQtGraphicsItem.h"

EdtQtGraphicsItem::EdtQtGraphicsItem(QGraphicsItem *w):
    QGraphicsObject(w)
{
    is_image = false;
    needs_redraw = false;
    m_width = 1;
    m_height = 1;
}

EdtQtGraphicsItem::~EdtQtGraphicsItem()
{

}

void EdtQtGraphicsItem::UpdateIfNeeded()
{
    if (!is_image && needs_redraw)
        update();
}

void EdtQtGraphicsItem::setPen(const QPen &pen)
{
    m_pen = pen;
}

void EdtQtGraphicsItem::setBrush(const QBrush &brush)
{
    m_brush;
}

void EdtQtGraphicsItem::SetShape(Shape shape)
{
    m_shape = shape;
}

void EdtQtGraphicsItem::SetSize(const int width,const int height)
{
    m_width = width;
    m_height = height;
}

void EdtQtGraphicsItem::SetWidth(const int width)
{
    m_width = width;
}

void EdtQtGraphicsItem::SetHeight(const int height)
{
    m_height = height;
}

void EdtQtGraphicsItem::SetStartPos(const int xpos,const int ypos)
{
    m_startPos.setX(xpos);
    m_startPos.setY(ypos);
}

void EdtQtGraphicsItem::SetEndPos(const int xpos,const int ypos)
{
    m_endPos.setX(xpos);
    m_endPos.setY(ypos);
}

QRectF EdtQtGraphicsItem::boundingRect() const

{
    return QRectF(m_startPos.x(), m_startPos.y(), m_width, m_height);
}

void EdtQtGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (m_shape)
    {
    case Line:
        painter->drawLine(m_startPos, m_endPos);
        break;
    default:
        break;
    }
}

void EdtQtGraphicsItem::SetNeedsRedraw(const bool state)
{
    needs_redraw = state;
}

bool EdtQtGraphicsItem::isImage() const
{
    return is_image;
}

bool EdtQtGraphicsItem::NeedsRedraw()

{
    return needs_redraw;
}

