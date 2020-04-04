#include "AppSettings.h"
#include <QFileInfo>


AppSettings::AppSettings(int unit, int channel, QString edt_interface)
{
    device = edt_interface + QString::number(unit) + "_" + QString::number(channel);
    settings = new QSettings("EDT", "vlviewer ");
    configFileInfo = new QFileInfo;
}

AppSettings::~AppSettings()
{
    delete(configFileInfo);
    delete(settings);
}

void AppSettings::setConfigFile(QString fullpath)
{
    configFileInfo->setFile(fullpath);

    settings->beginGroup(device);
      settings->beginGroup("config");
        settings->setValue("filename", configFileInfo->fileName());
        settings->setValue("directory", configFileInfo->path());
      settings->endGroup();
    settings->endGroup();
}

const QString AppSettings::getConfigFileName()
{
    return settings->value(device + "/config/filename").toString();
}

void AppSettings::setConfigFileDir(QString dir)
{
    settings->setValue(device + "/config/directory", dir);
}

const QString AppSettings::getConfigFileDir()
{
    return settings->value(device + "/config/directory").toString();
}

const QString AppSettings::getConfigFile()
{
    return settings->value(device + "/config/directory").toString() + "/" + settings->value(device + "/config/filename").toString();
}

QFileInfo *AppSettings::getConfigInfo()
{
    configFileInfo->setFile(getConfigFile());
    return configFileInfo;
}

