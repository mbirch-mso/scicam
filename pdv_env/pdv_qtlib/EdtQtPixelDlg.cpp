
#include "EdtQtPixelDlg.h"


EdtQtPixelDlg::EdtQtPixelDlg(EdtQtImageItem *win, QWidget * parent) : QDialog(parent),
    ui(new Ui::EdtQtPixelDlg)

{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    table = ui->tableWidget;

    SetWindow(win);

    QObject::connect(ui->hexCheckBox, SIGNAL(clicked(bool)), this, SLOT(onHexChecked(bool)));
    QObject::connect(ui->xEdit, SIGNAL(editingFinished()), this, SLOT(onEditPt()));
    QObject::connect(ui->yEdit, SIGNAL(editingFinished()), this, SLOT(onEditPt()));

}

EdtQtPixelDlg::~EdtQtPixelDlg()

{

}

void EdtQtPixelDlg::SetWindow(EdtQtImageItem *win)

{
    table->SetWindow(win);
    RefreshPoints();

}

void EdtQtPixelDlg::SetImage(EdtImage *image)

{
    table->SetImage(image);
    RefreshPoints();

}
void EdtQtPixelDlg::SetImagePoint(EdtImage *image, QPointF pt)

{
    table->SetImagePoint(image, pt);
    RefreshPoints();

}

void EdtQtPixelDlg::SetPoint(QPointF pt)

{
    table->SetPoint(pt);
    RefreshPoints();


}

void EdtQtPixelDlg::update_display()

{
    table->update_display();
    RefreshPoints();

}

void EdtQtPixelDlg::onHexChecked(bool checked)

{
    table->SetHex(checked);
    RefreshPoints();
}

void EdtQtPixelDlg::onEditPt()

{
    SetPoint(QPointF(ui->xEdit->text().toDouble(), ui->yEdit->text().toDouble()));
}

void EdtQtPixelDlg::RefreshPoints()

{

    QPointF p2 = table->Center();

    QString s;

    s= QString::number((int) p2.x());
    ui->xEdit->setText(s);
    s= QString::number((int) p2.y());
    ui->yEdit->setText(s);

}