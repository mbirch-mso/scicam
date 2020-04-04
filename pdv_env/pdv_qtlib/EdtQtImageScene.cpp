#include "EdtQtImageScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QDebug>

EdtQtImageScene::EdtQtImageScene(QObject *parent)
    : QGraphicsScene(parent)
{
	last_x = 0;
	last_y = 0;
    m_startX = 0;
    m_startY = 0;
    m_mouseReleased = true;
    rubberband = NULL;
    current_selection_state = NoSelect;

}

EdtQtImageScene::~EdtQtImageScene()
{

}

void EdtQtImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)

{
    QPoint p = ev->scenePos().toPoint();

    last_x = p.x();
    last_y = p.y();

    if (rubberband && !m_mouseReleased)
    {
        endpoint = ev->scenePos();

        if (current_selection_state == LineSelect)
        {
            m_line = QLineF(origin, endpoint);
            ((QGraphicsLineItem *)rubberband)->setLine(m_line);
            Selected(m_line);
        }
        else
        {
            m_box = QRectF(origin, endpoint).normalized();
            
            ((QGraphicsRectItem *)rubberband)->setRect(m_box);
            Selected(m_box);
        }
    }

     QGraphicsItem *item = mouseGrabberItem();
   
/*
     if (item)
     {
         printf("item = %p\n", item);
     }
*/

     MouseMoved(p);

     QGraphicsScene::mouseMoveEvent(ev);
}

void EdtQtImageScene::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    QPoint p = ev->scenePos().toPoint();


    m_startX = p.x();
    m_startY = p.y();
    m_mouseReleased = false;

    origin = ev->scenePos();

    if (current_selection_state != NoSelect)
    {
        if (!rubberband)
        {
            if (current_selection_state == LineSelect)
            {
                m_line = QLineF(origin, origin);
                rubberband = addLine(m_line, QPen(Qt::red,1));
                Selected(m_line);
            }
            else if (current_selection_state == RectSelect)
            {
                // m_box = QRectF(origin, originQSize());
                m_box = QRectF(origin, origin);
                rubberband = addRect(m_box, QPen(Qt::red,1));
                Selected(m_box);
            }
        }

        rubberband->show();
    }

    MousePressed(ev->scenePos());

    QGraphicsScene::mousePressEvent(ev);

    QGraphicsItem *item = mouseGrabberItem();

}


void EdtQtImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (rubberband)
    {

        MouseMoved(ev->scenePos());
        
       

    }

    m_mouseReleased = true;
}

int EdtQtImageScene::get_last_mouse_xpos()
{
    return last_x;
}

int EdtQtImageScene::get_last_mouse_ypos()
{
    return last_y;
}

int EdtQtImageScene::getStartXPos()
{
    return m_startX;
}

int EdtQtImageScene::getStartYPos()
{
    return m_startY;
}

bool EdtQtImageScene::mouseReleased()
{
    return m_mouseReleased;
}

void EdtQtImageScene::SetSelectState(EdtQtImageScene::selection_state state)

{
    if (rubberband)
    {
        delete rubberband;
        rubberband = NULL;

    }

    current_selection_state = state;
}

EdtQtImageScene::selection_state EdtQtImageScene::SelectState(void)

{
    return current_selection_state;
}

QRectF &EdtQtImageScene::SelectionBox()

{
    return m_box;
}

QLineF &EdtQtImageScene::SelectionLine()
{
    return m_line;
}
