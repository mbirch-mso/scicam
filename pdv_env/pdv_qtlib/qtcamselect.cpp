#include "qtcamselect.h"
#include <iostream>

#include "pdvplus/PdvConfig.h"

/* This is the graphical layout of the configuration file chooser */

/**********************************************************************************************//**
 * @fn  QtCamSelect::QtCamSelect(QWidget *parent, Qt::WindowFlags flags)
 *
 * @brief   Constructor.
 *
 * @param [in,out]  parent  If non-null, the parent.
 * @param   flags           The flags.
 **************************************************************************************************/

QtCamSelect::QtCamSelect(bool _cl_only, QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	int size = 0; //the size of the cam select tree window
			
    clink_only = _cl_only;

	//Labels for fields
	QLabel *camTypeLabel = new QLabel(tr("Camera Type: "));
	camType = new QLineEdit;
	
	QLabel *cfgFileNameLabel = new QLabel(tr("Config File Name: "));
	cfgFileName = new QLineEdit;

	QLabel *cfgPathLabel = new QLabel(tr("Config Path: "));
	cfgPath = new QLineEdit;

    clink_only = _cl_only;

	treeModel = new CamSelectTree(clink_only);
	//treeSelect = new QItemSelectionModel(treeModel);

    //Two dialog buttons
#ifdef CAMSELECTWIZARD // not implemented
	wizButton = new QPushButton(tr("Create new..."));
	srcButton = new QPushButton(tr("View source..."));
#endif

	//The three buttons
	okButton = new QPushButton(tr("Ok"));
	cancelButton = new QPushButton(tr("Cancel"));
	reloadButton = new QPushButton(tr("Reload"));

	//Allows you to react to all button events
	//in a single interface
	bGroup = new QButtonGroup;
	bGroup->addButton(okButton);
	bGroup->addButton(cancelButton);
	bGroup->addButton(reloadButton);
#ifdef CAMSELECTWIZARD
	bGroup->addButton(wizButton);
	bGroup->addButton(srcButton);
#endif

	//Where the file tree appears
	QVBoxLayout *fileTreeLayout = new QVBoxLayout;
	fileTreeLayout->addSpacing(size);
	cfgFileTree = new QTreeView;
	cfgFileTree->setSelectionMode(QAbstractItemView::SingleSelection);
 	cfgFileTree->setModel(treeModel);
	cfgFileTree->setHeaderHidden(true);
	cfgFileTree->setEnabled(true);
	fileTreeLayout->addWidget(cfgFileTree);

	//connect TreeView to parent to handle events
	QObject::connect(cfgFileTree->selectionModel(), 
		             SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
					 this, SLOT(setField(const QItemSelection &, const QItemSelection &)));
#ifdef CAMSELECTWIZARD
	QObject::connect(wizButton, SIGNAL(clicked()),
					 this, SLOT(handleWizButtonClick()));
	QObject::connect(srcButton, SIGNAL(clicked()),
					 this, SLOT(handleSrcButtonClick()));
#endif
	QObject::connect(okButton, SIGNAL(clicked()),
					 this, SLOT(handleOkButtonClick()));
	QObject::connect(cancelButton, SIGNAL(clicked()),
		             this, SLOT(handleCancelButtonClick()));
					
	//Holds all the buttons together
#ifdef CAMSELECTWIZARD
	QHBoxLayout *button1Layout = new QHBoxLayout;
	button1Layout->addWidget(wizButton);
	button1Layout->addWidget(srcButton);
#endif

	QHBoxLayout *button2Layout = new QHBoxLayout;
	button2Layout->addWidget(okButton);
	button2Layout->addWidget(cancelButton);
	button2Layout->addWidget(reloadButton);

    int row=0;

	//The main display
	mainLayout = new QGridLayout;
	mainLayout->addLayout(fileTreeLayout, row, 0, 1, 2);
     row++;
	mainLayout->addWidget(camTypeLabel, row, 0);
	mainLayout->addWidget(camType, row, 1);
     row++;
	mainLayout->addWidget(cfgFileNameLabel, row, 0);
	mainLayout->addWidget(cfgFileName, row, 1);
     row++;
	mainLayout->addWidget(cfgPathLabel, row, 0);
	mainLayout->addWidget(cfgPath, row, 1);
     row++;

    widthLineEdit = new QLineEdit();
    heightLineEdit = new QLineEdit();
    depthLineEdit = new QLineEdit;

    mainLayout->addWidget(new QLabel("Width"),row,0);
    mainLayout->addWidget(widthLineEdit,row,1);
     row++;
    mainLayout->addWidget(new QLabel("Height"),row,0);
    mainLayout->addWidget(heightLineEdit,row,1);
     row++;
    mainLayout->addWidget(new QLabel("Depth"),row,0);
    mainLayout->addWidget(depthLineEdit,row,1);
     row++;
    QLabel *label = new QLabel(tr("Camera Link only: "));
    clink_checkbox = new QCheckBox();
	mainLayout->addWidget(label, row, 0);
	mainLayout->addWidget(clink_checkbox, row, 1);
    clink_checkbox->setChecked(clink_only);
     row++;
#ifdef CAMSELECTWIZARD
	mainLayout->addLayout(button1Layout, row, 0, 1, 2);
     row++;
#endif
	mainLayout->addLayout(button2Layout, row, 0, 1, 2);

	//Display the window
	setLayout(mainLayout);
	setWindowTitle("Camera Configuration");
}

QtCamSelect::~QtCamSelect()
{

}

void QtCamSelect::setField(const QItemSelection &selected, const QItemSelection &deselect)
{ 
	
	int pathIndex = 1;
	int fnameIndex = 2;
	int camIndex = 0;

	QModelIndex index = cfgFileTree->selectionModel()->currentIndex();
	CamSelectTreeItem *item = static_cast<CamSelectTreeItem*>(index.internalPointer());
	QVariant child_count(item->childCount());

    PdvConfig *cfg = item->Config();
	//is a child object
	if(cfg != 0)
	{

        string s = cfg->GetCameraClass() + " " + cfg->GetCameraModel() + " " + cfg->GetCameraInfo();

		camType->setText(s.c_str());

		s_cfgFileName = cfg->GetName().c_str();
		cfgFileName->setText(s_cfgFileName);
		s_cfgPath = cfg->GetPath().c_str();
		cfgPath->setText(s_cfgPath);

        QString qs;

        qs = QString::number(cfg->GetWidth());
        widthLineEdit->setText(qs);
        qs = QString::number(cfg->GetHeight());
        heightLineEdit->setText(qs);
        qs = QString::number(cfg->GetDepth());
        depthLineEdit->setText(qs);

        clink_checkbox->setChecked(cfg->IsClink());

	}
    
	//is a parent object
	else 
	{
		camType->setText(item->data(camIndex).toString());
		cfgFileName->setText("");
		cfgPath->setText("");
	}
}

void QtCamSelect::handleOkButtonClick()
{
	//ok button is clicked
	if(!(QString::compare(s_cfgFileName,"")==0))
	{
	    //use the PdvInput object to perform the config file setup
		ConfigSelected(s_cfgPath, s_cfgFileName);
	}
    close();
}

void QtCamSelect::handleCancelButtonClick()
{
	close();
}


void QtCamSelect::handleWizButtonClick()
{
    close();
}

void QtCamSelect::handleSrcButtonClick()
{
    close();
}


void QtCamSelect::setCLinkOnly(const bool state)
{
    clink_only = state;
}

bool QtCamSelect::CLinkOnly()
{
    return clink_only;
}

// only compares the filename, should check for the proper directory too?
void QtCamSelect::setSelected(const QFileInfo *fileInfo)
{
    const QAbstractItemModel *model = cfgFileTree->selectionModel()->model();

    for (int i=0; i<model->rowCount(); i++)
    {
        const QModelIndex items = model->index(i, 0);

        for (int j=0; items.child(j,2).isValid(); j++)
        {
            if (items.child(j,2).data(0).toString() == fileInfo->fileName())
            {
                cfgFileTree->selectionModel()->setCurrentIndex(items.child(j,0), QItemSelectionModel::Select);
                break;
            }
        }
    }
}
