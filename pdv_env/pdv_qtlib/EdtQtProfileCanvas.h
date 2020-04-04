#ifndef EDTQTPROFILEPLOTAREA_H
#define EDTQTPROFILEPLOTAREA_H

#include "EdtQtPlotCanvas.h"

#include "edtimage/EdtRect.h"
#include "edtimage/EdtImage.h"
#include "edtimage/EdtImgProfile.h"

#include <QGraphicsSceneMouseEvent>

#include "EdtQtPlotCurve.h"

class EdtQtProfileCanvas : public EdtQtPlotCanvas
{
    Q_OBJECT

public:
    EdtQtProfileCanvas(QGraphicsItem *parent = NULL);
    EdtQtProfileCanvas(const QRectF &rect, QGraphicsItem *parent = NULL);
    EdtQtProfileCanvas(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = NULL);

    virtual ~EdtQtProfileCanvas();

    void setImage(EdtImage *p_image);
    EdtImage *Image();

    void Update();

    void setPoints(EdtPoint pt1, EdtPoint pt2);
    void setPoint1(EdtPoint pt1);
    void setPoint2(EdtPoint pt2);

    EdtPoint Point1();
    EdtPoint Point2();
        
    void update_display();

    void setLine(QLineF new_line);

    void setColorEnabled(int band, bool state);
    bool colorEnabled(int band);

private slots:

    void cursor_down_in_plot(QPointF pt); // mouse down or move while down

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);


private:

    //EdtQtImageItem *m_window;

    EdtImgProfile profile;

    QLineF line;

    int m_yMax;
    double length;

    EdtQtPlotCurve *m_dataCurve[4];

    void set_plot_size();
    void update_values();

    void Setup();


private:
    
    EdtImage *m_image;
    EdtPoint point1, point2;
    
};

#endif // EDTQTPROFILEPLOT_H
