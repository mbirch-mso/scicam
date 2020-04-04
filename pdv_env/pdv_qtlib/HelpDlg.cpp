#include "HelpDlg.h"
#include <QResource>

HelpDlg::HelpDlg(QString name, QWidget * parent) : QDialog(parent), ui(new Ui::HelpDlg)
{
    if (name.size())
    {
        QResource r( name );
        QByteArray helpText( reinterpret_cast< const char * >( r.data() ), r.size() );

        ui->setupUi(this);

        ui->textBrowser->setOpenExternalLinks(true);
        ui->textBrowser->setText(helpText);

    	QObject::connect(ui->okButton, SIGNAL(clicked()),
    					 this, SLOT(handleOkButtonClick()));

        setWindowTitle("Help");
    }

}

HelpDlg::~HelpDlg()

{

}

void HelpDlg::handleOkButtonClick()
{

}

