#include "profilewidget.h"
#include "ui_profilewidget.h"



ProfileWidget::ProfileWidget(EdtImage *pImage, QLineF *pline, QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    m_image = pImage;

    Init(pImage, pline);

}
    
    
ProfileWidget::ProfileWidget(EdtQtImageItem *pitem, QLineF *pline, QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    this->setFixedSize(this->size());

    m_image = pitem->ActiveImage();

    Init(m_image, pline);

}
ProfileWidget::ProfileWidget(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    this->setFixedSize(this->size());

    m_image = NULL;

    Init(NULL, NULL);

}

EdtQtPlotView * ProfileWidget::plotView()

{
    return profile_view;
}


void ProfileWidget::Init(EdtImage *pImage, QLineF *pline)

{
    m_image = pImage;

    if (m_image)
    {
        int midy = m_image->GetHeight() / 2;

        if (pline)
            line = *pline;
        else
            line = QLineF(0,midy, m_image->GetWidth()-1, midy);

        
    }

    profile_view = ui.profile;

    profile_canvas = new EdtQtProfileCanvas(NULL);

    profile_view->Setup(profile_canvas);

    profile_view->setTitle("Histogram");
    profile_view->setXAxisLabel("Pixel Value");
    profile_view->setYAxisLabel("N Pixels");

    setImage(m_image);

    connect(profile_canvas, SIGNAL(MouseMoved(QPointF, QVector<int>)),
            this, SLOT(mouse_moved(QPointF, QVector<int>)));

}

ProfileWidget::~ProfileWidget()
{
   
}

void ProfileWidget::setImage(EdtImage *image)
{
    m_image = image;

    profile_canvas->setImage(image);
}

void ProfileWidget::setLine(QLineF new_line)
{
    line = new_line;
    profile_canvas->setLine(line);
}

void ProfileWidget::update_display()

{
    profile_canvas->update_display();
}

void  ProfileWidget::redButton_clicked()

{

}

void  ProfileWidget::blueButton_clicked()

{

}

void  ProfileWidget::greenButton_clicked()

{

}

void  ProfileWidget::allButton_clicked()

{

}

// slot to get data from the canvas

void ProfileWidget::mouse_moved(QPointF pt, QVector<int> values)
{
    // plot vertical line at x value

    QString s = QString::number(pt.x()) + QString(",") + QString::number(pt.y());

    ui.coordEdit->setText(s);

    int nbands = m_image->GetNColors();

    s = "";

    int i;
    for (i=0;i<nbands;i++)
    {
        if (i > 0)
            s += ",";
        
        s += QString::number(values[i]);
    }

    ui.valueEdit->setText(s);
 
}

