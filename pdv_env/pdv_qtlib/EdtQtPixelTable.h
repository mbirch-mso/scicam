#ifndef EDTQTPIXELTABLE_H
#define EDTQTPIXELTABLE_H

#include <QTableWidget>

#include "edtimage/EdtImage.h"
#include "EdtQtImageItem.h"

class EdtQtPixelTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit EdtQtPixelTable(QWidget *parent = NULL);
    virtual ~EdtQtPixelTable();

    void SetPoint(QPointF pt);

    void update_display();

    void SetWindow(EdtQtImageItem *win);

    void SetImage(EdtImage *image);

    void SetImagePoint(EdtImage *image, QPointF pt);

    void SetDimensions(int rows, int columns);

    QPointF Center();

    QPoint UpperLeft();

    QPoint BottomRight();

    bool Hex();
    void SetHex(const bool state);

protected:


private:
    void SetItemAt(int row, int column, QString s);

    EdtQtImageItem *m_window;
    EdtImage * m_image;
    QPointF point;

    QPoint uleft;
    QPoint bright;

    bool is_hex;
};

#endif //EdtQtPlot
