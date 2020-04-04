#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QFileInfo>

class AppSettings
{

public:
    AppSettings(int unit, int channel, QString edt_interface="pdv");
    ~AppSettings();
    const QString getConfigFile();
    void setConfigFile(QString fullpath);
    const QString getConfigFileName();
    void setConfigFileDir(QString qs_value);
    const QString getConfigFileDir();
    QFileInfo *getConfigInfo();


private:
    QString device;
    QSettings *settings;
    QFileInfo *configFileInfo;
};

#endif //APPSETTINGS_H 
