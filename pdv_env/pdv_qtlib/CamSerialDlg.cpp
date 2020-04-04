
#include "CamSerialDlg.h"
#include <QMessageBox>

CamSerialDlg::CamSerialDlg(PdvInput *p_camera, QWidget * parent) : QDialog(parent), ui(new Ui::CamSerialDlg)
{
    ui->setupUi(this);

    m_pCamera = p_camera;

	QObject::connect(ui->okButton, SIGNAL(clicked()),
					 this, SLOT(handleOkButtonClick()));

    setWindowTitle("Camera Serial");

}

CamSerialDlg::~CamSerialDlg()

{

}

void CamSerialDlg::handleOkButtonClick()
{
}

void CamSerialDlg::on_sendButton_clicked()
{
    char outbuf[4096];
    int ishex = 0;
    int expected_chars = 0;

    ui->sendButton->setText("Sending...");
    ui->sendButton->setEnabled(false);
    ui->okButton->setEnabled(false);
    ui->sendButton->repaint();
    ui->okButton->repaint();

    m_pCamera->SendCommand(ui->lineEdit->text().toStdString().c_str(), ishex, outbuf);
    showSerial(ui->lineEdit->text(), "> ");
    if (*outbuf) 
        showSerial(outbuf, "> ");

    m_pCamera->ReadResponse(outbuf, ishex);
    if (*outbuf)
        showSerial(outbuf, "< ");

    ui->lineEdit->setText("");

    ui->sendButton->setText("Send");
    ui->sendButton->setEnabled(true);
    ui->okButton->setEnabled(true);
    ui->sendButton->repaint();
    ui->okButton->repaint();
}

// TODO: handle non-ascii
void CamSerialDlg::showSerial(const QString str, const QString pre)
{
    ui->textBrowser->append(pre + str);

}

// TODO: handle non-ascii
void CamSerialDlg::showSerial(const char *str, const QString pre)
{
    ui->textBrowser->append(pre + QString(str));
}

