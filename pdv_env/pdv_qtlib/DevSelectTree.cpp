#include "DevSelectTree.h"
#include "DevSelectTreeItem.h"

#include "pdvplus/PdvDeviceSet.h"

DevSelectTree::DevSelectTree(QObject *parent)
: QAbstractItemModel(parent)
{
    root = new DevSelectTreeItem("");

    devSlot = NULL;

    setModelData();
}

DevSelectTree::~DevSelectTree()
{
    delete root;
}

QModelIndex DevSelectTree::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();

    DevSelectTreeItem *parentItem = getItem(parent);

    DevSelectTreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DevSelectTree::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DevSelectTreeItem *childItem = static_cast<DevSelectTreeItem*>(index.internalPointer());
    DevSelectTreeItem *parentItem = childItem->parent();

    if (parentItem == root || parentItem == NULL)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DevSelectTree::rowCount(const QModelIndex &parent) const
{	
    if(parent.column() > 0)
        return 0;

    DevSelectTreeItem * parentItem;

    if (!parent.isValid()) 
        return root->childCount();

    parentItem = static_cast<DevSelectTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int DevSelectTree::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DevSelectTreeItem*>(parent.internalPointer())->columnCount();
    else
        return root->columnCount();
}

QVariant DevSelectTree::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DevSelectTreeItem *item = static_cast<DevSelectTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

DevSelectTreeItem *DevSelectTree::getItem(const QModelIndex &index) const 
{
    if (index.isValid()) {
        DevSelectTreeItem *item = static_cast<DevSelectTreeItem*>(index.internalPointer());
        if(item) return item;
    }

    return root;
}


/*Qt::ItemFlags DevSelectTree::flags(const QModelIndex &index) const
{
if(!index.isValid())
return Qt::ItemIsEnabled;

return Qt::ItemIsEditable | Qt::ItemIsSelectable;
}*/

void DevSelectTree::setModelData()
{
    PdvDeviceSet dset;
    DevSelectTreeItem *currParent;
    currParent = root;

    dset.EnumerateDevices("pdv");

    double x = edt_dtime();   // why?

    PdvDeviceUnit *d = dset.FirstDeviceUnit();


    int ndevs = 0;

    while (d)
    {
        vector<PdvDevice *> *v = d->second;
        QString qs = QString::number(d->first);

        currParent = new DevSelectTreeItem(qs, root);
        root->appendChild(currParent);

        DevSelectTreeItem *child;

        size_t i;

        for (i=0;i<v->size();i++)
        {
            PdvDevice *dev = (*v)[i];

            child = new DevSelectTreeItem(dev, currParent);   

            currParent->appendChild(child);

            ndevs++;

        }

        d = dset.NextDeviceUnit();
    }
}
