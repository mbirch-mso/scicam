#ifndef CAMSERIALDLG_H
#define CAMSERIALDLG_H

#include "pdvplus/PdvInput.h"

#include <QWidget>

#include "ui_CamSerialDlg.h"


namespace Ui {
    class CamSerialDlg;
}

class CamSerialDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CamSerialDlg(PdvInput *p_camera = NULL, QWidget *parent = 0);
    virtual ~CamSerialDlg();

private slots:
    void handleOkButtonClick();
    void on_sendButton_clicked();

private:

    Ui::CamSerialDlg *ui;

    PdvInput *m_pCamera;

    void showSerial(const QString str, const QString pre = "");
    void showSerial(const char *str, const QString pre = "");
};


#endif // CAMSERIALG_H
