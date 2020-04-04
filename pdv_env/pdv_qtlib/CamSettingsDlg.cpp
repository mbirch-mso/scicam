
#include "CamSettingsDlg.h"
#include <QMessageBox>

CamSettingsDlg::CamSettingsDlg(PdvInput *p_camera, QWidget * parent) : QDialog(parent), ui(new Ui::CamSettingsDlg)
{
    ui->setupUi(this);

    m_pCamera = p_camera;

	QObject::connect(ui->okButton, SIGNAL(clicked()),
					 this, SLOT(handleOkButtonClick()));

    int low = 0, high = 0;
    if (m_pCamera)
    {
        if (!m_pCamera->GetMinMaxShutter(low, high))
        {
            ui->horizontalSlider_Exposure->setDisabled(true);
            ui->spinBox_Exposure->setDisabled(true);
        }
        else
        {
            ui->horizontalSlider_Exposure->setMinimum(low);
            ui->horizontalSlider_Exposure->setMaximum(high);
            ui->spinBox_Exposure->setMinimum(low);
            ui->spinBox_Exposure->setMaximum(high);
        }

        if (!m_pCamera->GetMinMaxGain(low, high))
        {
            ui->horizontalSlider_Gain->setDisabled(true);
            ui->spinBox_Gain->setDisabled(true);
        }
        else
        {
            ui->horizontalSlider_Gain->setMinimum(low);
            ui->horizontalSlider_Gain->setMaximum(high);
            ui->spinBox_Gain->setMinimum(low);
            ui->spinBox_Gain->setMaximum(high);
        }

        if (!m_pCamera->GetMinMaxLevel(low, high))
        {
            ui->horizontalSlider_Level->setDisabled(true);
            ui->spinBox_Level->setDisabled(true);
        }
        else
        {
            ui->horizontalSlider_Level->setMinimum(low);
            ui->horizontalSlider_Level->setMaximum(high);
            ui->spinBox_Level->setMinimum(low);
            ui->spinBox_Level->setMaximum(high);
        }
    }

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    setWindowTitle("Camera Settings");

}

CamSettingsDlg::~CamSettingsDlg()

{

}

void CamSettingsDlg::handleOkButtonClick()
{
}


void CamSettingsDlg::on_spinBox_Exposure_valueChanged()
{
    ui->horizontalSlider_Exposure->setValue(ui->spinBox_Exposure->value());
    setExposure(ui->spinBox_Exposure->value());
}

void CamSettingsDlg::on_horizontalSlider_Exposure_valueChanged()
{
    ui->spinBox_Exposure->setValue(ui->horizontalSlider_Exposure->value());
}

void CamSettingsDlg::setExposure(int newval)
{
    // Get the current value
    int nValue;
    if (!m_pCamera->GetShutterSpeed(nValue))
    {
        return;
    }

    m_pCamera->SetShutterSpeed(ui->spinBox_Exposure->value());
}

void CamSettingsDlg::on_spinBox_Gain_valueChanged()
{
    ui->horizontalSlider_Gain->setValue(ui->spinBox_Gain->value());
    setGain(ui->horizontalSlider_Gain->value());
}

void CamSettingsDlg::on_horizontalSlider_Gain_valueChanged()
{
    ui->spinBox_Gain->setValue(ui->horizontalSlider_Gain->value());
}

void CamSettingsDlg::setGain(int newval)
{
    // Get the current value
    int nValue;
    if (!m_pCamera->GetGain(nValue))
    {
        return;
    }

    m_pCamera->SetGain(newval);
}

void CamSettingsDlg::on_spinBox_Level_valueChanged()
{
    ui->horizontalSlider_Level->setValue(ui->spinBox_Level->value());
    setLevel(ui->spinBox_Level->value());
}

void CamSettingsDlg::on_horizontalSlider_Level_valueChanged()
{
    ui->spinBox_Level->setValue(ui->horizontalSlider_Level->value());
}

void CamSettingsDlg::setLevel(int newval)
{
    // Get the current value
    int nValue;
    if (!m_pCamera->GetLevel(nValue))
    {
        return;
    }

    m_pCamera->SetLevel(newval);
}
