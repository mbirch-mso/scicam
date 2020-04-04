#include "EdtQwtPlot.h"
#include <stdio.h>

EdtQwtPlot::EdtQwtPlot(QWidget *parent) :
    QwtPlot(parent)
{
    m_last_x = -1;
    m_last_y = -1;
    m_mousePressed = false;
}

EdtQwtPlot::~EdtQwtPlot()
{}

void EdtQwtPlot::mouseReleaseEvent(QMouseEvent *ev)
{
    //reset last_x and last_y when mouse button is released
    // so that the plot doesn't jump around
    m_last_x = -1;
    m_last_y = -1;
    m_mousePressed = false;
    MouseReleased();

}

#include "qwt_plot_canvas.h"
bool EdtQwtPlot::transformEvent(QMouseEvent *ev, QPointF &pt)

{
    QRect crect = canvas()->geometry();


    if (crect.contains(ev->x(), ev->y()))
    {
        pt.setX(invTransform(2,ev->x()- crect.x()));
        pt.setY(invTransform(0,ev->y()- crect.y()));
        
        return true;
    }

    return false;

}

void EdtQwtPlot::mousePressEvent(QMouseEvent *ev)
{
    m_mousePressed = true;
    QPointF cursor_pt;

    m_last_x = ev->x();
    m_last_y = ev->y();

    if (transformEvent(ev, cursor_pt))
    {
        MousePressed(cursor_pt);
    }
      
}



void EdtQwtPlot::mouseMoveEvent(QMouseEvent *ev)
{
    int evX = ev->x();
    int evY = ev->y();

    // first mouse movement
    if(!(m_last_x == -1))
    {
        moveQXpos.enqueue(evX - m_last_x);
        moveQYpos.enqueue(evY - m_last_y);
    }
    else
    {
        moveQXpos.erase(moveQXpos.begin(), moveQXpos.end());
        moveQYpos.erase(moveQYpos.begin(), moveQYpos.end());
    }

    m_last_x = evX;
    m_last_y = evY;

    QPointF cursor_pt;

    if (transformEvent(ev, cursor_pt))
        MouseMoved(m_mousePressed, cursor_pt);    

}

/* Sets first time mouse button it pressed
   within the canvas */
bool EdtQwtPlot::get_mouse_pressed()
{
    return m_mousePressed;
}

double EdtQwtPlot::get_next_move_x()
{
     if(moveQXpos.empty())
         return 0;

     else
         return moveQXpos.dequeue();
}

double EdtQwtPlot::get_next_move_y()
{
    if(moveQYpos.empty())
        return 0;

    else
        return moveQYpos.dequeue();
}

double EdtQwtPlot::get_last_x()
{
    return m_last_x;
}

double EdtQwtPlot::get_last_y()
{
    return m_last_y;
}
