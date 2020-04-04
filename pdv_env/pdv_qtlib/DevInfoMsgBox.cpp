#include "DevInfoMsgBox.h"
#include <QFile>

DevInfoMsgBox::DevInfoMsgBox(PdvInput *p_camera, QWidget *parent) 
{
    QString infoString;
    QPixmap *pixmap = NULL;
    QString pixmap_name;
    QSpacerItem* horizontalSpacer = new QSpacerItem(580, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout;

    msgBox = new QMessageBox;

    /* use spacer item to force wider width, to prevent text wrap */
    layout = (QGridLayout*)msgBox->layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    QString lconfig = (QString)p_camera->GetLoadedConfig();
    int llen = lconfig.length();

    if (p_camera)
    {
        infoString =
        "<p><b>Board:</b> " + (QString)p_camera->GetDevIdStr()
        + "<br><b>Address:</b> " + (QString)p_camera->GetDevName() + " unit " + QString::number(p_camera->GetUnitNumber()) + " channel " + QString::number(p_camera->GetChannelNumber())
        + "<br><b>Part #:</b> " + (QString)p_camera->GetSerialNumStr()
        + "<br><b>Serial #:</b> " +  (QString)p_camera->GetPartNumStr()
        + "<br><b>Hardware:</b> " + (QString)p_camera->GetFpgaMfg() + " " + (QString)p_camera->GetFpgaDesc()
          + (QString)p_camera->GetBusDesc() + " FPGA, "
          + (QString)p_camera->GetPromDesc() + " FPROM"
        + "<br><b>Device ID:</b> 0x" + QString::number(p_camera->GetDevId(), 16)
        + "<br><b>Driver:</b> " + (QString)p_camera->GetDevName() + " v. " + (QString)p_camera->GetDriverVersion()
        + "<br><b>Libraries:</b> pdvlib/edtlib v. " + (QString)p_camera->GetLibraryVersion()
        + "<br><b>Configuration:</b> " + ((llen == 0)? "not configured": lconfig)
        + ", " + QString::number(p_camera->GetWidth())
        + " pixels x "
        + QString::number(p_camera->GetHeight()) + 
        + " lines x "
        + QString::number(p_camera->GetDepth()) + 
        + " bits, "
        +  QString::number(p_camera->GetNumTaps())
        + " taps"
        + "</p>" ;

        QString devname = (QString)p_camera->GetDevIdStr();
        devname = devname.toLower();
        devname.replace(" ", "_");
        pixmap_name = ":/pdv_qtlib/Resources/" + devname + ".png";

        if (!QFile::exists(pixmap_name))
            pixmap_name = ":/pdv_qtlib/Resources/board_icon.png";

        pixmap = new QPixmap(pixmap_name);
        msgBox->setIconPixmap(*pixmap);
    }
    else
    {
        infoString = "Device not found";
        msgBox->setIcon(QMessageBox::Warning);
    }


    msgBox->setWindowTitle("Device info");
    msgBox->setText(infoString);
    msgBox->show();
    delete(pixmap);
}

DevInfoMsgBox::~DevInfoMsgBox()
{
    delete(msgBox);
}

