#-------------------------------------------------
#
# Project created by QtCreator 2011-07-01T11:04:55
#
# MODIFIED for shipping with EDTpdv package ; top level (vlv/..)
# makefile calls qmake with this file as an argument the first time
# it tries to make vlviewer, or any time vlv/makefile is not present
#
#-------------------------------------------------

QT	+= core gui svg
CONFIG += qt resources release
#CONFIG += qt resources debug
DESTDIR = ..
MAKEFILE=makefile
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-but-set-parameter -Wno-unused-variable -Wno-unused-value -Wno-ignored-qualifiers -Wno-unused-but-set-variable

TARGET = vlviewer
TEMPLATE = app

SOURCES += main.cpp\
        vlviewer.cpp \

HEADERS  += vlviewer.h 

FORMS    += vlviewer.ui

INCLUDEPATH += .. ../pdv_qtlib ../pdvplus /opt/EDTpdv ../edtimage ../imgfiles ../dispatch ../jpeg-6b
DEPENDPATH += .. ../pdv_qtlib ../pdvplus /opt/EDTpdv ../edtimage ../imgfiles ../dispatch
LIBS += -L.. 

QMAKE_RPATHDIR += /opt/EDTpdv

win32:CONFIG(release, debug|release): LIBS += -L../pdv_qtlib/release/ -lpdv_qt
else:win32:CONFIG(debug, debug|release): LIBS += -L../pdv_qtlib/debug/ -lpdv_qt
else:symbian: LIBS += -lpdv_qt
else:unix: LIBS += -lpdv_qt

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += ../pdv_qtlib/release/libpdv_qt.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../pdv_qtlib/debug/libpdv_qt.lib
else:unix:!symbian: PRE_TARGETDEPS += ../libpdv_qt.a

win32:CONFIG(release, debug|release): LIBS += -L../dispatch/release/ -ldispatch
else:win32:CONFIG(debug, debug|release): LIBS += -L../dispatch/debug/ -ldispatch
else:symbian: LIBS += -ldispatch
else:unix: LIBS += -ldispatch

win32:CONFIG(release, debug|release): LIBS += -L../pdvplus/release/ -lpdvplus
else:win32:CONFIG(debug, debug|release): LIBS += -L../pdvplus/debug/ -lpdvplus
else:symbian: LIBS += -lpdvplus
else:unix: LIBS += -lpdvplus

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += ../pdvplus/release/pdvplus.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../pdvplus/debug/pdvplus.lib
else:unix:!symbian: PRE_TARGETDEPS += ../libpdvplus.a

win32:CONFIG(release, debug|release): LIBS += -L../release/ -lpdv
else:win32:CONFIG(debug, debug|release): LIBS += -L../debug/ -lpdv
else:symbian: LIBS += -lpdv
else:unix: LIBS += -lpdv

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += ../release/pdv.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../debug/pdv.lib
else:unix:!symbian: PRE_TARGETDEPS += ../libpdv.a

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += ../edtimage/release/edtimage.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../edtimage/debug/edtimage.lib
else:unix:!symbian: PRE_TARGETDEPS += ../libedtimage.a

win32:CONFIG(release, debug|release): LIBS += -L../edtimage/release/ -ledtimage
else:win32:CONFIG(debug, debug|release): LIBS += -L../edtimage/debug/ -ledtimage
else:symbian: LIBS += -ledtimage
else:unix: LIBS += -ledtimage

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += ../imgfiles/release/imgfiles.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../imgfiles/debug/imgfiles.lib
else:unix:!symbian: PRE_TARGETDEPS += ../libimgfiles.a

win32:CONFIG(release, debug|release): LIBS += -L../imgfiles/release/ -limgfiles
else:win32:CONFIG(debug, debug|release): LIBS += -L../imgfiles/debug/ -limgfiles
else:symbian: LIBS += -limgfiles
else:unix: LIBS += -limgfiles

#LIBS += -ldl -ljpeg -ltiff -lreadline -lncurses
LIBS += -ldl -ljpeg -ltiff
