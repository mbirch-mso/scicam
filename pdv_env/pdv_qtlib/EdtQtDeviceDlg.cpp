
#include "EdtQtDeviceDlg.h"
#include <QMessageBox>
EdtQtDeviceDlg::EdtQtDeviceDlg(QWidget * parent) : QDialog(parent),
    ui(new Ui::EdtQtDeviceDlg)
{
    ui->setupUi(this);

	treeModel = new DevSelectTree();

	ui->treeView_device->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->treeView_device->setModel(treeModel);
	ui->treeView_device->setHeaderHidden(true);

	QObject::connect(ui->treeView_device->selectionModel(), 
		             SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
					 this, SLOT(setField(const QItemSelection &, const QItemSelection &)));

	QObject::connect(ui->okButton, SIGNAL(clicked()),
					 this, SLOT(handleOkButtonClick()));

    setWindowTitle("Select Device");

}

EdtQtDeviceDlg::~EdtQtDeviceDlg()
{
}


void
EdtQtDeviceDlg::setField(const QItemSelection &, const QItemSelection &)
{

}

void
EdtQtDeviceDlg::accept(const QString &, const int &, const int &)
{

}


void EdtQtDeviceDlg::onDeviceSelected()
{
    QMessageBox::information(this, tr("Debug"), tr("Debug: onDeviceSelected: OK"));

	//ok button is clicked
#if 0
	if((!(QString::compare(s_cfgFileName,"")==0))
	{
	    //use the PdvInput object to perform the config file setup
		DeviceSelected(s_devname, unit, channel);
	}
#endif
}

void EdtQtDeviceDlg::handleOkButtonClick()
{
    QMessageBox::information(this, tr("Debug"), tr("STUB: onDeviceSelected: OK"));
}

