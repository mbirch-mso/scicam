#include "EdtQtPlotView.h"
#include "EdtQtImageScene.h"

#include <QApplication>

EdtQtPlotView::EdtQtPlotView(QWidget *parent)
    : QGraphicsView(parent)
{
    plot_area = NULL;
    base_item = NULL;
    title_item = NULL;
    x_axis_label_item = NULL;
    y_axis_label_item = NULL;

    x_axis_item = NULL;
    y_axis_item = NULL;

    scene = new EdtQtImageScene(this);
    setScene(scene);
    
    setMouseTracking(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

EdtQtPlotView::~EdtQtPlotView()
{

}

void EdtQtPlotView::resizeEvent(QResizeEvent *event)

{
     if (plot_area)
     {
         UpdateLayout();
     }
}

EdtQtImageScene * EdtQtPlotView::Scene()

{
    return scene;
}

void EdtQtPlotView::Setup(EdtQtPlotCanvas *canvas)

{
 
    // Layout 
    base_item = new QGraphicsRectItem(NULL);
    base_item->setBrush(QBrush(QColor(Qt::red)));

    scene->addItem(base_item);

    left_margin = 32;
    top_margin = 32;
    right_margin = 32;
    bottom_margin = 32;

    plot_area = canvas;

    plot_area->setParentItem(base_item);

    plot_area->setPlot(this);

    title_item = new QGraphicsSimpleTextItem("Title", base_item);
    x_axis_label_item = new QGraphicsSimpleTextItem("X Axis",base_item);
    y_axis_label_item = new QGraphicsSimpleTextItem("Y Axis", base_item);


    UpdateLayout();

}

void EdtQtPlotView::replot()

{
    update();
}


void EdtQtPlotView::UpdateLayout()

{
    if (rect().width() == 0)
        return;

    QRectF arearect = rect();


    arearect.adjust(left_margin,top_margin,-right_margin,-bottom_margin);

    plot_area->setRect(arearect);
    plot_area->setAreaRect(arearect);

    // center title

    QPointF center = rect().center();

    QRectF labelbox = title_item->boundingRect();

    title_item->setX(center.x() - labelbox.width()/2);
    title_item->setY(top_margin/2 - labelbox.height()/2);

    labelbox = y_axis_label_item->boundingRect();

    y_axis_label_item->setX(left_margin/2 - labelbox.width()/2);
    y_axis_label_item->setY(center.y() - labelbox.height()/2);

    labelbox = x_axis_label_item->boundingRect();

    x_axis_label_item->setX(center.x() - labelbox.width()/2);
    x_axis_label_item->setY(rect().bottom() -labelbox.height()/2 - bottom_margin/2);

    plot_area->rescale();

}

void EdtQtPlotView::setAxisMaxMajor(const int low, const int high)

{

}

void EdtQtPlotView::setAxisMaxMinor(const int low, const int high)

{

}
void EdtQtPlotView::setAxisScale(const int index, double min_value, double max_value)

{

}

void EdtQtPlotView::setTitle(const QString &s)

{
    title = s;
    title_item->setText(s);
    UpdateLayout();
}
const QString & EdtQtPlotView::Title()

{
    return title;
}
void EdtQtPlotView::setXAxisLabel(const QString &s)

{
    x_axis_label = s;
    x_axis_label_item->setText(s);
    UpdateLayout();
}
const QString & EdtQtPlotView::XAxisLabel()

{
    return x_axis_label;
}
void EdtQtPlotView::setYAxisLabel(const QString &s)

{
    y_axis_label = s;
    y_axis_label_item->setText(s);
    UpdateLayout();
}
const QString & EdtQtPlotView::YAxisLabel()

{
    return y_axis_label;
}

void EdtQtPlotView::setMargins(double l, double t, double r, double b)

{
    left_margin = l;
    top_margin = t;
    right_margin = r;
    bottom_margin = b;
    UpdateLayout();
}

void EdtQtPlotView::setTitleFont(const QFont &font)

{
    if (title_item)
        title_item->setFont(font);
    UpdateLayout();
}
void EdtQtPlotView::setAxisFont(const QFont &font)

{
    if (y_axis_label_item)
    {
        y_axis_label_item->setFont(font);
    }
    if (x_axis_label_item)
    {
        x_axis_label_item->setFont(font);
    }
    UpdateLayout();
}

QFont EdtQtPlotView::TitleFont() const

{
    if (title_item)
    {
        return title_item->font();
    }

    return QApplication::font();

}

QFont EdtQtPlotView::AxisFont() const

{
    if (y_axis_label_item)
    {
        return y_axis_label_item->font();
    }

    return QApplication::font();

}

EdtQtPlotCanvas *EdtQtPlotView::Canvas()

{
    return plot_area;
}
