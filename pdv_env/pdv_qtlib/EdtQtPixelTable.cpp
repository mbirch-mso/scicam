
#include "EdtQtPixelTable.h"

EdtQtPixelTable::EdtQtPixelTable(QWidget * parent) : QTableWidget(parent)

{
    m_window = NULL;
    m_image = NULL;
    point.setX(-1);
    point.setY(-1);
    is_hex = false;


}

EdtQtPixelTable::~EdtQtPixelTable()

{

}

void EdtQtPixelTable::SetWindow(EdtQtImageItem *win)

{
    m_window = win;

    SetImage(win->ActiveImage());

}

void EdtQtPixelTable::SetImage(EdtImage *image)

{
    m_image = image;

    update_display();

}

void EdtQtPixelTable::SetPoint(QPointF pt)

{
    point = pt;

    update_display();
}

void EdtQtPixelTable::SetImagePoint(EdtImage *image, QPointF pt)

{
    m_image = image;
    point = pt;

    update_display();

}

void EdtQtPixelTable::SetItemAt(int row, int column, QString s)

{
    QTableWidgetItem *pitem = item(row,column);

    if (pitem == NULL)
    {
        pitem = new QTableWidgetItem(s);
        setItem(row,column, pitem);
        pitem->setTextAlignment(Qt::AlignCenter);
    }
    else    
        pitem->setText(s);
        
}

void EdtQtPixelTable::update_display()

{
    if (m_image)
    {

        int x1, y1, x2, y2;
        int dx, dy;
        int c;
        int fullwidth, halfwidth;
        int fullheight, halfheight;
        int nbands;
        int row, column;
        
        if (point.x() < 0 || point.y() < 0 || point.x() > m_image->GetWidth()-1 || point.y() > m_image->GetHeight()-1)
        {
            point.setX(m_image->GetWidth()/2);
            point.setY(m_image->GetHeight()/2);
        }
          
        nbands = m_image->GetNColors();

        fullwidth= columnCount();
        halfwidth= fullwidth/2;
        fullheight= rowCount();
        halfheight= fullheight/2;

        if (halfwidth == 0)
            halfwidth = 1;
        if (halfheight == 0)
            halfheight = 1;

        x1 = point.x() - halfwidth;
        y1 = point.y() - halfheight;

        if (x1 < 0)
            x1 = 0;
        if (y1 < 0)
            y1 = 0;

        x2 = x1 + fullwidth;
        y2 = y1 + fullheight;

        if (x2 > m_image->GetWidth()-1)
            x2 = m_image->GetWidth()-1;
        if (y2 > m_image->GetHeight()-1)
            y2 = m_image->GetHeight()-1;

        QStringList labels;

        for (row=0, dy=y1;row<fullheight;row++, dy++)
        {
            labels.append(QString::number(dy));            
        }

        setVerticalHeaderLabels(labels);

        labels.clear();


        for (column=0, dx=x1;column<fullwidth;column++, dx++)
        {
            labels.append(QString::number(dx));            
        }
          
        setHorizontalHeaderLabels(labels);
      
        for (row=0;row <fullheight;row++)
        {
            for (column=0;column < fullwidth;column++)
            {
                QString s = "";

                int values[4];

                m_image->GetPixelV(x1+column, y1+row, values);

                for (c=0;c<nbands;c++)
                {
                    if (c > 0)
                        s += QString(", ");
                    s += QString::number(values[c], (is_hex)?16:10);
                }

                SetItemAt(row, column, s);
            }
        }
    }
}

void EdtQtPixelTable::SetHex(bool state)

{
    bool old_state = is_hex;
    is_hex = state;
    if (old_state != state)
        update_display();
}

QPointF EdtQtPixelTable::Center()

{
    return point;
}

