#include "DevSelectTreeItem.h"

DevSelectTreeItem::DevSelectTreeItem(const QString &in_name, DevSelectTreeItem *parent)
{
	parentItem = parent;
	name = in_name;
    dev = NULL;
    devSlot = NULL;
}

DevSelectTreeItem::DevSelectTreeItem(PdvDevice *in_dev, DevSelectTreeItem *parent)
{
	parentItem = parent;
	dev = in_dev;
}


DevSelectTreeItem::~DevSelectTreeItem()
{
	qDeleteAll(childItems);
}

void DevSelectTreeItem::appendChild(DevSelectTreeItem *item)
{

	childItems.append(item);

}

DevSelectTreeItem* DevSelectTreeItem::child(int row) 
{

    if (childItems.size())
	    return childItems.value(row);
    else
        return NULL;
}

int DevSelectTreeItem::columnCount() const
{
	
	if (childItems.size())
        return 1;
    else
        return 3;
}

int DevSelectTreeItem::childCount() const
{

	return childItems.count();
}

DevSelectTreeItem* DevSelectTreeItem::parent() 
{

	return parentItem;
}


QVariant DevSelectTreeItem::data(int column) const 
{
	
    string s = "";
    QString qs;

    if (dev)
    {
        switch(column)
        {
        case 0:
            s = dev->GetDeviceInterface() + " " + SSTR(dev->GetDeviceUnit());
            break;

        case 1:
            s = SSTR(dev->GetDeviceUnit());
            break;

        case 2:
            s = SSTR(dev->GetDeviceChannel());
            break;

        }
     
        qs = QString::fromStdString(s);
   }
    else
        qs = name;


	return QVariant(qs);

}

int DevSelectTreeItem::row() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<DevSelectTreeItem*>(this));

     return 0;
 }

void DevSelectTreeItem::treeItemConnect()
{
	if (devSlot)
        QObject::connect(this, SIGNAL(this->treeItemSelected()),
		             devSlot, SLOT(setValue()));

}

PdvDevice *DevSelectTreeItem::Device()

{
    return dev;
}

