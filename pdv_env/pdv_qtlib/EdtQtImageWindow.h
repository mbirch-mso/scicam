#ifndef EDTQTIMAGEWINDOW_H
#define EDTQTIMAGEWINDOW_H

#include "EdtQtImageView.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QSpacerItem>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QTimer>


class EdtQtImageWindow : public QMainWindow
{
	Q_OBJECT
 
    EdtQtImageView *image_window;
    QStatusBar *statusbar;

    bool own_update;

	QLabel *pixelvalues;
	QLabel *pixelX;
	QLabel *pixelY;
    QTimer *timer;

public:
	EdtQtImageWindow(QWidget *parent = 0);
    virtual ~EdtQtImageWindow(){};

    virtual void Setup();

    EdtQtImageItem *ImageWindow(const int index = 0);

    virtual EdtQtImageItem *AddImageItem(EdtImageData *pimg = NULL);

    void Refresh();

    double GetMeanUpdate();

    void GetCursorValue(QPoint &pt, QString &tag);

    bool GetAutoScale();

    void SetAutoScale(const bool ascale);

    void SetZoom(const double scale);

    void Scale(const double scale);

    double GetZoom();

    void SetOwnUpate(const bool state);

    bool GetOwnUpdate() const;

    void SetDataImage(EdtImage *img);

    EdtImage *GetDataImage();

    public slots:

    void UpdateStatus();

    void ZoomIn();
    void ZoomOut();
    void Zoom1();
    void ZoomAuto(bool checked);
};

#endif // EDTQTIMAGEVIEW_H
