
#include "DisplaySkip.h"

DisplaySkip::DisplaySkip(EdtQtLiveDisplay *dpy, QWidget * parent) : QDialog(parent), ui(new Ui::DisplaySkip)
{
    ui->setupUi(this);

    liveDisplay = dpy;

    QObject::connect(ui->okButton, SIGNAL(clicked()),
                     this, SLOT(handleOkButtonClick()));

    QObject::connect(ui->spinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setSkip(int)));

    setWindowTitle("Display Skip");
}

DisplaySkip::~DisplaySkip()

{

}

void DisplaySkip::setSkip(int value)
{
    liveDisplay->SetSkip(value);
}

int DisplaySkip::getSkip()
{
    return liveDisplay->GetSkip();
}

void DisplaySkip::handleOkButtonClick()
{

}

