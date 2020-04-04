#pragma once

#ifndef CAMSELECTTREEITEM_H
#define CAMSELECTTREEITEM_H

#include <QList>
#include <QVariant>
#include <QLineEdit>

#include "pdvplus/PdvConfig.h"

class CamSelectTreeItem : public QObject
{
	Q_OBJECT

public:
    CamSelectTreeItem(const QString &in_name, CamSelectTreeItem *parent = NULL);
    CamSelectTreeItem(PdvConfig *in_cfg, CamSelectTreeItem *parent = NULL);
	virtual ~CamSelectTreeItem();

	void appendChild(CamSelectTreeItem *item);
	CamSelectTreeItem *child(int row);
	int childCount() const;
    int columnCount() const;
	int row() const;
    CamSelectTreeItem *parent();
    QVariant data(int column) const;
    PdvConfig *Config();
signals:
    void treeItemSelected();

private:
	 QList<CamSelectTreeItem*> childItems;
    
     CamSelectTreeItem *parentItem;
	 QLineEdit *camSlot;
	 QLineEdit *cfgSlot;
	 void treeItemConnect();

     QString name;
     PdvConfig *cfg;
};

#endif //CAMSELECTTREEITEM_H
