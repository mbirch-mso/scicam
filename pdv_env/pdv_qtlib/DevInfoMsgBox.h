#ifndef DEVINFOMSGBOX_H
#define DEVINFOMSGBOX_H

#include "pdvplus/PdvInput.h"
#include <QMessageBox>
#include <QString>
#include <QLayout>
#include <QGridLayout>
#include <QSpacerItem>


class DevInfoMsgBox
{

public:
    DevInfoMsgBox(PdvInput *p_camera, QWidget *parent = 0);
    ~DevInfoMsgBox();

private:
    QMessageBox *msgBox;

};

#endif //DEVINFOMSGBOX_H 
