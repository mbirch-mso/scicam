#include "EdtQtHistoCanvas.h"

#include "EdtQtPlotView.h"

EdtQtHistoCanvas::EdtQtHistoCanvas(QGraphicsItem *parent)
    : EdtQtPlotCanvas(parent)
{
    Setup();
}

EdtQtHistoCanvas::~EdtQtHistoCanvas()
{

}

EdtQtHistoCanvas::EdtQtHistoCanvas(const QRectF &rect, QGraphicsItem *parent)
    : EdtQtPlotCanvas(rect, parent)
{
    Setup();
}

EdtQtHistoCanvas::EdtQtHistoCanvas(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : EdtQtPlotCanvas(x,y,width,height, parent)

{
    Setup();
}

void EdtQtHistoCanvas::Setup()

{
    int c;

    setAcceptHoverEvents(true);

    m_dataCurve[0] = new EdtQtHistogramItem(this);
    m_dataCurve[1] = new EdtQtHistogramItem(this);
    m_dataCurve[2] = new EdtQtHistogramItem(this);
    m_dataCurve[0]->setBrush(QBrush(QColor(Qt::red)));
    m_dataCurve[1]->setBrush(QBrush(QColor(Qt::green)));
    m_dataCurve[2]->setBrush(QBrush(QColor(Qt::blue)));
    m_dataCurve[0]->setPen(QPen(QColor(Qt::red)));
    m_dataCurve[1]->setPen(QPen(QColor(Qt::green)));
    m_dataCurve[2]->setPen(QPen(QColor(Qt::blue)));

}

void EdtQtHistoCanvas::setImage(EdtImage *p_image)
{
    m_image = p_image;

    update_display();
}

EdtImage *EdtQtHistoCanvas::Image()
{
    return m_image;
}

void EdtQtHistoCanvas::Update()
{

}
void EdtQtHistoCanvas::set_curve_data(int band)
{

    QVector<EdtIntervalData> data;

    int *histoData = m_histogram.GetHistogram(band);

    for (int i=0; i<m_histogram.GetNBins(); ++i)
    {

        EdtIntervalData pt((double) histoData[i], (double) i, (double) i+1.5);

        data.append(pt);
    }

    m_dataCurve[band]->setSamples(data);
    //m_dataCurves[band].attach(m_histoPlot);
}

int EdtQtHistoCanvas::get_y_max(int band)
{
    int max = 0;
    int *histoData = m_histogram.GetHistogram(band);
    for (int i=0; i<m_histogram.GetNBins(); ++i)
    {
        if(histoData[i] > max)
            max = histoData[i];
    }

    return max + 4096;
}

void EdtQtHistoCanvas::compute_geometry()

{
    // avoid changing y axis unless off by more than sqrt(2)
    int old_y_max = m_yMax;

    m_nBands = m_image->GetNColors();

    m_xMax = m_histogram.GetNBins();


    int max = 0;
    m_yMax = 0;
    for(int i=0; i<m_nBands; ++i)
    {
        max = get_y_max(i);
        if(max > m_yMax)
            m_yMax = max;
    }

    if (m_yMax > old_y_max || m_yMax < old_y_max / 1.414)
    {
        m_yMax = m_yMax * 1.1;
    }
    else
        m_yMax = old_y_max;

    setScale(0,0,m_xMax, m_yMax);

    
}
void EdtQtHistoCanvas::update_display()

{

    if (m_image)
    {
        m_nBands = m_image->GetNColors();

        m_histogram.Compute(m_image);

        EdtImgStats temp_stats[4];

        m_histogram.ComputeStatistics(temp_stats);

        m_stats.clear();

        for (int i=0;i<m_nBands;i++)
            m_stats.append(temp_stats[i]);

        compute_geometry();

        for(int i=0; i<m_nBands; ++i)
        {
            set_curve_data(i);
        }

        update_stats(&m_stats);
    }

}


void EdtQtHistoCanvas::cursor_down_in_plot(QPointF pt)
{

    int index = pt.x();
    if (index < 0 || index >= m_histogram.GetNBins())
    {
        return;
    }

    QVector<int> qvalues;

    for (int band = 0;band < m_nBands; band++)
    {
        int *histoData = m_histogram.GetHistogram(band);

        qvalues.append(histoData[index]);

    }

    MouseMoved(pt, qvalues);

    setCursorLinePos(pt);

}

void EdtQtHistoCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)

{
    cursor_down_in_plot(ev->pos());
}

void EdtQtHistoCanvas::mousePressEvent(QGraphicsSceneMouseEvent *ev)

{
    cursor_down_in_plot(ev->pos());
}

void EdtQtHistoCanvas::setColorEnabled(int band, bool state)
{
    m_dataCurve[band]->setVisible(state);
}
