#ifndef EDTQTHISTOGRAMITEM_H
#define EDTQTHISTOGRAMITEM_H

#include "EdtQtGraphicsItem.h"

#include "EdtIntervalData.h"

class EdtQtHistogramItem : public QAbstractGraphicsShapeItem
{

public:
	EdtQtHistogramItem(QGraphicsItem *parent = NULL);
	virtual ~EdtQtHistogramItem();

    void setSamples(QVector<EdtIntervalData> &data);
    void setSamples(QVector<int> &data);

    QRectF boundingRect() const;
    void paint(QPainter *painter, 
                const QStyleOptionGraphicsItem *option, 
                QWidget *widget = NULL);

private:
	
    bool use_intervals;

    QVector<EdtIntervalData> intervals;
    QVector<int>  integers;

    QRectF box;
};

#endif // EDTQTHISTOGRAMITEM_H
