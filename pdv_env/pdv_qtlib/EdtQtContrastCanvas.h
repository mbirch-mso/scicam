#ifndef EDTQTCONTRASTPLOTAREA_H
#define EDTQTCONTRASTPLOTAREA_H

#include "EdtQtPlotCanvas.h"

#include "edtimage/EdtRect.h"
#include "edtimage/EdtImage.h"
#include "edtimage/EdtImgProfile.h"

#include <QGraphicsSceneMouseEvent>

#include "EdtQtPlotCurve.h"
#include "edtimage/EdtLut.h"

class EdtQtContrastCanvas : public EdtQtPlotCanvas
{
    Q_OBJECT

public:
    EdtQtContrastCanvas(EdtImage *src = NULL, EdtLut *lut = NULL, QGraphicsItem *parent = NULL);
    virtual ~EdtQtContrastCanvas();

    void setImage(EdtImage *p_image);
    EdtImage *image();

    void Update();
      
    void update_display();

    void setColorEnabled(int band, bool state);
    bool colorEnabled(int band);

    EdtLut * lut();
    void setLut(EdtLut *l);

    void setGamma(double g);
    double gammaValue();

    void set_map_type(EdtLut::MapType type);

signals:

    void lutChanged(EdtLut *l);
    void linearSet(int low, int high);

public slots:

    void resetLinear();

private slots:

    void cursor_down_in_plot(QPointF pt); // mouse down or move while down

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);

    double minValue;
    double maxValue;
    double origMax;
    double origMin;

    void set_curve_data();
    void setup_plot_curves();

    void update_lut_values();
    void update();

    void enable_linear(bool state);

 
private:

    int m_nBands;

    int m_xMax;
    int m_yMax;
    
    int m_low_value;
    int m_high_value;

    EdtQtPlotCurve *m_dataCurve[4];

    QGraphicsRectItem *m_handle_low;
    QGraphicsRectItem *m_handle_high;
    QGraphicsLineItem *m_line;

    int handle_width;

    void set_plot_size();
    void update_values();

    void Setup(EdtImage *img, EdtLut *lut);

    void setLine(int lowval, int highval);

    int m_activeBand;

private:
    
    double gam;

    EdtImage *m_image;
    EdtLut work_lut;
    EdtLut *pLut;   

    bool is_linear;
    
    QGraphicsItem *moving;

    QPointF press_pt;
};

#endif // EDTQTPROFILEPLOT_H
