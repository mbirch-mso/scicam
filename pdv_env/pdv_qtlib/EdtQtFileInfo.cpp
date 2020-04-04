
// File Info for EDT -- inherits QFileInfo, adds methods for auto-numbering 
// the filename
//
// Copyright (c) 2015, EDT, Inc.
//
#include "EdtQtFileInfo.h"


QString EdtQtFileInfo::sansNumber()
{
    return path() + "/" + baseNameSansNumber() + "." + suffix();
}
QString EdtQtFileInfo::baseNameSansNumber()
{
    int i;
    QString DEBUGba = baseName();

    for (i=baseName().size(); i > 0 && baseName().at(i-1).isDigit(); i--)
        ;
    return baseName().left(i);
}

uint EdtQtFileInfo::number()
{
    uint num = 0;

    for (int i=baseName().size(); i > 0 && baseName().at(i-1).isDigit(); i--)
        num = baseName().right(baseName().size() - i).toInt();

    return num;
}

QString EdtQtFileInfo::incremented(int incr, int pad)
{
    QString snum;

    if (baseName().at(baseName().size()-1).isDigit())
        snum.sprintf("%0*d", pad, number() + incr);
    else snum.sprintf("%0*d", pad, 0);

    return path() + "/" + baseNameSansNumber() + snum + "." + suffix();
}

bool EdtQtFileInfo::hasNumber()
{
    if (baseName().at(baseName().size()-1).isDigit())
        return true;
    return false;
}

// set the filename suffix to a new suffix
// if the existing suffix is in the list of given suffixes, replace it. If not,
// append it.
QString EdtQtFileInfo::modSuffix(QString filter, QList<QFileInfo> list)
{
    QString name = fileName();
    QString newSuff = QFileInfo(filter).suffix().toLower();

    if (newSuff.endsWith(")")) // typically the filter is in the form e.g. "Bitmap (*.bmp)"
        newSuff.chop(1);

    // check the new suffix against the list (replace)
    for (int i=0; i < list.count(); i++)
    {
        if (list.at(i).suffix().toLower() == newSuff)
        {
            name = baseName();
            break;
        }
    }

    return path() + "/" + name + "." + newSuff;
}



