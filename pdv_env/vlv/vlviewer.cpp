#include <math.h>
#include "edtinc.h"
#include "vlviewer.h"
#include "ui_vlviewer.h"

#include <QProcessEnvironment>
#include <QRegExp>
#include <QMessageBox>

#include "imagefilemgr.h"

#include "contrastwidget.h"
#include <assert.h>
#include <QDebug>

#ifdef WIN32
#define PROCESS_OWNS_SHAREDMEM 1
#define unlink(arg) _unlink(arg)
#define getpid() _getpid()
#endif


vlviewer::vlviewer(QWidget *parent) : QMainWindow(parent),
    sharedMemory(), ui(new Ui::mainWindow)
{
    int user_nBufs = -1;
    QString configFileName = "";
    QString saveFileName = "";

    ui->setupUi(this);

    capturing = 0;

    iconPath = ":/pdv_qtlib/Resources/";

    appSettings = NULL;
    camSelectDialog = NULL;
    devSelectDialog = NULL;
    devInfoBox = NULL;
    camSettingsDialog = NULL;
    displaySkipDialog = NULL;
    helpDialog = NULL;
    camSerialDialog = NULL;
    devInfoBox = NULL;
    saveDialog = NULL;
    saveDialogAutoNumberCheckBox = NULL;
    saveDialogLayout = NULL;
    contrastWidget = NULL;
    histoWidget = NULL;
    profileWidget = NULL;
    channel = NULL;
    pixelWidget = NULL;
    doResize = true;
    alreadyLocked = false;
    doQuit = 0;
    autoNumber = false;
    forceConfig = false;
    devtype = "pdv";

    unit = 0;
    dma_channel = 0;

    line_changed = false;
    box_changed = false;

    EdtImage  *pImage = NULL; // just so we know if (optional) load image happened/succeeded

    // Initialize icons for continuous capture on toolbar
    // so not recreated on change
    QString curPath = iconPath;
    start = new QIcon(curPath.append("start.png"));
    
    curPath = iconPath;
    stop = new QIcon(curPath.append("process-stop.png"));
    iconCleaner << start << stop;

    timer = new QTimer(this);
    cleaner = new QObjectCleanupHandler();

    createToolbar();
    createStatusBar();

    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(UpdateStatus()));

    QObject::connect(qApp,  SIGNAL(aboutToQuit()),
                     this, SLOT(on_actionExit_triggered()));

    m_image = new EdtImage;

    QCoreApplication *pApp = QCoreApplication::instance();

    // QProcessEnvironment sysenv = QProcessEnvironment::systemEnvironment();
    PDVHOME= QProcessEnvironment::systemEnvironment().value("PDVHOME");
    if (PDVHOME.isEmpty())
    {
#ifdef WIN32
    PDVHOME="c:/EDT/pdv";
#else
    #ifdef __APPLE__
        PDVHOME="/apps/EDTpdv";
    #else
        PDVHOME="/opt/EDTpdv";
    #endif
#endif
    }

    QStringList args = pApp->arguments();

    SetupSaveFilters();

    if (args.length() > 1)
    {

        int i;

        for (i=1;i<args.length();i++)
        {
            QString arg = args.at(i);
            char devname[256];

            if (arg[0] == QChar('-') || arg[0] == QChar('/') )
            {
                if (arg.toLower().startsWith("-pdv"))
                {	
                    char szUnit[256];

                    strcpy(szUnit, (const char *)arg.toLocal8Bit()+1);
                    unit = edt_parse_unit_channel(szUnit,
                            devname,
                            EDT_INTERFACE,
                            &dma_channel);
                }
                else if (arg.toLower().startsWith("-nb"))
                {	
                    bool ok;
                    user_nBufs = arg.mid(3).toInt(&ok);

                    if (!ok)
                        user_nBufs = -1;
                }
                else if (arg == "-u")
                {
                    i++;
                    if (i < args.length())
                    {
                        // arg = args.at(i);
                        unit = args.at(i).toInt();
                    }
                }
                else if (arg == "-c")
                {
                    i++;
                    if (i < args.length())
                    {
                        arg = args.at(i);
                        dma_channel = arg.toInt();
                    }
                }
                else if ((arg == "-i") || (arg == "--init"))
                {
                    if (i+1 < args.length() && !args.at(i+1).startsWith("-"))
                    {
                        configFileName = args.at(++i);
                    }
                    else forceConfig = true;
                }
                else if ((arg == "-f") || (arg == "--file"))
                {
                    i++;
                    if (i < args.length())
                    {
                        saveFileName = args.at(i);
                    }
                }
                else if ((arg == "-q") || (arg == "--quit"))
                {
                    doQuit = 1;
                }
                else if ((arg == "-v") ||(arg == "--version") ||(arg == "--about")) 
                {
                    on_actionAbout_vlviewer_triggered();
                    doQuit = 1;
                    return;
                }
                else if ((arg == "-h") || (arg == "--help"))
                {
                    usage("");
                    doQuit = 1;
                    return;
                }
                else
                {
                    usage(QString("unknown argument: ") + arg + QString("<br><br>"));
                    doQuit = 2;
                    return;
                }
                
            }
            else pImage = EdtImageFileMgr.LoadEdtImage(arg.toLocal8Bit(), m_image);
        }
    }

    if (lock())
    {
        QString tmpConfigFileName;

        if ((configFileName != "")
         && (configFileName[0] != QChar('/') && configFileName[0] != QChar('\\') && configFileName[0] != QChar('.')))
            tmpConfigFileName = "./" + configFileName;
        else
            tmpConfigFileName = configFileName;

        appSettings = new AppSettings(unit, dma_channel);

        lastSavedFile = "";

        if (!SetupLive(unit, dma_channel, tmpConfigFileName))
            return;

        if (user_nBufs >= 0)
            channel->GetCamera()->SetNBufs(user_nBufs) ;

        timer->start(10);

        doResize = true;
        if ((!pImage) && (channel->GetCamera()->GetWidth() > 0))
		{
			channel->GetCamera()->UpdateFromCamera();
            onCapture_One_Image();
		}
        if (!saveFileName.isEmpty())
            doSave(saveFileName, true);

        if (do_quit())
            return;
    }
}


bool vlviewer::SetupLive(const int unit, const int chan, QString initConfig)
{
    channel = new EdtQtLiveDisplay(devtype.toLocal8Bit(), unit, chan);

    if (channel->GetCamera()->GetDevice() == NULL)
    {

        QString str = devtype + QString::number(unit) + "_" + QString::number(chan) + " not found, or not a framegrabber";
        QMessageBox::critical(this, tr("Error"), str);
        doQuit = 2;
        close();
        qApp->quit();
        return false;
    }

    if (m_image->GetWidth() == 0)
    {
        if (channel->GetCamera()->GetWidth())
            channel->GetCamera()->SetImageToCamera(m_image);
        else
            m_image->Create(TypeBYTE,1024,1024);
    }

    viewer.Setup((EdtQtImageView *)(ui->ImageView));
    image_item = viewer.AddImageItem();
    channel->AddDisplayWindow(image_item);
    image_item->SetDataImage(m_image);
    image_item->UpdateDisplayImage();

    scene = (EdtQtImageScene * ) ui->ImageView->Scene();

    QObject::connect(scene, SIGNAL(MouseMoved(QPointF)),this,SLOT(onCursorMoved(QPointF)));
    QObject::connect(scene, SIGNAL(Selected(QLineF)),this,SLOT(onSelectedLine(QLineF)));
    QObject::connect(scene, SIGNAL(Selected(QRectF)),this,SLOT(onSelectedBox(QRectF)));
    QObject::connect(scene, SIGNAL(MousePressed(QPointF)),this,SLOT(onCursorMoved(QPointF)));

    if (!initConfig.isEmpty())
    {
        SetupSelected(QFileInfo(initConfig).path(), QFileInfo(initConfig).fileName());
    }

    if (forceConfig || (channel->GetCamera()->GetWidth() <= 0))
    {
        onSelect_Camera(true);
    }

    setTitle();
    initButtons();

    return true;

}

vlviewer::~vlviewer()
{

    delete ui;
    delete timer;

    if (devSelectDialog)
        delete devSelectDialog;

    if (camSelectDialog)
        delete camSelectDialog;

    if (camSerialDialog)
        delete camSerialDialog;

    if (helpDialog)
        delete helpDialog;

    if (displaySkipDialog)
        delete displaySkipDialog;

    if (contrastWidget)
        delete contrastWidget;

    if (histoWidget)
        delete histoWidget;

    if (profileWidget)
        delete profileWidget;

   for (int i=0; i<iconCleaner.size(); ++i)
   {
       delete iconCleaner.at(i);
   }

   detach();

}

QAction *vlviewer::addActionToToolBar(const QString & file, const QString & text)
{
    QAction *newAction;
    QString path = iconPath;
    QIcon *newIcon = new QIcon(path.append(file));
    newAction = ui->mainToolBar->addAction(*newIcon, text);
    iconCleaner << newIcon;

    return newAction;
}

#define ACTION_F(name) SLOT(on##name())
#define ACTION_NAME(name) action##name

#define setMenuIcon(name, iconname) \
{ \
    QString path = iconPath; \
    ui->ACTION_NAME(name)->setIcon(QIcon(path.append(iconname))); \
}

#define setActionWithIcon(name, iconname) \
{ \
    QString path = iconPath; \
    ui->ACTION_NAME(name)->setIcon(QIcon(path.append(iconname))); \
    ui->mainToolBar->addAction(ui->ACTION_NAME(name)); \
    QObject::connect(ui->ACTION_NAME(name), SIGNAL(triggered()), this, ACTION_F(name));  \
}

#define setActionState(name, state) \
{ \
    ui->ACTION_NAME(name)->setEnabled(state); \
}

// The tool bar actions are assumed to have been created in the UI editor,
// so they all have menu equivalents - the action name will be
// action<name>, the slot will be on<name>
// 


/**
 * @fn  int vlviewer::createToolbar()
 *
 * @brief   Creates the toolbar
 *
 * @return  .
**/

int vlviewer::createToolbar()
{

    QDir::setSearchPaths("icons",QStringList(":/qtdir/Resources"));

    QString s = "ACTION_F(name)";

    // these just get the menu associated with the icon (no toolbar item)
    // setMenuIcon(Hardware_invert,"invert.png");
    // setMenuIcon(First_pixel_counter,"pixel_counter.png");

    // These are not yet implemented (and may never be), so...
    // setActionWithIcon(NewImage,"filenew.png");
    // setActionWithIcon(OpenImage,"imagenew.png");
    ui->actionNewImage->setVisible(false);
    ui->actionOpenImage->setVisible(false);
    ui->actionClose->setVisible(false); 
    ui->actionCamera_File_Capture->setVisible(false);
    ui->actionRecent_FIle->setVisible(false);  
    ui->actionRefresh->setVisible(false); 
    ui->actionTime_Display->setVisible(false); 
    ui->actionZoom_Panel->setVisible(false); 
    ui->actionPan->setVisible(false); 

    ui->actionBayer->setEnabled(false); 

    setActionWithIcon(SaveImage,"saveimg.png");

    ui->mainToolBar->addSeparator();

    setActionWithIcon(Device_info,"board_icon.png");
    setActionWithIcon(Select_Device,"devselect.png");
    setActionState(Select_Device, false); // this one is yet to be implemented fully 
    setActionWithIcon(Select_Camera,"camselect.png");
    setActionWithIcon(Camera_Settings,"fstop.png");
    setActionWithIcon(Camera_Serial,"serial1.png");


    ui->mainToolBar->addSeparator();
    setActionWithIcon(Capture_One_Image,"single_shot.png");
    setActionWithIcon(Continuous_Capture,"start.png");
    ui->mainToolBar->addSeparator();

    setActionWithIcon(Pixel_Text,"pixel_text.png");
    setActionWithIcon(Histogram,"histogram.png");
    setActionWithIcon(Profile,"profile.png");

     ui->mainToolBar->addSeparator();

     setActionWithIcon(Contrast,"contrast.png");
    setActionWithIcon(Bayer,"colors.png");

    //setActionWithIcon(Set_ROI,"ROI.png");
    ui->actionSet_ROI->setVisible(false);   // DISABLED for now
     ui->mainToolBar->addSeparator();

    QActionGroup *cursor_group = new QActionGroup(ui->mainToolBar);

    setActionWithIcon(Reset_Selection,"reset_cursor.png");
    setActionWithIcon(Select_Line,"draw_line.png");
    setActionWithIcon(Select_Region,"draw_box.png");

    cursor_group->addAction(ui->actionReset_Selection);
    cursor_group->addAction(ui->actionSelect_Line);
    cursor_group->addAction(ui->actionSelect_Region);
    ui->actionReset_Selection->setCheckable(true);
    ui->actionSelect_Line->setCheckable(true);
    ui->actionSelect_Region->setCheckable(true);
    ui->actionReset_Selection->setChecked(true);
    ui->mainToolBar->addSeparator();

    setActionWithIcon(Zoom_In,"zoom.png"); 
    setActionWithIcon(Zoom_Out,"zoom_out.png");
    setActionWithIcon(Normal_Size,"zoom_normal.png");

    zoom_level = new QComboBox();
    zoom_level->addItem("100%", 1);
    zoom_level->addItem("1600%", 16);
    zoom_level->addItem("800%", 8);
    zoom_level->addItem("400%", 4);
    zoom_level->addItem("200%", 2);
    zoom_level->addItem("50%", .5);
    zoom_level->addItem("25%", .25);
    zoom_level->insertSeparator(5);

    ui->mainToolBar->addWidget(new QLabel("Zoom"));

    ui->mainToolBar->addWidget(zoom_level);

    connect(zoom_level, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zoom_level_changed(int)));

    return  1;
}

void vlviewer::setButtonsConfigured(bool state)
{
    setActionState(Camera_Settings, state);
    setActionState(Camera_Serial, state);

    setActionState(Capture_One_Image, state);
    setActionState(Continuous_Capture, state);

    setActionState(Set_ROI, state);
}


void vlviewer::createStatusBar()
{
    statusFCountLabel = new QLabel("Frame count:");
    statusFCountEdit = new QLineEdit();
    statusFCountEdit->setReadOnly(true);

    statusFPSLabel = new QLabel("FPS:");
    statusFPSEdit = new QLineEdit();
    statusFPSEdit->setReadOnly(true);

    statusTimeoutsLabel = new QLabel("Timeouts:");
    statusTimeoutsEdit = new QLineEdit();
    statusTimeoutsEdit->setReadOnly(true);
    statusTimeoutsEdit->setToolTip("Timeouts indicate image data is missing");

    statusXYLabel = new QLabel("  X,Y:");
    statusXYEdit = new QLineEdit();
    statusXYEdit->setReadOnly(true);

    statusPixLabel = new QLabel("Pixel:");
    statusPixEdit = new QLineEdit();
    statusPixEdit->setReadOnly(true);

    statusSelectionLabel = new QLabel("Selection:");
    statusSelectionEdit = new QLineEdit();
    statusSelectionEdit->setReadOnly(true);

    statusSkippedLabel = new QLabel("Skipped:");
    statusSkippedEdit = new QLineEdit();
    statusSkippedEdit->setReadOnly(true);

    ui->statusBar->addWidget(statusFCountLabel);
    ui->statusBar->addWidget(statusFCountEdit);
    ui->statusBar->addWidget(statusFPSLabel);
    ui->statusBar->addWidget(statusFPSEdit);
    ui->statusBar->addWidget(statusTimeoutsLabel);
    ui->statusBar->addWidget(statusTimeoutsEdit);
    ui->statusBar->addWidget(statusXYLabel);
    ui->statusBar->addWidget(statusXYEdit);
    ui->statusBar->addWidget(statusPixLabel);
    ui->statusBar->addWidget(statusPixEdit);
    ui->statusBar->addWidget(statusSelectionLabel);
    ui->statusBar->addWidget(statusSelectionEdit);
    ui->statusBar->addWidget(statusSkippedLabel);
    ui->statusBar->addWidget(statusSkippedEdit);

}


void vlviewer::on_actionExit_triggered()
{
    close();
    qApp->quit();
}

void vlviewer::on_actionTool_Bar_changed()
{
    if (!ui->actionTool_Bar->isChecked())
        ui->mainToolBar->hide();

    else
        ui->mainToolBar->show();
}


void vlviewer::on_actionHardware_invert_changed()
{
    bool inv = ui->actionHardware_invert->isChecked();
    if (channel)
        channel->GetCamera()->SetHardwareInvert(inv);
}

void vlviewer::on_actionFirst_pixel_counter_changed()
{
    bool inv = ui->actionFirst_pixel_counter->isChecked();
    if (channel)
        channel->GetCamera()->SetFirstPixelCounter(inv);
}

void vlviewer::onNewImage()
{

}

void vlviewer::onOpenImage()
{

}

void vlviewer::onSaveImage()
{

}

void vlviewer::onDevice_info()
{

}

void vlviewer::onSelect_Device()
{

}

void vlviewer::on_actionAbout_vlviewer_triggered()
{
    QString ver = "<H2>VLviewer v. " + QString(EDT_LIBRARY_VERSION) + "</h2>";

    QMessageBox::about(this, tr("About VLviewer"),
            ver + 
            "<p>Copyright &copy; 2013-2015 EDT, Inc." +
            "<p>Image capture / display / save application for EDT VisionLink<br>" +
            "and PDV series Camera Link and AIA frame grabbers.");
}

void vlviewer::on_actionDevice_info_triggered()
{
    if (devInfoBox)
        delete devInfoBox;
    devInfoBox = new DevInfoMsgBox(channel->GetCamera(), this);
}

void vlviewer::on_actionSaveImageAs_triggered()
{
    QString selectedFilter;

    if (image_item->ActiveImage()->GetData() == NULL)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No image to save!\t\t"));
        return;
    }

    QString gotSaveFileName;

    if (!saveDialog)
    {
        saveDialog = new QFileDialog(this, Qt::Dialog);
        saveDialog->setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::DontConfirmOverwrite);
        saveDialogAutoNumberCheckBox = new QCheckBox(tr("Autonumber"), saveDialog);
        saveDialogLayout = (QGridLayout *)saveDialog->layout();
        saveDialog->setLabelText(QFileDialog::Accept, "Save");
        saveDialog->setNameFilters(saveFilterStringList);

        saveDialogLayout->addWidget(saveDialogAutoNumberCheckBox, 4, 0);

        connect(saveDialogAutoNumberCheckBox, SIGNAL(stateChanged(int)),
                this, SLOT(onSaveDialogAutoNumberChanged(int)));

        connect(saveDialog, SIGNAL(filterSelected(QString)),
                this, SLOT(onSaveDialogFilterChanged()));
    }

    saveDialogAutoNumberCheckBox->setCheckState(autoNumber?Qt::Checked:Qt::Unchecked);
    QString file = getNextFileName(lastSavedFile, selectedFilter);
    saveDialog->setDirectory(file);
    saveDialog->selectFile(file);
    // saveDialog->setNameFilter(selectedFilter);

    if (saveDialog->exec())
        gotSaveFileName = saveDialog->selectedFiles().at(0);

#if 0 // another way

    gotSaveFileName = saveDialog->getSaveFileName(this, tr("Save Image"),
        getNextFileName(lastSavedFile, selectedFilter), saveFilterString, &selectedFilter, QFileDialog::DontUseNativeDialog | QFileDialog::DontConfirmOverwrite);
#endif

#if 0 // old
    QString gotSaveFileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
        getNextFileName(lastSavedFile, selectedFilter), saveFilterString, &selectedFilter, QFileDialog::DontConfirmOverwrite);
#endif

    if (!gotSaveFileName.isEmpty())
    {
        doSave(gotSaveFileName /*, getSaveFilterType(selectedFilter) */);
    }
}


void vlviewer::on_actionSaveImage_triggered()
{
    if (lastSavedFile.isEmpty())
    {
        on_actionSaveImageAs_triggered();
    }
    else doSave(getNextFileName(lastSavedFile));
}

bool vlviewer::doSave(QString qsPathname, bool forceOverwrite /* QString type */)
{
	bool rc = false;

    if (qsPathname.isEmpty())
    {
        QMessageBox::warning(this, tr("Debug"), "No file name provided!");
        return false;
    }

    EdtQtFileInfo fileInfo(qsPathname);

    // if (ui->actionAutoNumber->isChecked() && !fileInfo.hasNumber())
    if (autoNumber && !fileInfo.hasNumber())
        qsPathname = fileInfo.incremented(1, 3);

    fileInfo.setFile(qsPathname);

    if (fileInfo.exists() && !forceOverwrite)
    {
        QString msg = fileInfo.fileName() + tr(" already exists.\nDo you want to replace it?\t\t");

        if (QMessageBox::warning(this, tr("Confirm Save As"), msg,
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)

            return false;
    }

    if (image_item && image_item->ActiveImage())
    {
        char imagetype[MAX_PATH];
        QByteArray ba = qsPathname.toLocal8Bit();
        const char *pathname = ba.data();
        PdvInput * camera = channel->GetCamera();

        getFileSuffix(qsPathname,imagetype,MAX_PATH-1);

        if (camera->IsCapturingImages())
        {
            EdtImage *pImage = channel->GetLastImage();

            if (pImage && (pImage != image_item->ActiveImage()))
            {
                image_item->ActiveImage()->Copy(pImage);
            }
        }

        EdtImageFileMgr.SetPureRaw(TRUE);

        rc = (EdtImageFileMgr.SaveImage(pathname, image_item->ActiveImage(), imagetype) == 0);
    }

    if (rc)
        lastSavedFile = qsPathname;

	return rc;
}

// given a pathname, return the suffix (filetype) as a string in imagetype,
// and return value as QString
QString vlviewer::getFileSuffix(QString qsPathname, char *cImagetype, int maxlen)
{
    QFileInfo fileInfo(qsPathname);
    QString qsImagetype = fileInfo.suffix();
    QByteArray ba = qsImagetype.toLocal8Bit();
    const char *ca = ba.data();
    strncpy(cImagetype, ca, maxlen);

    return qsImagetype;
}

QString vlviewer::getNextFileName(const QString fname, int increment)
{
    QString dmy = "";
    return getNextFileName(fname, dmy, increment);
}

QString vlviewer::getNextFileName(const QString lastFname, QString &filter, int increment)
{
    QString newName = PDVHOME + "/" + devtype + "image.tif";

    if (!lastFname.isEmpty())
        newName = lastFname;
    
    if (filter.isEmpty())
    {
        if (EdtQtFileInfo(newName).suffix().isEmpty())
            filter = "tif";
        else filter = getSaveFilterString(QFileInfo(newName).suffix().toLower());
    }

    if (autoNumber)
    {
        EdtQtFileInfo fileInfo(newName);
        newName = fileInfo.incremented(increment, 3);
    }

    return newName;
}


void vlviewer::on_actionSelect_Device_triggered()
{
    if (devSelectDialog)
        delete devSelectDialog;
    devSelectDialog = new EdtQtDeviceDlg();
    connect(devSelectDialog,SIGNAL(accept(const QString &, const int &, const int &)),
        this, SLOT(SetupDevice(const QString &, const int &, const int &)));
    devSelectDialog->show();
}

void vlviewer::on_actionCamera_Settings_triggered()
{
    if (camSettingsDialog)
        delete camSettingsDialog;
    camSettingsDialog = new CamSettingsDlg(channel->GetCamera());
    camSettingsDialog->show();
}

void vlviewer::on_actionCamera_Serial_triggered()
{
    if (!camSerialDialog)
    {
        camSerialDialog = new CamSerialDlg(channel->GetCamera());
    }
    camSerialDialog->show();
}

void vlviewer::on_actionVlviewer_Help_triggered()
{
    if (!helpDialog)
    {
        helpDialog = new HelpDlg(":/pdv_qtlib/Resources/vlvhelp.html");
    }
    helpDialog->show();
}

void vlviewer::on_actionDisplay_Skip_triggered()
{
    if (channel)
    {
        if (!displaySkipDialog)
        {
            displaySkipDialog = new DisplaySkip(channel);
        }
        displaySkipDialog->show();
    }
}

void vlviewer::on_actionIncrease_Exposure_Time_triggered()
{
    PdvInput *pCamera = channel->GetCamera();

    // Get the current value
    int nShutterSpeed;
    if (!pCamera->GetShutterSpeed(nShutterSpeed))
    {
        return;
    }

    // Validate the shutter speed value
    int low, high;
    pCamera->GetMinMaxShutter(low, high);

    if (++nShutterSpeed > high)
    {    
        nShutterSpeed = high;
    }

    pCamera->SetShutterSpeed(nShutterSpeed);
}

void vlviewer::on_actionDecrease_Exposure_Time_triggered()
{
    PdvInput *pCamera = channel->GetCamera();

    // Get the current value
    int nShutterSpeed;
    if (!pCamera->GetShutterSpeed(nShutterSpeed))
    {    
        return ;
    }

    int low, high;
    pCamera->GetMinMaxShutter(low, high);

    // Validate the shutter speed value
    if (--nShutterSpeed < low)
    {
        nShutterSpeed = low;
    }

    pCamera->SetShutterSpeed(nShutterSpeed);
    
}

void vlviewer::onSelect_Camera(bool blocking)
{
    lastConfig = appSettings->getConfigInfo();
    bool wasLive = false;

    if (camSelectDialog)
        delete camSelectDialog;

    doResize = true;

    if (capturing)
    {
        onContinuous_Capture();
        wasLive = true;
    }

    camSelectDialog = new QtCamSelect(true);
    if (!lastConfig->fileName().isEmpty())
        camSelectDialog->setSelected(lastConfig);
    connect(camSelectDialog,SIGNAL(ConfigSelected(const QString &, const QString &)),
        this, SLOT(SetupSelected(const QString &, const QString &)));

    camSelectDialog->resize(415, 525);

    if (blocking) // at startup at least
        camSelectDialog->exec();
    else camSelectDialog->show();

    if (channel->GetCamera()->GetWidth() > 0)
        setButtonsConfigured(true);
    else setButtonsConfigured(false);

    setTitle();

    if (wasLive)
    {
        onContinuous_Capture();
    }

}

void vlviewer::onCamera_Settings()
{

}

void vlviewer::onCamera_Serial()
{

}

void vlviewer::onCapture_One_Image()
{
    if (capturing)
    {
        Stop();
        capturing = 0;
        ui->actionContinuous_Capture->setIcon(*start);
        ui->actionContinuous_Capture->setText("Start Continuous Capture");
    }
    else
    {
        channel->capture_single_image(doResize);
        setStatusTimeouts(channel->GetTimeouts());
        image_item->UpdateDisplayImage(doResize);
    }

    doResize = false;
}

void vlviewer::onContinuous_Capture()
{
    if (!capturing)
    {
        channel->Start();
        capturing = 1;
        ui->actionContinuous_Capture->setIcon(*stop);
        ui->actionContinuous_Capture->setText("Stop Continuous Capture");
    }

    else
    {
        Stop();
        capturing = 0;
        ui->actionContinuous_Capture->setIcon(*start);
        ui->actionContinuous_Capture->setText("Start Continuous Capture");
    }
}

void vlviewer::onPixel_Text()

{
    if (pixelWidget)
    {
        if (pixelWidget->isVisible())
        {
            pixelWidget->hide();
            return;
        }
    
        pixelWidget->update_display();

    }
    else
    {
        pixelWidget = new EdtQtPixelDlg(image_item, this);
    }

    pixelWidget->show();
}

void vlviewer::onContrast()
{
    if (contrastWidget)
    {
        if (contrastWidget->isVisible())
        {
            contrastWidget->hide();
            return;
        }
    
        contrastWidget->update_display();

    }
    else
    {

        image_item->SetTransformActive();


        contrastWidget = new ContrastWidget(image_item, this);

        EdtQtContrastCanvas * canvas = contrastWidget->canvas();

        connect(canvas, SIGNAL(lutChanged(EdtLut *)),
           image_item,SLOT(setLut(EdtLut *)));

    }

    contrastWidget->show();
}

void vlviewer::onHistogram()
{
    if (histoWidget)
    {
        if (histoWidget->isVisible())
        {
            histoWidget->hide();
            return;
        }
    
        histoWidget->update_display();
    }
    else
    {
        histoWidget = new HistoWidget(image_item, this);
    }

    histoWidget->show();
}

void vlviewer::onProfile()
{
    QLineF *lp = NULL;

    if (profileWidget)
    {
        if (profileWidget->isVisible())
        {
            profileWidget->hide();
            return;
        }

        profileWidget->setImage(image_item->ActiveImage());
        if (scene->SelectState() == EdtQtImageScene::LineSelect)
        {
            profileWidget->setLine(selected_line);
        }

        profileWidget->update_display();
    }
    else
    {
        if (scene->SelectState() == EdtQtImageScene::LineSelect)
        {
            lp = &selected_line;
        }
        profileWidget = new ProfileWidget(image_item, lp, this);
    }

    profileWidget->show();
}

void vlviewer::onBayer()

{

}

void vlviewer::onSet_ROI()

{

}


void vlviewer::onReset_Selection()

{
    scene->SetSelectState(EdtQtImageScene::NoSelect);
    has_line = false;
    has_box = false;
    image_item->ClearRegion();
}

void vlviewer::onSelect_Line()

{
    scene->SetSelectState(EdtQtImageScene::LineSelect);

}

void vlviewer::onSelect_Region()

{
    scene->SetSelectState(EdtQtImageScene::RectSelect);

}


void vlviewer::onZoom_In()

{
    viewer.Scale(1.414);
}

void vlviewer::onZoom_Out()

{
    viewer.Scale((1.0/1.414));
}

void vlviewer::onNormal_Size()

{
    viewer.SetZoom(1.0);
}

void vlviewer::UpdateStatus()
{
    QString pixel;
    QPoint pt;


    viewer.GetCursorValue(pt, pixel);
    QString xy = QString::number(pt.x());
    xy.append(", ");
    xy.append(QString::number(pt.y()));
    //statusXYEdit->setText(xy);
    statusPixEdit->setText(pixel);
    update_point();

    if (channel && channel->IsLive())
    {
        int nFrames = channel->GetFrameCount();
        int frameRate = (int)channel->GetFrameRate();
        int nSkipped = channel->GetCamera()->GetSkipped();
        int nTimeouts = channel->GetTimeouts();

        statusFPSEdit->setText(QString::number(frameRate));
        statusFCountEdit->setText(QString::number(nFrames));
        setStatusTimeouts(nTimeouts);

        QString sskipped;

        sskipped.sprintf("%4d %7.0f %%", nSkipped, (nFrames)? ((double) nSkipped  / ((double)nFrames + nSkipped)) * 100.0 : 0.0);

        statusSkippedEdit->setText(sskipped);

    }

    if (viewer.UpdateNeeded())
    {
        viewer.Refresh();

        update_box();

        update_line();
        

    }
}

void vlviewer::setStatusTimeouts(int nTimeouts)
{
    if (nTimeouts)
    {
        statusTimeoutsEdit->setStyleSheet("color: #ff0000; background-color: #ffdddd; font-weight:bold;");
    }
    else
    {
        statusTimeoutsEdit->setStyleSheet("color: #000000; background-color: #ffffff; font-weight: normal;");
    }
    statusTimeoutsEdit->setText(QString::number(nTimeouts));
}

void vlviewer::Stop()
{
    channel->Stop();
}

void vlviewer::SetupDevice(const QString &dev, const int &u, const int &c)
{
    QString qs = "SetupDevice: dev " + dev + " unit " + QString::number(u) + " chan " + QString::number(c);
    
    QMessageBox::information(this, tr("Debug"), qs);
}

void vlviewer::SetupSelected(const QString &cfg_path, const QString &cfg_name)

{
    PdvInput * camera;

    if (channel)
    {
        QString work_path = cfg_path;
        QString work_name = cfg_name;

        if (!(work_path.endsWith("/") || work_path.endsWith("\\")))
            work_path.append('/');
        appSettings->setConfigFile(work_path + work_name);


        if (capturing)
        {
            onContinuous_Capture();
        }

        camera = channel->GetCamera();

        // deal with camera_config

        int i= work_path.indexOf("camera_config");

        if (i != -1)
        {
            work_name.insert(0, QString("camera_config/"));
            work_path.truncate(i);
        }

        if (!camera->Setup(work_path.toAscii(), work_name.toAscii()))
        {
            QMessageBox::critical(this, tr("Error"), QString::fromStdString(camera->lastError()));
            return;
        }

        camera->SetImageToCamera(m_image);
		camera->UpdateFromCamera();

        onCapture_One_Image();

    }
}

void vlviewer::zoom_level_changed(int index)

{
    double scale = zoom_level->itemData(index).toDouble();

    viewer.SetZoom(scale);
}

void vlviewer::onCursorMoved(QPointF pt)

{

    QString xy = QString::number(pt.x());
    xy.append(", ");
    xy.append(QString::number(pt.y()));
    statusXYEdit->setText(xy);

    last_point = pt;

    if (!scene->mouseReleased())
        update_point();

}

void vlviewer::onSelectedLine(QLineF line)

{

    QString xy = QString::number(line.x1());
    xy.append(", ");
    xy.append(QString::number(line.y1()));
    xy.append(" -> ");
    xy.append(QString::number(line.x2()));
    xy.append(", ");
    xy.append(QString::number(line.y2()));
    statusSelectionEdit->setText(xy);

    if (line.length() > 2)
    {
        selected_line = line;
        has_line = true;
    
        update_line();
    }
}

void vlviewer::onSelectedBox(QRectF box)

{
    QString xy = QString::number(box.topLeft().x());
    xy.append(", ");
    xy.append(QString::number(box.topLeft().y()));
    xy.append(" -> ");
    xy.append(QString::number(box.bottomRight().x()));
    xy.append(", ");
    xy.append(QString::number(box.bottomRight().y()));
    statusSelectionEdit->setText(xy);

    if (box.width() >= 1 && box.height() >= 1)
    {
        selected_box = box;

        has_box = true;

        image_item->SetRegion(box);

        update_box();
    }

}

void vlviewer::update_line()

{
    if (profileWidget && profileWidget->isVisible() && has_line)
    {
        profileWidget->setImage(image_item->ActiveImage());
        profileWidget->setLine(selected_line);
        profileWidget->update_display();
    }
}

void vlviewer::update_box()

{
    if (histoWidget && histoWidget->isVisible())
    {
        histoWidget->setImage(image_item->ActiveImage());
    }
    if (contrastWidget && contrastWidget->isVisible())
    {
        contrastWidget->setImage(image_item->ActiveImage());
    }
}

void vlviewer::update_point()

{
    if (pixelWidget && pixelWidget->isVisible())
    {
        pixelWidget->SetImagePoint(image_item->ActiveImage(), last_point);
    }
}

// setup possible save formats; use QFile as a convenient way to access title and suffix
void vlviewer::SetupSaveFilters()
{
    saveFilterList << QFileInfo("Tiff.tif")
                   << QFileInfo("Bitmap.bmp")
                   << QFileInfo("PNG.png")
                   << QFileInfo("JPEG.jpg")
                   << QFileInfo("FITS.fts")
                   << QFileInfo("Raw.raw");

    saveFilterString = "";
    for (int i=0; i<saveFilterList.size(); i++)
    {
        if (canSave(saveFilterList.at(i).suffix()))
        {
            saveFilterString += SaveFilter(i) + ";;";
            saveFilterStringList += SaveFilter(i);
        }
        else
        {
            saveFilterList.removeAt(i--);
        }
    }
}

const QString vlviewer::SaveFilter(int i)
{
    return saveFilterList.at(i).baseName() + " (*." + saveFilterList.at(i).suffix() + ")";
}


// check if format is supported; allow filenames or just extensions
bool vlviewer::canSave(const QString qsFmt)
{
    QByteArray ba;

    if (qsFmt.size() < 4)
        ba = qsFmt.right(3).toLocal8Bit();
    else if (qsFmt.at(qsFmt.size()-4) == '.')
        ba = qsFmt.right(3).toLocal8Bit();
    else return false;

    const char *fmt = ba.data();

    return EdtImageFileMgr.CanSave(fmt);
}

const QString vlviewer::getSaveFilterString(const QString qsFmt)
{
    for (int i=0; i<saveFilterList.size(); i++)
    {
        if (qsFmt == saveFilterList.at(i).suffix())
            return SaveFilter(i);
    }

    return SaveFilter(0);
}

const QString vlviewer::getSaveFilterType(const QString filter)
{
    QString qs = filter.right(4);
    return qs.left(3);
}

void vlviewer::detach()
{

#ifdef PROCESS_OWNS_SHAREDMEM
    if (!sharedMemory.detach())
    {
        ; // noop
    }
#else
    if (lockFile.size())
    {
        unlink(lockFile.toStdString().c_str());
    }
#endif
    
}


// Windows use QsharedMemory
// Linux etc. use a file lock (see QSharedMemory for why -- hint, what happens if app crashes)
// If not locked then lock it and return true, otherwise return false (also can get whether
// it was already locked (inverse of this return) via already_running();
//
bool
vlviewer::lock()
{
    sharedKey = devtype + QString::number(unit) + "_" + QString::number(dma_channel) + ".lck";
    lockFile = "/tmp/" + sharedKey;

#ifdef PROCESS_OWNS_SHAREDMEM
    // In windows the process owns the shared memory so it will be released if the app crashes which is what we want.
    sharedMemory.setKey(sharedKey);
    if (!sharedMemory.create(1))
    {
        QString msg = "<b>vlviewer -" + QFileInfo(sharedKey).baseName() + " is already running.</b>";

        QMessageBox::warning(this, tr("Alert"), msg);
        alreadyLocked = true;
        doQuit = 2;
    }
#else
    // In non-windows the OS owns the shared mem qnd will remain after a crash. So use a lockFile instead

    QFileInfo fi(lockFile);
    QFileInfo fd(fi.path());

    if (fi.exists())
    {
        QString msg = "<b>vlviewer -" + QFileInfo(sharedKey).baseName() + " is already running</b><br><br>...or an earlier instance crashed. If there is no other instance running, check for the existence of <code>" + lockFile + "</code> and delete it.";
        QMessageBox::warning(this, tr("Alert"), msg);
        lockFile = ""; // so detatch doesn't delete it
        alreadyLocked = true;
        doQuit = 2;
    }
    else if (fd.isDir() && fd.isWritable())
    {
        // write the process id to the file ;
        FILE *fp = fopen(lockFile.toStdString().c_str(), "w");

        if (fp == NULL)
            QMessageBox::warning(this, tr("Alert"), "Error creating lockFile.");
        else
        {
            QString s_pid = QString::number(getpid());

            fwrite(s_pid.toStdString().c_str(), 1, s_pid.size(), fp);
            fclose(fp);
        }
    }
#endif

    return !alreadyLocked;

}

void vlviewer::setTitle()
{
    QString title = QString(channel->GetCamera()->GetDevIdStr())
    + " | " + devtype
    + QString::number(channel->GetCamera()->GetUnitNumber())
    + "_"
    + QString::number(channel->GetCamera()->GetChannelNumber())
    + " | "
    + channel->GetCamera()->GetLoadedConfig()
    + " ("
    + QString::number(channel->GetCamera()->GetWidth())
    + "x"
    + QString::number(channel->GetCamera()->GetHeight())
    + "x"
    + QString::number(channel->GetCamera()->GetDepth())
    + ") "
    + " - vlviewer";

    this->setWindowTitle(title);
}

void vlviewer::initButtons()
{
    if (channel)
    {
        int d = channel->GetCamera()->GetDepth();
        int e = channel->GetCamera()->GetExtendedDepth();
        int bayer = false;

        if (((e == 8) || (e == 10)) && (d == 24))
            bayer = true;

        ui->actionBayer->setEnabled(bayer);
        ui->actionFirst_pixel_counter->setChecked(channel->GetCamera()->GetFirstPixelCounter());
        ui->actionHardware_invert->setChecked(channel->GetCamera()->GetHardwareInvert());
    }
}

void vlviewer::onSaveDialogAutoNumberChanged(int state)
{
    QString selectedFilter;

    autoNumber = state;
    ui->actionAutoNumber->setChecked(state);
    saveDialogAutoNumberCheckBox->setCheckState(autoNumber?Qt::Checked:Qt::Unchecked);
    saveDialog->selectFile(getNextFileName(saveDialog->selectedFiles().at(0), selectedFilter, 0));
}

void vlviewer::on_actionAutoNumber_changed()
{
    autoNumber = ui->actionAutoNumber->isChecked();
}

void vlviewer::onSaveDialogFilterChanged()
{
    saveDialog->selectFile(EdtQtFileInfo(saveDialog->selectedFiles().at(0)).modSuffix(saveDialog->selectedNameFilter(), saveFilterList));
}

void
vlviewer::usage(QString msg)
{
    QMessageBox::about(this, tr("VlViewer"),
        msg +
        "<b>usage:</b> vlviewer <em>args</em>, where <em>args</em> can be a combination of</td>" +
        "<br>" +
        "<table>" +
        "<tr><td>-u <em>unit</em></td><td>specify the unit (board) number (default 0)</td></tr>" +
        "<tr><td>-c <em>chan</em></td><td>specify the channel number (default 0)</td></tr>" +
        "<tr><td>-pdv<em>U_C</em></td><td>alternate way to specify unit and channel; <em>U</em> = unit, <em>C</em> = channel</td></tr>" +
        "<tr><td>-nb<em>X</em></td><td><em>X</em>=number of buffers (default will be anywhere between 4 and 64, depending on the image size)</td></tr>" +
        "<tr><td>-i, --init [<em>config_path</em>]&nbsp;</td><td>initialize with the given config file (relative or absolute path); if no config_path argument provided, causes the config dialog to be displayed on open</td></tr>" +
        "<tr><td>-f, --file <em>filename</em>&nbsp;</td><td>capture one image and save it, then quit; format determined by the extension (.raw, .tif, .jpg, .bmp.)</td></tr>" +
        "<tr><td>-v, --version &nbsp;&nbsp;</td><td>show the version message box and exit on OK</td></tr>" +
        "<tr><td>-h, --help</td><td>show this message box and exit on OK</td></tr>" +
        "<tr><td>-q, --quit</td><td>quit after performing any operations specified by other args (typically used with -f, -i)</td></tr>" +
        "</table>");
}


void vlviewer::closeEvent(QCloseEvent *evt)
{
    Q_UNUSED(evt);
    foreach (QWidget *widget, QApplication::topLevelWidgets())
        widget->close();
}
