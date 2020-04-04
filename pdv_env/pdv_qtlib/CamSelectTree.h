#pragma once

#ifndef CAMSELECTTREE_H
#define CAMSELECTTREE_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QLineEdit>
#include "CamSelectTreeItem.h"
#include <iostream>

#include "pdvplus/PdvConfigSet.h"



class CamSelectTree : public QAbstractItemModel
{
	Q_OBJECT

public:
	CamSelectTree(bool _cl_only, QObject *parent = 0);
	virtual ~CamSelectTree();
	
	QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	QVariant data(const QModelIndex &index, int role) const;

	//Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	CamSelectTreeItem *root;
	void setModelData();
	CamSelectTreeItem *getItem(const QModelIndex &index) const;
	QLineEdit *cfgSlot;
	QLineEdit *camSlot;
    QString cfgFilePath;

    bool clink_only; // this should really be filter function


};



#endif
