#include "edtinc.h"
#include "vlviewer.h"
#include <QtGui/QApplication>
#ifdef __linux
#include <QCleanlooksStyle>
#endif

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

#ifdef __linux // fixes some inexplicable widget errors for whatever reason
    a.setStyle(new QCleanlooksStyle);
#endif

    // might not need to do this in linux
    Q_INIT_RESOURCE(pdv_qtlib);

    vlviewer w;

    if (w.do_quit())
        return w.do_quit() - 1;

    w.show();

    return a.exec();
}

