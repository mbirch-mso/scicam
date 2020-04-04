#ifndef DISPLAYSKIP_H
#define DISPLAYSKIP_H

#include "EdtQtLiveDisplay.h"

#include <QWidget>

#include "ui_DisplaySkip.h"


namespace Ui {
    class DisplaySkip;
}

class DisplaySkip : public QDialog
{
    Q_OBJECT

public:
    explicit DisplaySkip(EdtQtLiveDisplay *channel = NULL, QWidget *parent = 0);
    virtual ~DisplaySkip();

private slots:
    void handleOkButtonClick();
    void setSkip(int value);
    int getSkip();

private:

    Ui::DisplaySkip *ui;

    EdtQtLiveDisplay *liveDisplay;

};


#endif // DISPLAYSKIP_H
