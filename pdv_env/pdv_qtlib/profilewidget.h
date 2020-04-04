#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include "edtimage/EdtImgProfile.h"


#include <QWidget>
#include <QDialog>

#include "EdtQtPlotView.h"
#include "EdtQtProfileCanvas.h"

#include "EdtQtImageItem.h"

#include "ui_profilewidget.h"



class ProfileWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileWidget(EdtQtImageItem *pItem, QLineF *pline = 0, QWidget *parent = 0);
    explicit ProfileWidget(EdtImage *pImage, QLineF *pline = 0, QWidget *parent = 0);
    explicit ProfileWidget(QWidget *parent = 0);
    virtual ~ProfileWidget();

    void Init(EdtImage *pImage, QLineF *pline);

    EdtQtPlotView *plotView();

public slots:

    void setLine(QLineF new_line);
    void setImage(EdtImage *image);
    void update_display();

private slots:
    void redButton_clicked();
    void blueButton_clicked();
    void greenButton_clicked();
    void allButton_clicked();

    void mouse_moved(QPointF pt, QVector<int> values);

private:
    Ui::ProfileWidget ui;

    EdtImage *m_image;

    QLineF line;


    EdtQtPlotView * profile_view;
    
    EdtQtProfileCanvas *profile_canvas;
    
};

#endif // PROFILEWIDGET_H
