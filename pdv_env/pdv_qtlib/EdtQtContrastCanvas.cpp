#include "EdtQtContrastCanvas.h"

#include "EdtQtPlotView.h"

EdtQtContrastCanvas::EdtQtContrastCanvas(EdtImage *img, EdtLut *lut, QGraphicsItem *parent)
    : EdtQtPlotCanvas(parent)
{
    Setup(img,lut);
}

EdtQtContrastCanvas::~EdtQtContrastCanvas()
{

}

void EdtQtContrastCanvas::Setup(EdtImage *src, EdtLut *lut)

{
     
    handle_width = 5;

    m_image = src;

    for (int i=0;i<4;i++)
    {
        QColor q;
        switch (i)
        {
        case 0:
            q = QColor(Qt::red);
            break;
        case 1:
            q = QColor(Qt::green);
            break;
        case 2:
            q = QColor(Qt::blue);
            break;
        case 3:
            q = QColor(Qt::black);
        break;
        }
        
        m_dataCurve[i] = new EdtQtPlotCurve(this);
        
        m_dataCurve[i]->setPen(QPen(q));
        

    }
    
    m_handle_low = new QGraphicsRectItem(this);
     m_handle_low->setBrush(QBrush(QColor(Qt::red)));


    m_handle_high = new QGraphicsRectItem(this);
    m_handle_high->setBrush(QBrush(QColor(Qt::red)));

    m_line = new QGraphicsLineItem(this);

    enable_linear(true);

    m_xMax = 255;

    m_activeBand = 0;

    if (lut)
        pLut = lut;
    else       
        pLut = &work_lut;

    
    m_image = NULL;
    resetLinear();

    set_plot_size();
    
    gam = 1;

    m_activeBand = -1;

}

void EdtQtContrastCanvas::setLine(int lowval, int highval)

{

    if (lowval < 0)
        lowval = 0;
    else if (lowval >= m_xMax)
        lowval = m_xMax;
     if (highval < 0)
        highval = 0;
    else if (highval >= m_xMax)
        highval = m_xMax;

    QPointF p1((double) lowval, 0.0);
    QPointF p2((double) highval, 255.0);

    m_low_value = lowval;
    m_high_value = highval;

    m_line->setLine(QLineF(p1,p2));

    double x1 = p1.x()-handle_width;
    if (x1 < 0)
        x1 = 0;
    else if (x1 > m_xMax-handle_width*2)
        x1 = m_xMax-handle_width*2;


    m_handle_low->setRect(x1, p1.y(), handle_width * 2, handle_width * 2);
    x1 = p2.x()-handle_width;
    if (x1 < 0)
        x1 = 0;
    else if (x1 > m_xMax-handle_width*2)
        x1 = m_xMax-handle_width*2;
    m_handle_high->setRect(x1, p2.y()-2 *handle_width, handle_width * 2, handle_width * 2);

    linearSet(lowval, highval);

    update_lut_values();

}

void EdtQtContrastCanvas::resetLinear()

{
    setLine(0,m_xMax);
    
}

void EdtQtContrastCanvas::setImage(EdtImage *p_image)
{
    m_image = p_image;
    
    if (pLut->GetMapType() == EdtLut::AutoLinear)
    {
        update_lut_values();

    }
    update_display();
}

EdtImage *EdtQtContrastCanvas::image()
{
    return m_image;
}

void EdtQtContrastCanvas::Update()
{

}

void EdtQtContrastCanvas::update_display()

{
    if(!is_linear)
    {
        set_curve_data();

    }

}

void EdtQtContrastCanvas::cursor_down_in_plot(QPointF pt)
{
 
 
}

void EdtQtContrastCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)

{
    if (moving == m_handle_low)
        setLine(ev->pos().x(), m_high_value);
    else if (moving == m_handle_high)
        setLine(m_low_value, ev->pos().x());
    else if (moving == m_line)
    {
        double dx = ev->pos().x() - press_pt.x();
        setLine(m_low_value + dx, m_high_value + dx);
        press_pt = ev->pos();
    }

}


void EdtQtContrastCanvas::mousePressEvent(QGraphicsSceneMouseEvent *ev)

{
    

    if (m_handle_low->isUnderMouse())
    {
        moving = m_handle_low;

    }
    else if (m_handle_high->isUnderMouse())
    {
        moving = m_handle_high;

    }
    else 
    {
        moving = m_line;
        press_pt = ev->pos();
    }
    
}

void EdtQtContrastCanvas::set_plot_size()

{
    if (m_image)
    {
        m_xMax = m_image->GetMaxValue();
    }
    else
        m_xMax = 255;


    m_yMax = 255;
    
    setScale(0,0,m_xMax, m_yMax);

}

void EdtQtContrastCanvas::update_values()

{
    
}


/* A curve consists of a series of x,y coordinates.
   x = index, maximum x = length of domain of lut
   y = value from the lut (8 bit), max = 255
   Once curve data is set the plot will automatically redraw
   with a call to replot()
 */
void EdtQtContrastCanvas::set_curve_data()
{

    int band;

    for (band = 0;band < image()->GetNColors(); band++)
    {
        QVector<QPointF> data;

        unsigned char * pLutArray = (unsigned char *) pLut->GetLutElement(band);

        for (int i=0; i<pLut->GetDomainLength(); i++)
        {
            data.append(QPointF((double) i,(double)pLutArray[i]));
        }

        m_dataCurve[band]->setSamples(data);

    }

}

void EdtQtContrastCanvas::setup_plot_curves()
{
    for (int i=0;i<3;i++)
    {
        m_dataCurve[i] = new EdtQtPlotCurve(NULL);

        m_dataCurve[i]->setPen(QPen(Qt::black, 2));
    }

     
}

void EdtQtContrastCanvas::enable_linear(bool state)

{
    is_linear = state;

    
    m_line->setVisible(is_linear);
    m_handle_low->setVisible(is_linear);
    m_handle_high->setVisible(is_linear);
    m_dataCurve[0]->setVisible(!is_linear);
    m_dataCurve[1]->setVisible(!is_linear);
    m_dataCurve[2]->setVisible(!is_linear);
    

}

void EdtQtContrastCanvas::set_map_type(EdtLut::MapType type)
{
    
    enable_linear(type == EdtLut::Linear);

    pLut->SetMapType(type);

    update_lut_values();

    update_display();
}

void EdtQtContrastCanvas::update_lut_values()
{
    int band;
    int start, end;
    
    if (m_activeBand == -1)
    {
        start = 0; 
        end = image()->GetNColors();
    }
    else
    {
        start = m_activeBand;
        end = m_activeBand + 1;
    }
         
    for (band=start; band<end; band++)
    {
        switch (pLut->GetMapType())
        {
        case EdtLut::Linear:
            pLut->SetLinear(m_low_value, m_high_value, band);
            break;
            
        case EdtLut::AutoLinear:
            break;

        case EdtLut::Gamma:
            pLut->SetGamma(gam, band);
            break;

        case EdtLut::Sqrt:
        case EdtLut::AutoHEQ:
        case EdtLut::Custom:

            printf("LUT type not supported\n");
            return;

        case EdtLut::Copy:
            break;
            
        }
    }

    pLut->UpdateMap(m_image);

    lutChanged(pLut);

    
}

void EdtQtContrastCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)

{
    moving = NULL;
}

void EdtQtContrastCanvas::setColorEnabled(int band, bool state)
{
    m_dataCurve[band]->setVisible(state);
}

void EdtQtContrastCanvas::setLut(EdtLut *l)
{
    pLut = l;
}

void EdtQtContrastCanvas::setGamma(double g)
{
    gam = g;
       
    update_lut_values();

    update_display();
}

double EdtQtContrastCanvas::gammaValue()

{
    return gam;
}

