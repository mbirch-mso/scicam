
#ifndef DEVSELECTTREE_H
#define DEVSELECTTREE_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QLineEdit>


//#include "pdvplus/PdvDeviceSet.h"

class DevSelectTreeItem;

class DevSelectTree : public QAbstractItemModel
{
	Q_OBJECT

public:
	DevSelectTree(QObject *parent = 0);
	virtual ~DevSelectTree();
	
	QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	QVariant data(const QModelIndex &index, int role) const;

	//Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	DevSelectTreeItem *root;
	void setModelData();
	DevSelectTreeItem *getItem(const QModelIndex &index) const;
	QLineEdit *devSlot;
        QString devname;

};



#endif
