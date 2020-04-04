#ifndef EDTQTREGISTERDIALOG_H
#define EDTQTREGISTERDIALOG_H

#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QDialog>
#include <QString>
#include "ui_pdvsim_registers.h"
#include "cls.h"

class EdtQtRegisterDlg : public QDialog
{
	Q_OBJECT

public:
	EdtQtRegisterDlg(Cls *pdvsim_cls, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EdtQtRegisterDlg();
    void update(string *str);
	
private slots:
    int on_pushButton_regUpdate_clicked();

private:
	Ui::EdtQtRegisterDlg ui;
    string m_regString;
    Cls *cls;
};

#endif // EDTQTREGISTERDIALOG_H
