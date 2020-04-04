#ifndef HISTOWIDGET_H
#define HISTOWIDGET_H

#define QWT_DLL

#include <QDialog>
#include <QTimer>

#include "edtimage/EdtHistogram.h"
#include "EdtQtLiveDisplay.h"
#include "edtimage/EdtImgStats.h"
#include "EdtQtImageItem.h"

#include "EdtQtHistoCanvas.h"
#include "EdtQtPlotView.h"
#include "ui_histowidget.h"


class HistoWidget : public QDialog
{
    Q_OBJECT

public:
    explicit HistoWidget(EdtQtImageItem *item,  QWidget *parent = 0);
    explicit HistoWidget(EdtImage *source,  QWidget *parent = 0);
    explicit HistoWidget(QWidget *parent = 0);
    virtual ~HistoWidget();

    void Init();

    EdtQtHistoCanvas *HistoCanvas();

    public slots:
    void setImage(EdtImage *source);
    void update_display();
    void update_stats(QVector<EdtImgStats> *stats);
    void mouse_moved(QPointF pt, QVector<int> values);

private slots:
    void redButton_clicked();
    void blueButton_clicked();
    void greenButton_clicked();

private:
    Ui::HistoWidget ui;
 
    EdtQtPlotView * m_histoView;
    EdtQtHistoCanvas *m_histoCanvas;
        
    EdtImage *m_image;
    
    int m_nBands;
    int m_activeBand;

    QVector<EdtImgStats> m_stats;
    
    void setActiveBand(int band);

};

#endif // HISTOWIDGET_H
