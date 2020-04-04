#include "pdvsim.h"
#include "EdtQtRegisterDlg.h" 

EdtQtRegisterDlg::EdtQtRegisterDlg(Cls *pdvsim_cls, QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
    cls = pdvsim_cls;
    ui.setupUi(this);
}

EdtQtRegisterDlg::~EdtQtRegisterDlg()
{

}

void EdtQtRegisterDlg::update(string *str)
{
    ui.textBrowser->setHtml(QString::fromStdString(*str));
}

int EdtQtRegisterDlg::on_pushButton_regUpdate_clicked()
{
    const string *s = cls->dumpRegisters();
    ui.textBrowser->setHtml(QString::fromStdString(*s));
    return 0;
}
