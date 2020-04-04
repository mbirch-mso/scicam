#include "CamSelectTreeItem.h"

CamSelectTreeItem::CamSelectTreeItem(const QString &in_name, CamSelectTreeItem *parent)
{
	parentItem = parent;
	name = in_name;
    cfg = NULL;
    camSlot = NULL;
    cfgSlot = NULL;
}

CamSelectTreeItem::CamSelectTreeItem(PdvConfig *in_cfg, CamSelectTreeItem *parent)
{
	parentItem = parent;
	cfg = in_cfg;
}


CamSelectTreeItem::~CamSelectTreeItem()
{
	qDeleteAll(childItems);
}

void CamSelectTreeItem::appendChild(CamSelectTreeItem *item)
{

	childItems.append(item);

}

CamSelectTreeItem* CamSelectTreeItem::child(int row) 
{

    if (childItems.size())
	    return childItems.value(row);
    else
        return NULL;
}

int CamSelectTreeItem::columnCount() const
{
	
	if (childItems.size())
        return 1;
    else
        return 3;
}

int CamSelectTreeItem::childCount() const
{

	return childItems.count();
}

CamSelectTreeItem* CamSelectTreeItem::parent() 
{

	return parentItem;
}


QVariant CamSelectTreeItem::data(int column) const 
{
	
    string s = "";
    QString qs;
    if (cfg)
    {
        switch(column)
        {
        case 0:
            s = cfg->GetCameraModel() + " " + cfg->GetCameraInfo();
            break;

        case 1:
            s = cfg->GetCameraModel();
            break;

        case 2:
            s = cfg->GetName();
            break;

        }
     
        qs = QString::fromStdString(s);
   }
    else
        qs = name;


	return QVariant(qs);

}

int CamSelectTreeItem::row() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<CamSelectTreeItem*>(this));

     return 0;
 }

void CamSelectTreeItem::treeItemConnect()
{
	if (camSlot)
        QObject::connect(this, SIGNAL(this->treeItemSelected()),
		             camSlot, SLOT(setValue()));

	if (cfgSlot)
        QObject::connect(this, SIGNAL(this->treeItemSelected()),
		             cfgSlot, SLOT(setValue()));
}

PdvConfig *CamSelectTreeItem::Config()

{
    return cfg;
}

