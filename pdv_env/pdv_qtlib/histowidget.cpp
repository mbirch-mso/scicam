#include <math.h>
#include "histowidget.h"
#include "ui_histowidget.h"

#include <QDebug>

#include <QPointF>

#include "EdtIntervalData.h"

HistoWidget::HistoWidget(QWidget *parent) :
QDialog(parent)
{
    ui.setupUi(this);

    m_image = NULL;


    Init();


}
HistoWidget::HistoWidget(EdtImage *pImage, QWidget *parent) :
QDialog(parent)
{
    ui.setupUi(this);

    m_image = pImage;

    Init();

}

HistoWidget::HistoWidget(EdtQtImageItem *source, QWidget *parent) :
QDialog(parent)
{
    ui.setupUi(this);

    m_image = source->ActiveImage();

    Init();

}
void HistoWidget::Init()

{
    if (m_image)
        m_nBands = m_image->GetNColors();
    else
        m_nBands = 1;

    m_activeBand = 0;

    m_histoView = ui.histoPlotView;

    m_histoCanvas = new EdtQtHistoCanvas(NULL);
    m_histoView->Setup(m_histoCanvas);
    
    m_histoView->setTitle("Histogram");
    m_histoView->setXAxisLabel("Pixel Value");
    m_histoView->setYAxisLabel("N Pixels");

    ui.redButton->setChecked(true);

    QObject::connect(ui.redButton, SIGNAL(clicked()),
        this, SLOT(redButton_clicked()));
    QObject::connect(ui.blueButton, SIGNAL(clicked()),
        this, SLOT(blueButton_clicked()));
    QObject::connect(ui.greenButton, SIGNAL(clicked()),
        this, SLOT(greenButton_clicked()));

    connect(m_histoCanvas, SIGNAL(MouseMoved(QPointF, QVector<int>)),
            this, SLOT(mouse_moved(QPointF, QVector<int>)));
    connect(m_histoCanvas, SIGNAL(update_stats(QVector<EdtImgStats> *)),
            this, SLOT(update_stats(QVector<EdtImgStats> *)));

    m_histoCanvas->setImage(m_image);
}

HistoWidget::~HistoWidget()
{

}

void HistoWidget::setImage(EdtImage *pImage)

{
    m_image = pImage;
    if (pImage)
        m_nBands = pImage->GetNColors();

    m_histoCanvas->setImage(m_image);

}

//SLOTS

/**
 * @fn  void HistoWidget::redButton_clicked()
 *
 * @brief   Red button clicked.
**/

void HistoWidget::redButton_clicked()
{
    if(m_nBands > 1)
    {
          setActiveBand(0);

       
    }
}

void HistoWidget::setActiveBand(int band)

{
    int i;

    for (i=0;i<m_nBands;i++)
    {
        if (i == band)
            m_histoCanvas->setColorEnabled(i,true);
        else
            m_histoCanvas->setColorEnabled(i,false);
     }
         
    m_activeBand = band;
    m_histoCanvas->update();
    update_stats(NULL);
}

/**
 * @fn  void HistoWidget::blueButton_clicked()
 *
 * @brief   Blue button clicked.
**/


void HistoWidget::blueButton_clicked()
{
    if(m_nBands > 1)
    {
        setActiveBand(2);
    }
}

/**
 * @fn  void HistoWidget::greenButton_clicked()
 *
 * @brief   Green button clicked.
**/

void HistoWidget::greenButton_clicked()
{
    if(m_nBands > 1)
    {
         setActiveBand(1);

    }
}

/**
 * @fn  void HistoWidget::update_display()
 *
 * @brief   Updates the display.
**/

void HistoWidget::update_display()
{
    m_histoCanvas->setImage(m_image);
    
}

/**
 * @fn  void HistoWidget::update_stats()
 *
 * @brief   Updates the stats.
**/

void HistoWidget::update_stats(QVector<EdtImgStats> *stats) 

{
    if (stats)
        m_stats = *stats;

    if (m_stats.size()> m_activeBand)
    {

        ui.meanEdit->setText(QString::number(m_stats[m_activeBand].m_dMean));
        ui.medianEdit->setText(QString::number(m_stats[m_activeBand].m_nMedian));
        ui.sdEdit->setText(QString::number(m_stats[m_activeBand].m_dSD));
        ui.minPixelEdit->setText(QString::number(m_stats[m_activeBand].m_nMin));
        ui.maxPixelEdit->setText(QString::number(m_stats[m_activeBand].m_nMax));
        ui.nPixelEdit->setText(QString::number(m_stats[m_activeBand].m_nN));
        ui.modeEdit->setText(QString::number(m_stats[m_activeBand].m_nMode));

        ui.modeFreqEdit->setText(QString::number(m_stats[m_activeBand].m_nModeFreq));
    }
}


void HistoWidget::mouse_moved(QPointF pt, QVector<int> values)
    
{   
    QString s;

    ui.indexEdit->setText(QString::number((int) pt.x()));
 
    int i;

    for (i=0;i<values.size();i++)
    {
        if (i)
            s += ",";
        s += QString::number(values[i]);
    }
    ui.freqEdit->setText(s);

}

EdtQtHistoCanvas * HistoWidget::HistoCanvas()

{
    return m_histoCanvas;
}
