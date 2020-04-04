#ifndef EDTQTPLOTCURVE_H
#define EDTQTPLOTCURVE_H

#include "EdtQtGraphicsItem.h"


class EdtQtPlotCurve : public QGraphicsPolygonItem
{

public:
	EdtQtPlotCurve(QGraphicsItem *parent = 0);
	virtual ~EdtQtPlotCurve();

	void setSamples(QVector<QPointF> &other);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget = NULL);


private:
    Q_DISABLE_COPY(EdtQtPlotCurve)
};

#endif // EDTQTPLOTCURVE_H
