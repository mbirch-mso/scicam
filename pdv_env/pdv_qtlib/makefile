
!include ../makefile.def

PDVHOME=..

CC = cl

INTDIR=$(CFG)$(ARCH)

!IF "$(ARCH)" == "x86"

!IF "$(QTDIR)" == ""
    QTDIR=c:\qt\4.8.4
!ENDIF

!ELSE

!IF "$(QTDIR)" == ""
    QTDIR=c:\qt\4.8.4_x64
!ENDIF

!ENDIF

QTMOC=$(QTDIR)\bin\moc.exe
QTUIC=$(QTDIR)\bin\uic.exe
QTRCC=$(QTDIR)\bin\rcc.exe

QTINCLUDEFLAGS=-I$(QTDIR) -I$(QTDIR)\include -I$(QTDIR)\include\QtCore -I$(QTDIR)\include\qtmain -I$(QTDIR)\include\QtGui
INCLUDEFLAGS=-I.. -I..\edtimage  $(QTINCLUDEFLAGS)

GENSRC=.\GeneratedFiles

INCLUDEFLAGS = $(INCLUDEFLAGS) -I..\img -I..\ffms-2.16_SDK -I$(GENSRC)

CFLAGS = /EHsc $(OPTIM) -DWIN32 -nologo $(INCLUDEFLAGS)
CCFLAGS = $(OPTIM) -DWIN32 -nologo $(INCLUDEFLAGS)

#LIBS = pdvlib.lib

LINK32 = link
DLLFLAGS = /dll /nologo /link50compat /map

LIB32 = link -lib

LIBOBJECTS = \
    $(INTDIR)\AppSettings.obj \
    $(INTDIR)\HelpDlg.obj \
    $(INTDIR)\DisplaySkip.obj \
    $(INTDIR)\CamSettingsDlg.obj \
    $(INTDIR)\CamSerialDlg.obj \
    $(INTDIR)\CamSelectTree.obj \
    $(INTDIR)\CamSelectTreeItem.obj \
    $(INTDIR)\DevSelectTree.obj \
    $(INTDIR)\DevSelectTreeItem.obj \
    $(INTDIR)\DisplayContrast.obj \
    $(INTDIR)\EdtQtContrastCanvas.obj \
    $(INTDIR)\EdtQtHistoCanvas.obj \
    $(INTDIR)\EdtQtHistogramItem.obj \
    $(INTDIR)\EdtQtGraphicsItem.obj \
    $(INTDIR)\EdtQtImageItem.obj \
    $(INTDIR)\EdtQtImageScene.obj \
    $(INTDIR)\EdtQtImageView.obj \
    $(INTDIR)\EdtQtImageViewer.obj \
    $(INTDIR)\EdtQtImageWindow.obj \
    $(INTDIR)\EdtQtLiveDisplay.obj \
    $(INTDIR)\EdtQtDeviceDlg.obj \
    $(INTDIR)\DevInfoMsgBox.obj \
    $(INTDIR)\EdtQtPixelTable.obj \
    $(INTDIR)\EdtQtPixelDlg.obj \
    $(INTDIR)\EdtQtPlotCanvas.obj \
    $(INTDIR)\EdtQtPlotCurve.obj \
    $(INTDIR)\EdtQtPlotView.obj \
    $(INTDIR)\EdtQtProfileCanvas.obj \
    $(INTDIR)\EdtQtFileInfo.obj \
    $(INTDIR)\EdtIntervalData.obj \
    $(INTDIR)\contrastwidget.obj \
    $(INTDIR)\histowidget.obj \
    $(INTDIR)\profilewidget.obj \
    $(INTDIR)\qtcamselect.obj \
    $(INTDIR)\moc_CamSettingsDlg.obj \
    $(INTDIR)\moc_HelpDlg.obj \
    $(INTDIR)\moc_DisplaySkip.obj \
    $(INTDIR)\moc_CamSerialDlg.obj \
    $(INTDIR)\moc_CamSelectTree.obj \
    $(INTDIR)\moc_CamSelectTreeItem.obj \
    $(INTDIR)\moc_DevSelectTree.obj \
    $(INTDIR)\moc_DevSelectTreeItem.obj \
    $(INTDIR)\moc_EdtQtContrastCanvas.obj \
    $(INTDIR)\moc_EdtQtHistoCanvas.obj \
    $(INTDIR)\moc_EdtQtHistogramItem.obj \
    $(INTDIR)\moc_EdtQtGraphicsItem.obj \
    $(INTDIR)\moc_EdtQtImageItem.obj \
    $(INTDIR)\moc_EdtQtImageScene.obj \
    $(INTDIR)\moc_EdtQtImageView.obj \
    $(INTDIR)\moc_EdtQtImageViewer.obj \
    $(INTDIR)\moc_EdtQtImageWindow.obj \
    $(INTDIR)\moc_EdtQtDeviceDlg.obj \
    $(INTDIR)\moc_EdtQtPixelDlg.obj \
    $(INTDIR)\moc_EdtQtPixelTable.obj \
    $(INTDIR)\moc_EdtQtProfileCanvas.obj \
    $(INTDIR)\moc_EdtQtPlotCanvas.obj \
    $(INTDIR)\moc_EdtQtPlotView.obj \
    $(INTDIR)\moc_edtqtplotcurve.obj \
    $(INTDIR)\moc_contrastwidget.obj \
    $(INTDIR)\moc_histowidget.obj \
    $(INTDIR)\moc_profilewidget.obj \
    $(INTDIR)\moc_qtcamselect.obj \
    $(INTDIR)\qrc_pdv_qtlib.obj \
    $(INTDIR)\qrc_qtcamselect.obj

MOCS = \
    $(GENSRC)\moc_HelpDlg.cpp \
    $(GENSRC)\moc_DisplaySkip.cpp \
    $(GENSRC)\moc_CamSettingsDlg.cpp \
    $(GENSRC)\moc_CamSerialDlg.cpp \
    $(GENSRC)\moc_CamSelectTree.cpp \
    $(GENSRC)\moc_CamSelectTreeItem.cpp \
    $(GENSRC)\moc_EdtQtDeviceDlg.cpp \
    $(GENSRC)\moc_DevSelectTree.cpp \
    $(GENSRC)\moc_DevSelectTreeItem.cpp \
    $(GENSRC)\moc_EdtQtContrastCanvas.cpp \
    $(GENSRC)\moc_EdtQtHistoCanvas.cpp \
    $(GENSRC)\moc_EdtQtHistogramItem.cpp \
    $(GENSRC)\moc_EdtQtGraphicsItem.cpp \
    $(GENSRC)\moc_EdtQtPixelDlg.cpp \
    $(GENSRC)\moc_EdtQtPixelTable.cpp \
    $(GENSRC)\moc_EdtQtPlotCanvas.cpp \
    $(GENSRC)\moc_EdtQtProfileCanvas.cpp \
    $(GENSRC)\moc_EdtQtImageItem.cpp \
    $(GENSRC)\moc_EdtQtImageScene.cpp \
    $(GENSRC)\moc_EdtQtImageView.cpp \
    $(GENSRC)\moc_EdtQtImageViewer.cpp \
    $(GENSRC)\moc_EdtQtImageWindow.cpp \
    $(GENSRC)\moc_contrastwidget.cpp \
    $(GENSRC)\moc_histowidget.cpp \
    $(GENSRC)\moc_profilewidget.cpp \
    $(GENSRC)\moc_edtqtplotcurve.cpp \
    $(GENSRC)\moc_qtcamselect.cpp

UIS = \
    $(GENSRC)\ui_EdtQtDeviceDlg.h \
    $(GENSRC)\ui_HelpDlg.h \
    $(GENSRC)\ui_DisplaySkip.h \
    $(GENSRC)\ui_CamSettingsDlg.h \
    $(GENSRC)\ui_CamSerialDlg.h \
    $(GENSRC)\ui_EdtQtPixelDlg.h \
    $(GENSRC)\ui_EdtQtImageWindow.h \
    $(GENSRC)\ui_contrastwidget.h \
    $(GENSRC)\ui_contrastwidget.h \
    $(GENSRC)\ui_histowidget.h \
    $(GENSRC)\ui_profilewidget.h

RESOURCES = \
    $(GENSRC)\qrc_pdv_qtlib.cpp \
    $(GENSRC)\qrc_qtcamselect.cpp

INCLUDES = $(INCLUDES) \
    AppSettings.h \
    CamSelectTree.h \
    CamSelectTreeItem.h \
    DevInfoMsgBox.h \
    DevSelectTree.h \
    DevSelectTreeItem.h \
    DisplayContrast.h \
    EdtQtContrastCanvas.h \
    EdtQtHistoCanvas.h \
    EdtQtHistogramItem.h \
    EdtQtGraphicsItem.h \
    EdtQtImageItem.h \
    EdtQtImageScene.h \
    EdtQtImageView.h \
    EdtQtImageViewer.h \
    EdtQtImageWindow.h \
    EdtQtFileInfo.h \
    EdtQtLiveDisplay.h \
    EdtQtPlotCanvas.h \
    EdtQtPlotCurve.h \
    EdtQtPlotView.h \
    EdtQtProfileCanvas.h \
    EdtIntervalData.h \
    contrastwidget.h \
    histowidget.h \
    profilewidget.h \
    qtcamselect.h

TARGETLIB=$(LIBDIR)\pdv_qtlib.lib
TARGETDLL=$(LIBDIR)\pdv_qtlib.dll

$(TARGETLIB): dir uis resources mocs $(LIBOBJECTS) $(INCLUDES)  
	 link /lib /out:$(TARGETLIB) $(LIBOBJECTS)

mocs: $(MOCS)

uis: $(UIS)

resources: $(RESOURCES)

dir:
    @echo INTDIR $(INTDIR)
    @echo GENSRC $(INTDIR)
	@if not exist "$(INTDIR)/" mkdir $(INTDIR)
	@if not exist "$(GENSRC)/" mkdir $(GENSRC)

clean: objclean
 
objclean:
	-del $(LIBOBJECTS) 2>nul
    -del $(TARGETLIB) 2>nul
    -del $(TARGETDLL) 2>nul
    -del $(MOCS) 2>nul
    -del $(UIS) 2>nul
    -del $(RESOURCES) 2>nul
    -del /Q GeneratedFiles\* 2>nul
    -del /Q Releaseamd64\* 2>nul
    -del /Q x64\Release\* 2>nul
    -del /Q Releasex86\* 2>nul
    -del /Q x86\Release\* 2>nul

{}.c{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

{}.cpp{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

{$(GENSRC)}.cpp{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

# QT UIs, MOCs and Resources -- please can we use inference rules here? guess not with
# stupid MS rules since they don't (?) allow for prefix (ui_, moc_ etc.) and extension :-(
#
# UIs
#
$(GENSRC)\ui_EdtQtImageWindow.h: EdtQtImageWindow.h EdtQtImageWindow.ui
    $(QTUIC) EdtQtImageWindow.ui -o $@

$(GENSRC)\ui_HelpDlg.h: HelpDlg.h HelpDlg.ui
    $(QTUIC) HelpDlg.ui -o $@

$(GENSRC)\ui_DisplaySkip.h: DisplaySkip.h DisplaySkip.ui
    $(QTUIC) DisplaySkip.ui -o $@

$(GENSRC)\ui_CamSettingsDlg.h: CamSettingsDlg.h CamSettingsDlg.ui
    $(QTUIC) CamSettingsDlg.ui -o $@

$(GENSRC)\ui_CamSerialDlg.h: CamSerialDlg.h CamSerialDlg.ui
    $(QTUIC) CamSerialDlg.ui -o $@

$(GENSRC)\ui_EdtQtPixelDlg.h: EdtQtPixelDlg.h EdtQtPixelDlg.ui
    $(QTUIC) EdtQtPixelDlg.ui -o $@

$(GENSRC)\ui_EdtQtDeviceDlg.h: EdtQtDeviceDlg.h EdtQtDeviceDlg.ui
    $(QTUIC) EdtQtDeviceDlg.ui -o $@

$(GENSRC)\ui_contrastwidget.h: contrastwidget.h contrastwidget.ui
    $(QTUIC) contrastwidget.ui -o $@

$(GENSRC)\ui_histowidget.h: histowidget.h histowidget.ui
    $(QTUIC) histowidget.ui -o $@

$(GENSRC)\ui_profilewidget.h: profilewidget.h profilewidget.ui
    $(QTUIC) profilewidget.ui -o $@

$(GENSRC)\ui_qtcamselect.h: qtcamselect.h qtcamselect.ui
    $(QTUIC) qtcamselect.ui -o $@

$(GENSRC)\ui_edtqtplotcurve.h: EdtQtPlotCurve.h edtqtplotcurve.ui
    $(QTUIC) edtqtplotcurve.ui -o $@


# MOCs
#
$(GENSRC)\moc_CamSettingsDlg.cpp: CamSettingsDlg.h
    $(QTMOC) $(DEFINES) $(INCPATH) CamSettingsDlg.h -o $@

$(GENSRC)\moc_DisplaySkip.cpp: DisplaySkip.h
    $(QTMOC) $(DEFINES) $(INCPATH) DisplaySkip.h -o $@

$(GENSRC)\moc_HelpDlg.cpp: HelpDlg.h
    $(QTMOC) $(DEFINES) $(INCPATH) HelpDlg.h -o $@

$(GENSRC)\moc_CamSerialDlg.cpp: CamSerialDlg.h
    $(QTMOC) $(DEFINES) $(INCPATH) CamSerialDlg.h -o $@

$(GENSRC)\moc_DevSelectTree.cpp: DevSelectTree.h
    $(QTMOC) $(DEFINES) $(INCPATH) DevSelectTree.h -o $@

$(GENSRC)\moc_EdtQtHistoCanvas.cpp: EdtQtHistoCanvas.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtHistoCanvas.h -o $@

$(GENSRC)\moc_EdtQtHistogramItem.cpp: EdtQtHistogramItem.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtHistogramItem.h -o $@

$(GENSRC)\moc_EdtQtGraphicsItem.cpp: EdtQtGraphicsItem.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtGraphicsItem.h -o $@

$(GENSRC)\moc_DevSelectTreeItem.cpp: DevSelectTreeItem.h
    $(QTMOC) $(DEFINES) $(INCPATH) DevSelectTreeItem.h -o $@

$(GENSRC)\moc_CamSelectTree.cpp: CamSelectTree.h
    $(QTMOC) $(DEFINES) $(INCPATH) CamSelectTree.h -o $@

$(GENSRC)\moc_CamSelectTreeItem.cpp: CamSelectTreeItem.h
    $(QTMOC) $(DEFINES) $(INCPATH) CamSelectTreeItem.h -o $@

$(GENSRC)\moc_EdtQtContrastCanvas.cpp: EdtQtContrastCanvas.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtContrastCanvas.h -o $@

$(GENSRC)\moc_EdtQtImageItem.cpp: EdtQtImageItem.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtImageItem.h -o $@

$(GENSRC)\moc_EdtQtImageScene.cpp: EdtQtImageScene.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtImageScene.h -o $@

$(GENSRC)\moc_EdtQtImageView.cpp: EdtQtImageView.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtImageView.h -o $@

$(GENSRC)\moc_EdtQtImageViewer.cpp: EdtQtImageViewer.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtImageViewer.h -o $@

$(GENSRC)\moc_EdtQtImageWindow.cpp: EdtQtImageWindow.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtImageWindow.h -o $@

$(GENSRC)\moc_contrastwidget.cpp: contrastwidget.h
    $(QTMOC) $(DEFINES) $(INCPATH) contrastwidget.h -o $@

$(GENSRC)\moc_histowidget.cpp: histowidget.h
    $(QTMOC) $(DEFINES) $(INCPATH) histowidget.h -o $@

$(GENSRC)\moc_profilewidget.cpp: profilewidget.h
    $(QTMOC) $(DEFINES) $(INCPATH) profilewidget.h -o $@

$(GENSRC)\moc_qtcamselect.cpp: qtcamselect.h
    $(QTMOC) $(DEFINES) $(INCPATH) qtcamselect.h -o $@

$(GENSRC)\moc_EdtQtPlotCurve.cpp: EdtQtPlotCurve.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtPlotCurve.h -o $@

$(GENSRC)\moc_EdtQtPlotView.cpp: EdtQtPlotView.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtPlotView.h -o $@

$(GENSRC)\moc_EdtQtProfileCanvas.cpp: EdtQtProfileCanvas.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtProfileCanvas.h -o $@

$(GENSRC)\moc_EdtQtDeviceDlg.cpp: EdtQtDeviceDlg.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtDeviceDlg.h -o $@

$(GENSRC)\moc_EdtQtPixelDlg.cpp: EdtQtPixelDlg.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtPixelDlg.h -o $@

$(GENSRC)\moc_EdtQtPixelTable.cpp: EdtQtPixelTable.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtPixelTable.h -o $@

$(GENSRC)\moc_EdtQtPlotCanvas.cpp: EdtQtPlotCanvas.h
    $(QTMOC) $(DEFINES) $(INCPATH) EdtQtPlotCanvas.h -o $@



# RESOURCES
#
# don't know that we need -n-compress but makes it match the one that's generated when with the VCC QT plugin
#
QTRCCFLAGS= -no-compress

$(GENSRC)\qrc_pdv_qtlib.cpp: pdv_qtlib.qrc
    $(QTRCC) $(QTRCCFLAGS) -name pdv_qtlib pdv_qtlib.qrc -o $@

$(GENSRC)\qrc_qtcamselect.cpp: qtcamselect.qrc
    $(QTRCC) $(QTRCCFLAGS) -name qtcamselect qtcamselect.qrc -o $@

