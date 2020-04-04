#ifndef EDTIMAGESCENE_H
#define EDTIMAGESCENE_H

#include "pdvplus/edtdefines.h"

#include <QGraphicsScene>

class EdtQtImageScene : public QGraphicsScene
{
    Q_OBJECT

    QGraphicsItem *rubberband;

    QRectF m_box; // selected region
    QLineF m_line;    // selected line
    QPointF origin;
    QPointF endpoint;

public:
    EdtQtImageScene(QObject *parent = NULL);
    virtual ~EdtQtImageScene();
	
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    int get_last_mouse_xpos();
    int get_last_mouse_ypos();
    int getStartXPos();
    int getStartYPos();
    bool mouseReleased();

    enum selection_state {
        NoSelect,
        LineSelect,
        RectSelect
    };

    void SetSelectState(selection_state state);

    selection_state SelectState();
    
    bool GetEndPoints(EdtPoint &pt1, EdtPoint &pt2);

    QRectF &SelectionBox();
    QLineF &SelectionLine();
    
signals:
        
    void MouseMoved(QPointF pt); // any mouse move event
    void Selected(QLineF line); // when a selection is finished 
    void Selected(QRectF rect); // when a selection is finished 
    void MousePressed(QPointF pt);

private:
	int last_x, last_y;
    int m_startX, m_startY;
    bool m_mouseReleased;
      selection_state current_selection_state;
  
};

#endif // EDTIMAGESCENE_H
