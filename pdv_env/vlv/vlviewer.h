#ifndef PDVSHOW_H
#define PDVSHOW_H

#include "edtinc.h"
#include "pdvplus/PdvInput.h"

#include <QMainWindow>
#include <QTimer>
#include <QHBoxLayout>
#include <QList>
#include <QString>
#include <QObjectCleanupHandler>
#include <QFileDialog>
#include <QComboBox>
#include <QSharedMemory>
#include "qtcamselect.h"
#include "contrastwidget.h"
#include "histowidget.h"
#include "profilewidget.h"
#include "EdtQtImageViewer.h"
#include "EdtQtLiveDisplay.h"
#include "EdtQtImageItem.h"
#include "EdtQtPixelDlg.h"
#include "EdtQtDeviceDlg.h"
#include "EdtQtFileInfo.h"
#include "CamSettingsDlg.h"
#include "CamSerialDlg.h"
#include "HelpDlg.h"
#include "DisplaySkip.h"
#include "AppSettings.h"
#include "DevInfoMsgBox.h"

#define QSdevname "pdv" + QString::number(unit) + "_" + QString::number(dma_channel)

namespace Ui {
    class mainWindow;
}

class vlviewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit vlviewer(QWidget *parent = 0);
    ~vlviewer();
    EdtQtLiveDisplay * channel;
    EdtQtImageViewer viewer;
    bool lock();
    bool SetupLive(const int unit=0, const int channel=0, const QString configPath="");
    void NotifyData(EdtImage *image);
    QAction *addActionToToolBar(const QString &, const QString &);
    AppSettings *appSettings;
    bool already_running() { return alreadyLocked; };
    bool do_quit() { return doQuit; };
    void usage(QString msg = "");

private:
    QSharedMemory sharedMemory;
    void detach();
    bool alreadyLocked;
    bool noBoard;
    int doQuit; // use 0 for don't quit, 1 for quit with 0, higher values quit with doQuit - 1
    QString lockFile;
    QString sharedKey;

signals:
   void triggered();
   void timeout();

private Q_SLOTS:
    void on_actionExit_triggered();
    void on_actionTool_Bar_changed();

    void onNewImage();
    void onOpenImage();
    void onSaveImage();
    void onDevice_info();
    void onSelect_Device();

    void on_actionSaveImageAs_triggered();
    void on_actionSaveImage_triggered();

    void on_actionAbout_vlviewer_triggered();
    void on_actionDevice_info_triggered();

    void on_actionHardware_invert_changed();
    void on_actionFirst_pixel_counter_changed();

    void on_actionSelect_Device_triggered();
    void on_actionCamera_Settings_triggered();
    void on_actionCamera_Serial_triggered();
    void on_actionDisplay_Skip_triggered();
    void on_actionVlviewer_Help_triggered();

    void on_actionIncrease_Exposure_Time_triggered();
    void on_actionDecrease_Exposure_Time_triggered();

    void on_actionAutoNumber_changed();

    void onSelect_Camera(bool at_startup=true);
    void onCamera_Settings();
    void onCamera_Serial();

    void onCapture_One_Image();
    void onContinuous_Capture();

    void onPixel_Text();
    void onHistogram();
    void onProfile();

    void onContrast();
    void onBayer();
    void onSet_ROI();
 
    void onReset_Selection();
    void onSelect_Region();
    void onSelect_Line();

    void onZoom_In();
    void onZoom_Out();
    void onNormal_Size();

    void onCursorMoved(QPointF pt);

    void onSaveDialogAutoNumberChanged(int state);
    void onSaveDialogFilterChanged();

    void onSelectedBox(QRectF box);
    void onSelectedLine(QLineF box);

    void Stop();
    void UpdateStatus();
    void SetupSelected(const QString &cfg_path, const QString &cfg_name);
    void SetupDevice(const QString &dev, const int &u, const int &c);
    void SetupSaveFilters();
    const QString SaveFilter(int i);
    void zoom_level_changed(int);

protected:
    int createToolbar();  //populate the toolbar
    void setButtonsConfigured(bool state);
    void createStatusBar();
    QString getFileSuffix(QString qsPathname, char *cImagetype, int maxlen);
    bool doSave(QString filename, bool forceOverwrite = false);
    QString getNextFileName(const QString fname, int increment=1);
    QString getNextFileName(const QString lastName, QString &filter, int increment=1);
    bool isAutoNumbered(QString basename);
    QString stripAutoNumber(QString fileName, uint &number);
    bool canSave(const QString qsFmt);
    const QString getSaveFilterString(const QString qsFmt);
    const QString getSaveFilterType(const QString filter);
    void setStatusTimeouts(int nTimeouts);
    void setTitle();
    void initButtons();
    void closeEvent(QCloseEvent *evt);

    Ui::mainWindow *ui;

    EdtQtImageItem * image_item;
    EdtQtImageScene *scene;

    EdtImage * m_image; 

    int capturing; // flag to indicate capture is happening
    bool at_startup;

    //List for garbage collection
    QObjectCleanupHandler *cleaner;
    QList<QIcon *> iconCleaner;

    QString iconPath;

    DevInfoMsgBox *devInfoBox;
    EdtQtDeviceDlg *devSelectDialog;
    CamSettingsDlg *camSettingsDialog;
    CamSerialDlg *camSerialDialog;
    HelpDlg *helpDialog;
    DisplaySkip *displaySkipDialog;
    QtCamSelect *camSelectDialog;
    ContrastWidget *contrastWidget;
    HistoWidget *histoWidget;
    ProfileWidget * profileWidget;
    EdtQtPixelDlg * pixelWidget;

    QString saveFileName;
    QString saveDir;
    QFileDialog *saveDialog;
    QCheckBox *saveDialogAutoNumberCheckBox;
    QGridLayout *saveDialogLayout;

    QRectF selected_box;
    QLineF selected_line;
    QPointF last_point;

    QTimer * timer;

    bool line_changed;
    bool box_changed;
    bool point_changed;

    bool has_box;
    bool has_line;

    void update_line();
    void update_box();
    void update_point();

    QLabel * statusTimeoutsLabel;
    QLabel * statusXYLabel;
    QLabel * statusPixLabel;
    QLabel * statusFPSLabel;
    QLabel * statusFCountLabel;
    QLabel * statusSelectionLabel;
    QLabel * statusSkippedLabel;
    QLineEdit * statusTimeoutsEdit;
    QLineEdit * statusFCountEdit;
    QLineEdit * statusFPSEdit;
    QLineEdit * statusPixEdit;
    QLineEdit * statusXYEdit;
    QLineEdit * statusSelectionEdit;
    QLineEdit * statusSkippedEdit;

    QComboBox * zoom_level;
    QIcon * start;
    QIcon * stop;

    bool forceConfig;
    bool doResize;
    int unit;
    int dma_channel;
    uint saveFileIndex;
    bool autoNumber;
    QString version;
	QString devtype;
    QString lastSavedFile;
    QFileInfo *lastConfig;
    QString PDVHOME;
    QString saveFilterString;
    QList<QFileInfo> saveFilterList;
    QStringList saveFilterStringList;
};

#endif // PDVSHOW_H
