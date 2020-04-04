#ifndef EdtQtPixelDlg_H
#define EdtQtPixelDlg_H

#include <QWidget>
#include <QTimer>

#include "ui_EdtQtPixelDlg.h"
//#include "DisplayContrast.h"
#include "EdtQtLiveDisplay.h"
#include "EdtQtImageScene.h"


namespace Ui {
    class EdtQtPixelDlg;
}

class EdtQtPixelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EdtQtPixelDlg(EdtQtImageItem * win, QWidget *parent = 0);
    virtual ~EdtQtPixelDlg();

    void SetPoint(QPointF pt);

    void update_display();

    void SetWindow(EdtQtImageItem *win);

    void SetImage(EdtImage *image);
    void SetImagePoint(EdtImage *image, QPointF pt);

    EdtQtImageItem * m_window;
    
    EdtQtPixelTable *table;



private slots:
    void onHexChecked(bool checked);

    void onEditPt();

private:
    void RefreshPoints();

    Ui::EdtQtPixelDlg *ui;

};



#endif // EdtQtPixelDlg_H
