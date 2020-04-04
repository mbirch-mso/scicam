#include "CamSelectTree.h"

CamSelectTree::CamSelectTree(bool cl_only, QObject *parent)
: QAbstractItemModel(parent)
{
    root = new CamSelectTreeItem("");

    cfgSlot = NULL;
    camSlot = NULL;

    clink_only = cl_only;

    setModelData();
}

CamSelectTree::~CamSelectTree()
{
    delete root;
}

QModelIndex CamSelectTree::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();

    CamSelectTreeItem *parentItem = getItem(parent);

    CamSelectTreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex CamSelectTree::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CamSelectTreeItem *childItem = static_cast<CamSelectTreeItem*>(index.internalPointer());
    CamSelectTreeItem *parentItem = childItem->parent();

    if (parentItem == root || parentItem == NULL)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CamSelectTree::rowCount(const QModelIndex &parent) const
{	
    if(parent.column() > 0)
        return 0;

    CamSelectTreeItem * parentItem;

    if (!parent.isValid()) 
        return root->childCount();

    parentItem = static_cast<CamSelectTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int CamSelectTree::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<CamSelectTreeItem*>(parent.internalPointer())->columnCount();
    else
        return root->columnCount();
}

QVariant CamSelectTree::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    CamSelectTreeItem *item = static_cast<CamSelectTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

CamSelectTreeItem *CamSelectTree::getItem(const QModelIndex &index) const 
{
    if (index.isValid()) {
        CamSelectTreeItem *item = static_cast<CamSelectTreeItem*>(index.internalPointer());
        if(item) return item;
    }

    return root;
}


/*Qt::ItemFlags CamSelectTree::flags(const QModelIndex &index) const
{
if(!index.isValid())
return Qt::ItemIsEnabled;

return Qt::ItemIsEditable | Qt::ItemIsSelectable;
}*/

void CamSelectTree::setModelData()
{
    PdvConfigSet cset;
    CamSelectTreeItem *currParent;
    currParent = root;

    cset.LoadConfigFiles(clink_only);

    double d = edt_dtime();  

    PdvCameraClass *c = cset.FirstCameraClass();

    int nclasses = 0;
    int nfiles = 0;

    while (c)
    {
        vector<PdvConfig *> *v = c->second;
        QString qs = c->first.c_str();

        currParent = new CamSelectTreeItem(qs, root);
        root->appendChild(currParent);

        CamSelectTreeItem *child;

        size_t i;

        for (i=0;i<v->size();i++)
        {
            PdvConfig *cfg = (*v)[i];

            child = new CamSelectTreeItem(cfg, currParent);   

            currParent->appendChild(child);

            nfiles++;

        }

        c = cset.NextCameraClass();

        nclasses++;

    }
}
