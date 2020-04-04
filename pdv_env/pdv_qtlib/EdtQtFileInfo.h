
// File Info for EDT -- inherits QFileInfo, adds methods for auto-numbering 
// the filename
//
// Copyright (c) 2015, EDT, Inc.
//
#ifndef EDTQTFILEINFO_H
#define EDTQTFILEINFO_H

#include <QFileInfo>


class EdtQtFileInfo : public QFileInfo
{

public:
    EdtQtFileInfo(QString file)  {setFile(file);}
    virtual ~EdtQtFileInfo() {}

    QString sansNumber();
    QString baseNameSansNumber();
    QString modSuffix(QString suffix, QList<QFileInfo> list);
    uint number();
    QString incremented(int incr=1, int pad=3);
    bool hasNumber();

};


#endif // EDTQTFILEINFO_H

