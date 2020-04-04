#ifndef EDTQWTPLOT_H
#define EDTQWTPLOT_H

#define QWT_DLL

#include "qwt_plot.h"
#include "qwt_compat.h"
#include "qwt_plot_curve.h"

#include <QMouseEvent>
#include <QQueue>

using namespace std;

class EdtQwtPlot : public QwtPlot
{
    Q_OBJECT

public:
    EdtQwtPlot(QWidget *parent = NULL);
    virtual ~EdtQwtPlot();

    bool get_mouse_pressed();
    double get_next_move_x();
    double get_next_move_y();
    double get_last_x();
    double get_last_y();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    
    bool transformEvent(QMouseEvent *ev, QPointF &pt);

signals:
    void MouseMoved(bool pressed, QPointF pt);
    void MouseReleased();
    void MousePressed(QPointF pt);

private:

    QQueue<double> moveQXpos;
    QQueue<double> moveQYpos;
    double m_last_x;
    double m_last_y;
    bool m_mousePressed;

};

#endif //EDTQWTPLOT
