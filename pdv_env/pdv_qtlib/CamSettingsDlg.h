#ifndef CAMSETTINGSDLG_H
#define CAMSETTINGSDLG_H

#include "pdvplus/PdvInput.h"

#include <QWidget>

#include "ui_CamSettingsDlg.h"


namespace Ui {
    class CamSettingsDlg;
}

class CamSettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CamSettingsDlg(PdvInput *p_camera = NULL, QWidget *parent = 0);
    virtual ~CamSettingsDlg();

private slots:
    void handleOkButtonClick();
    void on_spinBox_Exposure_valueChanged();
    void on_horizontalSlider_Exposure_valueChanged();
    void on_spinBox_Gain_valueChanged();
    void on_horizontalSlider_Gain_valueChanged();
    void on_spinBox_Level_valueChanged();
    void on_horizontalSlider_Level_valueChanged();

private:

    Ui::CamSettingsDlg *ui;

    PdvInput *m_pCamera;

    void setExposure(int newval);
    void setGain(int newval);
    void setLevel(int newval);
};


#endif // CAMSETTINGSDLG_H
