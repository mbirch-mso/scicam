#ifndef EDTQTDEVICEDLG_H
#define EDTQTDEVICEDLG_H

#include "pdvplus/PdvInput.h"
#include <QWidget>

#include "ui_EdtQtDeviceDlg.h"

#include "pdv_qtlib/DevSelectTree.h"

class DevSelectTree;

namespace Ui {
    class EdtQtDeviceDlg;
}

class EdtQtDeviceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EdtQtDeviceDlg(QWidget *parent = 0);
    virtual ~EdtQtDeviceDlg();

private Q_SLOTS:
    void setField(const QItemSelection &, const QItemSelection &);
    void accept(const QString &, const int &, const int &);
    void onDeviceSelected();
    void handleOkButtonClick();

private:

    Ui::EdtQtDeviceDlg *ui;

    DevSelectTree *treeModel;

};


#endif // EDTQTDEVICEDLG_H
