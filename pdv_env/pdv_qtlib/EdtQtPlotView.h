#ifndef EDTQTPLOTVIEW_H
#define EDTQTPLOTVIEW_H

#include <QGraphicsView>


#include "EdtQtImageScene.h"
#include "EdtQtPlotCanvas.h"

class EdtQtPlotView : public QGraphicsView
{
    Q_OBJECT

public:
    EdtQtPlotView(QWidget *parent);
    virtual ~EdtQtPlotView();

    EdtQtImageScene *scene;

    void Setup(EdtQtPlotCanvas *p);

    void resizeEvent(QResizeEvent *event);
        
    EdtQtImageScene * Scene();
    EdtQtPlotCanvas *Canvas();

	virtual void setAxisMaxMajor(const int low, const int high);
	virtual void setAxisMaxMinor(const int low, const int high);
	virtual void setAxisScale(const int index, double min_value, double max_value);

    virtual void setTitle(const QString &s);
    virtual const QString &Title();
    virtual void setXAxisLabel(const QString &s);
    virtual const QString &XAxisLabel();
    virtual void setYAxisLabel(const QString &s);
    virtual const QString &YAxisLabel();

    virtual void setMargins(double left, double top, double right, double bottom);

    
	void replot();

    void UpdateLayout();

    void setTitleFont(const QFont &font);
    void setAxisFont(const QFont &font);

    QFont TitleFont() const;
    QFont AxisFont() const;

protected:


    QString title;
    QString x_axis_label;
    QString y_axis_label;

    double left_margin, top_margin, bottom_margin, right_margin;

    QGraphicsRectItem *base_item;
    EdtQtPlotCanvas *plot_area;
    QGraphicsSimpleTextItem *title_item;
    QGraphicsSimpleTextItem *x_axis_label_item;
    QGraphicsSimpleTextItem *y_axis_label_item;

    QGraphicsLineItem * x_axis_item;
    QGraphicsLineItem * y_axis_item;


private:

    QRectF plot_box;

};

#endif // EDTQTPLOTVIEW_H
