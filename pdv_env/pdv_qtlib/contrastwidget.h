#ifndef CONTRASTWIDGET_H
#define CONTRASTWIDGET_H


#include "edtimage/EdtLut.h" 

#include <QDialog>


#include "ui_contrastwidget.h"
//#include "DisplayContrast.h"
#include "EdtQtLiveDisplay.h"
#include "EdtQtImageScene.h" 

#include "EdtQtPlotCurve.h"
#include "EdtQtContrastCanvas.h"

#include "EdtQtImageItem.h"



class ContrastWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ContrastWidget(EdtImage * image, EdtLut *l, QWidget *parent = 0);
    explicit ContrastWidget(EdtQtImageItem * item, QWidget *parent = 0);
    explicit ContrastWidget(QWidget *parent = 0);
    virtual ~ContrastWidget();

    void update_display();
    void Setup(EdtImage *src = NULL, EdtLut *l = NULL);

    int nBands();
        
    EdtQtContrastCanvas *canvas();
    
    EdtImage *Image();
    
    EdtLut * lut();
    

signals:
   void clicked();
   void valueChanged(int);

public slots:

   void setImage(EdtImage *image);
   void setLut(EdtLut *lut);
   void setGamma(double gamma);

   private slots:
   void cbAllBox_clicked();
   void cbRedBox_clicked();
   void cbGreenBox_clicked();
   void cbBlueBox_clicked();
   void linearButton_clicked();
   void gammaButton_clicked();
   void autoStretchButton_clicked();
   void gammaEdit_setValue(int value);
   void heqButton_clicked();
   void adjust_gamma(int value);
   void resetLinear();
   void linearSet(int lowval, int highval);

  

private:
    Ui::ContrastWidget ui;

    EdtQtContrastCanvas *linearPlot;

    bool linearChecked;
};



#endif // CONTRASTWIDGET_H
