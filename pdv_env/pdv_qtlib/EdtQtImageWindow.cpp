#include "EdtQtImageWindow.h"


#include <QMouseEvent>

EdtQtImageWindow::EdtQtImageWindow(QWidget *parent)
	: QMainWindow(parent)
{

    own_update = true;

    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QWidget *centralwidget;
    QFrame *frame;

    centralwidget = new QWidget(this);

    horizontalLayout = new QHBoxLayout(centralwidget);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(1, 1, 1, 1);

    QPushButton *button;
    QIcon icon;

    frame = new QFrame(centralwidget);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setMinimumSize(QSize(0, 0));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(3);

    verticalLayout = new QVBoxLayout(frame);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
    verticalLayout->setContentsMargins(0, 0, 0, -1);
 
    button = new QPushButton(frame);
    icon.addFile(QString::fromUtf8(":/pdv_qtlib/Resources/zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    verticalLayout->addWidget(button);
    button->setFixedWidth(32);
    button->setFixedHeight(32);
    button->setIcon(icon);
    connect(button,SIGNAL(clicked()), this, SLOT(ZoomIn()));

    button = new QPushButton(frame);
    icon.addFile(QString::fromUtf8(":/pdv_qtlib/Resources/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
    verticalLayout->addWidget(button);
    button->setFixedWidth(32);
    button->setFixedHeight(32);
    button->setIcon(icon);
    connect(button,SIGNAL(clicked()), this, SLOT(ZoomOut()));

    button = new QPushButton(frame);
    icon.addFile(QString::fromUtf8(":/pdv_qtlib/Resources/contrast.png"), QSize(), QIcon::Normal, QIcon::Off);
    verticalLayout->addWidget(button);
    button->setFixedWidth(32);
    button->setFixedHeight(32);
    button->setIcon(icon);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);

    image_window = new EdtQtImageView(this);
    horizontalLayout->addWidget(frame);

    horizontalLayout->addWidget(image_window);

    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));

    setStatusBar(statusbar);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateStatus()));
    timer->start(10);

	QLabel *label = new QLabel(" X ");

	statusbar->addPermanentWidget(label);
	pixelX = new QLabel;
	statusbar->addPermanentWidget(pixelX);
	label = new QLabel(" Y ");
	statusbar->addPermanentWidget(label);
	pixelY = new QLabel;
	statusbar->addPermanentWidget(pixelY);
	label = new QLabel(" Pixel ");
	statusbar->addPermanentWidget(label);
	pixelvalues = new QLabel;

	statusbar->addPermanentWidget(pixelvalues);

    setCentralWidget(centralwidget);
}

/*EdtQtImageWindow::~EdtQtImageWindow()

{

}*/

void EdtQtImageWindow::Setup()

{
    

}

EdtQtImageItem *EdtQtImageWindow::ImageWindow(const int index)

{

    return image_window->ImageWindow(index);
}

EdtQtImageItem *EdtQtImageWindow::AddImageItem(EdtImageData *pimage)

{
    if (pimage)
    {
        image_window->resize(pimage->GetWidth(),pimage->GetHeight());
        //image_window->setFixedWidth(pimage->GetWidth());
        //image_window->setFixedHeight(pimage->GetHeight());
    }

    return image_window->AddImageItem();
}

void EdtQtImageWindow::Refresh()

{
    image_window->Refresh();
}

double EdtQtImageWindow::GetMeanUpdate()

{

    return image_window->GetMeanUpdate();

}

void EdtQtImageWindow::GetCursorValue(QPoint &q, QString &tag)

{    	
    
    image_window->GetCursorValue(q, tag);


}

bool EdtQtImageWindow::GetAutoScale()

{
    return image_window->GetAutoScale();
}

void EdtQtImageWindow::SetAutoScale(const bool ascale)

{
    image_window->SetAutoScale(ascale);
}

void EdtQtImageWindow::SetZoom(const double scale)

{
    image_window->SetZoom(scale);


}

double EdtQtImageWindow::GetZoom()

{
    return image_window->GetZoom();
}

void EdtQtImageWindow::Scale(const double dscale)

{
    image_window->Scale(dscale);
}

void EdtQtImageWindow::UpdateStatus()

{
    char labelstr[80];


	QString pixel;
	QPoint pt;

	image_window->GetCursorValue(pt, pixel);

	char buf[32];
	sprintf(buf,"%d", pt.x());

	pixelX->setText(buf);
	sprintf(buf,"%d", pt.y());
	pixelY->setText(buf);
	
	pixelvalues->setText(pixel);

    image_window->Refresh();

}
void EdtQtImageWindow::SetOwnUpate(const bool state)
{
    own_update = state;
}

bool EdtQtImageWindow::GetOwnUpdate() const

{
    return own_update;
}

void EdtQtImageWindow::ZoomIn()

{
    image_window->Scale(1.5);
}

void EdtQtImageWindow::ZoomOut()

{
    image_window->Scale(1.0 / 1.5);
}

void EdtQtImageWindow::Zoom1()

{
    image_window->SetZoom(1.0);

}

void EdtQtImageWindow::ZoomAuto(bool state)

{
    image_window->SetAutoScale(state);
}


void  EdtQtImageWindow::SetDataImage(EdtImage *img)

{
	image_window->SetDataImage(img);
    
}


EdtImage *EdtQtImageWindow::GetDataImage()

{
    return image_window->GetDataImage();
}
