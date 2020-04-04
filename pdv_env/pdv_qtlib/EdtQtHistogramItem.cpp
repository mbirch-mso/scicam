#include "EdtQtHistogramItem.h"

EdtQtHistogramItem::EdtQtHistogramItem(QGraphicsItem *parent)
	: QAbstractGraphicsShapeItem(parent)
{
    use_intervals = false;
}

EdtQtHistogramItem::~EdtQtHistogramItem()
{

}

void EdtQtHistogramItem::setSamples(QVector<EdtIntervalData> &data)

{
    use_intervals = true;

    intervals = data;

    box.setLeft(0);
    box.setTop(0);
    box.setRight(data.size());

    double v = 0;

    for (int i=0;i<data.size();i++)
        if (data[i].Value() > v)
            v = data[i].Value();

    box.setBottom(v);

}

void EdtQtHistogramItem::setSamples(QVector<int> &data)

{
    use_intervals = false;

    integers = data;

}

QRectF EdtQtHistogramItem::boundingRect() const

{
    return box;
}

void EdtQtHistogramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)

{
    painter->setBrush(brush());
    if (use_intervals)
    {
        int i;

        for (i=0;i<intervals.size();i++)
        {
            double freq = intervals[i].Value();
            double x1 = intervals[i].Interval().Low();
            double w = intervals[i].Interval().High() -x1;

            painter->fillRect(QRectF(x1,0,w,freq), brush());

        }
    }
}
