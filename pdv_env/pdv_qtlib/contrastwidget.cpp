#include "contrastwidget.h"
#include <iostream>


ContrastWidget::ContrastWidget(EdtImage * source, EdtLut *l, QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    Setup(source, l);
}

ContrastWidget::ContrastWidget(EdtQtImageItem * item, QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    EdtImage *source = item->ActiveImage();

    EdtLut *l = item->lut();

    Setup(source, l);
}


ContrastWidget::ContrastWidget(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    Setup();

}

ContrastWidget::~ContrastWidget()
{
   
}

void ContrastWidget::Setup(EdtImage *src, EdtLut *l)

{
    this->setFixedSize(this->size());

    linearPlot = new EdtQtContrastCanvas(src, l);

    linearChecked = true;

    ui.Plot->Setup(linearPlot);
    linearPlot->setImage(src);
    if (l)
        linearPlot->setLut(l);

    ui.Plot->setTitle("Contrast");
    ui.Plot->setXAxisLabel("Input Pixel");
    ui.Plot->setYAxisLabel("Output");


    ui.minValueEdit->setText("0");
    ui.maxValueEdit->setText("255");

    ui.linearButton->setChecked(true);
    ui.gammaValEdit->setText("1.0");
    QObject::connect(ui.cbAllBox, SIGNAL(clicked()),
                     this, SLOT(cbAllBox_clicked()));
    QObject::connect(ui.cbRedBox, SIGNAL(clicked()),
                     this, SLOT(cbRedBox_clicked()));
    QObject::connect(ui.cbGreenBox, SIGNAL(clicked()),
                     this, SLOT(cbGreenBox_clicked()));
    QObject::connect(ui.cbBlueBox, SIGNAL(clicked()),
                      this, SLOT(cbBlueBox_clicked()));
    QObject::connect(ui.linearButton, SIGNAL(clicked()),
                     this, SLOT(linearButton_clicked()));
    QObject::connect(ui.gammaButton, SIGNAL(clicked()),
                     this, SLOT(gammaButton_clicked()));
    QObject::connect(ui.autoStretchButton,SIGNAL(clicked()),
                     this, SLOT(autoStretchButton_clicked()));
    
    QObject::connect(ui.gammaSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(gammaEdit_setValue(int)));
    QObject::connect(ui.gammaSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(adjust_gamma(int)));

    QObject::connect(ui.resetLinearButton, SIGNAL(clicked()),
        this, SLOT(resetLinear()));
        
    QObject::connect(linearPlot, SIGNAL(linearSet(int,int)),
        this, SLOT(linearSet(int,int)));
}

/************************
  SLOTS
************************/

void ContrastWidget::resetLinear()
{
    linearPlot->resetLinear();
}

void ContrastWidget::linearSet(int low, int high)

{
    ui.minValueEdit->setText(QString::number(low));
    ui.maxValueEdit->setText(QString::number(high));
}

int ContrastWidget::nBands()

{
    if (linearPlot->image())
        return linearPlot->image()->GetNColors();

    return 1;
}

void ContrastWidget::cbAllBox_clicked()
{
    if (this->ui.cbAllBox->isChecked())
    {
        ui.cbGreenBox->setChecked(true);
        ui.cbBlueBox->setChecked(true);
        ui.cbRedBox->setChecked(true);
        
        for (int i=0;i<nBands();i++)
            linearPlot->setColorEnabled(i,true);
        
    }
    else
    {
        ui.cbBlueBox->setChecked(false);
        ui.cbGreenBox->setChecked(false);
        ui.cbRedBox->setChecked(false);
    }
}

void ContrastWidget::cbRedBox_clicked()
{
    if (this->ui.cbRedBox->isChecked())
    {
        ui.cbGreenBox->setChecked(false);
        ui.cbBlueBox->setChecked(false);
    }
}

void ContrastWidget::cbGreenBox_clicked()
{
    if (nBands() > 1 && ui.cbGreenBox->isChecked())
    {
        ui.cbBlueBox->setChecked(false);
        ui.cbRedBox->setChecked(false);
    }
}

void ContrastWidget::cbBlueBox_clicked()
{
    if (nBands()> 2 && ui.cbBlueBox->isChecked())
    {
        ui.cbGreenBox->setChecked(false);
        ui.cbRedBox->setChecked(false);
    }
}

void ContrastWidget::linearButton_clicked()
{
    linearPlot->set_map_type(EdtLut::Linear);
    linearChecked = true;
}

void ContrastWidget::gammaButton_clicked()
{
    linearPlot->set_map_type(EdtLut::Gamma);
    linearChecked = false;
}

void ContrastWidget::autoStretchButton_clicked()
{
    linearPlot->set_map_type(EdtLut::AutoLinear);
    linearChecked = false;
}
void ContrastWidget::heqButton_clicked()
{
    linearPlot->set_map_type(EdtLut::AutoHEQ);
}

void ContrastWidget::adjust_gamma(int value)
{
    double gammaValue = (double)value / 10;
    linearPlot->setGamma(gammaValue);
    
}

/* value is actually a double, but slider values can only be ints */
void ContrastWidget::gammaEdit_setValue(int value)
{
    double actual_value = (double)value / 10;
    ui.gammaValEdit->setText(QString::number(actual_value));
}

void ContrastWidget::setLut(EdtLut *l)
{
    linearPlot->setLut(l);
}

void ContrastWidget::setImage(EdtImage *image)
{
    linearPlot->setImage(image);
}

void ContrastWidget::setGamma(double gamma)
{
    linearPlot->setGamma(gamma);
}

void ContrastWidget::update_display()

{
    linearPlot->update_display();
}

EdtQtContrastCanvas * ContrastWidget::canvas()

{
    return linearPlot;
}