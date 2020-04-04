#include "EdtQtPlotCurve.h"
#include <QDebug>

EdtQtPlotCurve::EdtQtPlotCurve(QGraphicsItem *parent)
	: QGraphicsPolygonItem(parent)
{

}

EdtQtPlotCurve::~EdtQtPlotCurve()
{

}

void EdtQtPlotCurve::setSamples(QVector<QPointF> &from)

{
    setPolygon(QPolygonF(from));

}

void EdtQtPlotCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)

{
    painter->setPen(pen());
    painter->drawPolyline(polygon());
}
