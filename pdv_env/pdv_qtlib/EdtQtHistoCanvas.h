#ifndef EDTQTHISTOPLOTAREA_H
#define EDTQTHISTOPLOTAREA_H

#include "EdtQtPlotCanvas.h"

#include "edtimage/EdtRect.h"
#include "edtimage/EdtImage.h"
#include "edtimage/EdtHistogram.h"

#include <QGraphicsSceneMouseEvent>

#include "EdtQtHistogramItem.h"


class EdtQtHistoCanvas : public EdtQtPlotCanvas
{
    Q_OBJECT

public:
    EdtQtHistoCanvas(QGraphicsItem *parent = NULL);
    EdtQtHistoCanvas(const QRectF &rect, QGraphicsItem *parent = NULL);
    EdtQtHistoCanvas(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = NULL);
    virtual ~EdtQtHistoCanvas();

    EdtImage *Image();

    void Update();

    void update_display();


    void setColorEnabled(int band, bool state);
    bool colorEnabled(int band);

signals:

    void update_stats(QVector<EdtImgStats> * stats);

public slots:

    void setImage(EdtImage *p_image);


private slots:

    void cursor_down_in_plot(QPointF pt); // mouse down or move while down

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);

    void compute_geometry();
    void set_curve_data(int band);
    int get_y_max(int band);

private:


    EdtHistogram m_histogram;

    EdtQtHistogramItem *m_dataCurve[4];
    QVector<EdtImgStats> m_stats;

    void set_plot_size();
    void update_values();

    void Setup();

    int m_yMax;
    int m_nBands;
    int m_xMax;

private:

    EdtImage *m_image;
};

#endif // EDTQTPROFILEPLOT_H
