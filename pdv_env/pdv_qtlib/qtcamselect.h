#ifndef QTCAMSELECT_H
#define QTCAMSELECT_H

#include "pdvplus/PdvInput.h"
//#include <QtGui>
#include <QDialog>
#include <QWidget>
#include <QTreeView>
#include <QButtonGroup>
#include <QModelIndex>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include "pdv_qtlib/CamSelectTree.h"


class QtCamSelect : public QDialog
{
	Q_OBJECT

public:
    QtCamSelect(bool cl_only = FALSE, QWidget *parent = 0, Qt::WindowFlags flags = 0);
   virtual ~QtCamSelect();

    void setSelected(const QFileInfo *file);
    // const QString &Path();
    // const QString &Fname();

    void setCLinkOnly(const bool state);
    bool CLinkOnly();
	
private:
	
    QLineEdit *camType;
    QLineEdit *cfgFileName;
    QLineEdit *cfgPath;
    QButtonGroup *bGroup;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *reloadButton;

#ifdef WIZBUTTONS
    QPushButton *wizButton;
    QPushButton *srcButton;
#endif

    CamSelectTree *treeModel;
    QItemSelectionModel *treeSelect;
    QTreeView *cfgFileTree;
    QGridLayout *mainLayout;

    QString s_cfgFileName;
    QString s_cfgPath;
    QLineEdit *widthLineEdit;
    QLineEdit *heightLineEdit;
    QLineEdit *depthLineEdit;

    QCheckBox *clink_checkbox;

    PdvInput *p_camera;

    bool clink_only;


public:

public slots:

    void handleOkButtonClick();
    void handleCancelButtonClick();
    void handleWizButtonClick();
    void handleSrcButtonClick();
    void setField(const QItemSelection &selection, const QItemSelection &deselect);
	
signals:

    void ConfigSelected(const QString &cfg_path, const QString &cfg_name);
	
};

#endif //QTCAMSELECT_H
