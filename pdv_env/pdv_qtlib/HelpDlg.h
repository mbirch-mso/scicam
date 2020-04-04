#ifndef HELPDLG_H
#define HELPDLG_H

#include "pdvplus/PdvInput.h"

#include <QWidget>

#include "ui_HelpDlg.h"


namespace Ui {
    class HelpDlg;
}

class HelpDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDlg(QString name = NULL, QWidget *parent = 0);
    virtual ~HelpDlg();

private slots:
    void handleOkButtonClick();

private:

    Ui::HelpDlg *ui;

};


#endif // HELPDLG_H
