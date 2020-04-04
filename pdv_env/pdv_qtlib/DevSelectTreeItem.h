#pragma once

#ifndef DEVSELECTTREEITEM_H
#define DEVSELECTTREEITEM_H

#include <QList>
#include <QVariant>
#include <QLineEdit>

#include "pdvplus/PdvDevice.h"

// maybe stick this in a more common place some time
#include <sstream>
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()

class DevSelectTreeItem : public QObject
{
    Q_OBJECT

public:
    DevSelectTreeItem(const QString &in_name, DevSelectTreeItem *parent = NULL);

    DevSelectTreeItem(PdvDevice *in_dev, DevSelectTreeItem *parent = NULL);
    virtual ~DevSelectTreeItem();

    void appendChild(DevSelectTreeItem *item);
    DevSelectTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    DevSelectTreeItem *parent();
    QVariant data(int column) const;
    PdvDevice *Device();
signals:
    void treeItemSelected();

private:
     QList<DevSelectTreeItem*> childItems;
    
     DevSelectTreeItem *parentItem;
     QLineEdit *devSlot;
     void treeItemConnect();

     QString name;
     PdvDevice *dev;
};

#endif //DEVSELECTTREEITEM_H
