#
# Windows makefile for vlviewer
#
# Copyright (c) 2013-2016 Engineering Design Team (EDT), Inc.
# All rights reserved.
# 
# This file is subject to the terms and conditions of the EULA defined at
# www.edt.com/terms-of-use
# 

!include ..\makefile.def

PDVHOME=..

# override makefile.def BINDIR if installed not build
!IFNDEF BUILD_ID
BINDIR = $(PDVHOME)
LIBDIR = $(PDVHOME)
!ENDIF

TARGETBASE=vlviewer

GENDIR=GeneratedFiles

TIFFDIR=..\tiff\libtiff

JPEGDIR=..\jpeg-6b

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

#can be overridden on the command line

!IF "$(ARCH)" == "x86"

WSDKLIBDIR="C:\Program Files\Microsoft SDKs\Windows\V6.0A\Lib"
MACHINE=I386

!ELSE

WSDKLIBDIR="C:\Program Files\Microsoft SDKs\Windows\V6.0A\Lib\x64"
MACHINE=X64

!ENDIF

QTLIBDIR=$(PDVHOME)\wlib\$(ARCH)

# this stuff is from vcproj but just including the ones that really are needed here...?
WINLIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

!IF "$(CFG)" == "Debug"

QTLIBS=$(QTLIBDIR)\qtmaind.lib $(QTLIBDIR)\QtCored4.lib $(QTLIBDIR)\QtGuid4.lib
LIBDISPATCH=$(LIBDIR)\dispatch_d.lib
LIBPDVPLUS=$(LIBDIR)\pdvplus_d.lib
LIBPDV=$(LIBDIR)\pdvlib_d.lib
LIBEDTIMAGE=$(LIBDIR)\edtimage_d.lib
LIBIMGFILES=$(LIBDIR)\imgfiles_d.lib
LIBTIFF=$(LIBDIR)\libtiff_d.lib
LIBPDV_QT=$(LIBDIR)\pdv_qtlib_d.lib
LIBJPEG=$(LIBDIR)\libjpeg_d.lib

!ELSE

QTLIBS=$(QTLIBDIR)\qtmain.lib $(QTLIBDIR)\QtCore4.lib $(QTLIBDIR)\QtGui4.lib
LIBDISPATCH=$(LIBDIR)\dispatch.lib
LIBPDVPLUS=$(LIBDIR)\pdvplus.lib
LIBPDV=$(LIBDIR)\pdvlib.lib
LIBEDTIMAGE=$(LIBDIR)\edtimage.lib
LIBIMGFILES=$(LIBDIR)\imgfiles.lib
LIBTIFF=$(LIBDIR)\libtiff.lib
LIBPDV_QT=$(LIBDIR)\pdv_qtlib.lib
LIBJPEG=$(LIBDIR)\libjpeg.lib

!ENDIF

LIBS = $(WINLIBS) $(LIBDISPATCH) $(LIBEDTIMAGE) $(LIBIMGFILES) $(LIBPDV) $(LIBPDVPLUS) $(LIBPDV_QT) $(LIBJPEG) $(LIBTIFF) $(QTLIBS)

QTINCLUDEFLAGS=-I$(QTDIR) -I$(QTDIR)\include -I$(QTDIR)\include\QtCore -I$(QTDIR)\include\qtmain -I$(QTDIR)\include\QtGui

INCLUDEFLAGS = -I$(GENDIR) -I$(PDVHOME) -I$(PDVHOME)\imgfiles -I$(PDVHOME)\dispatch -I$(PDVHOME)\edtimage -I$(PDVHOME)\pdv_qtlib -I$(PDVHOME)\pdv_qtlib\GeneratedFiles -I$(PDVHOME)\pdvplus -I$(JPEGDIR) -I$(TIFFDIR) $(QTINCLUDEFLAGS) 

QTRCC=$(QTDIR)\bin\rcc

INTDIR=$(CFG)$(ARCH)

# .SILENT:

DEFINES= -D$(BUILD_ID) -D"WIN32" -D"_WINDOWS" -D"_AFXEXT" -D"_NT_" -D"_WINDLL" -D"_AFXDLL" 

CFLAGS = $(CFLAGS) /D "_WINDOWS" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_DLL" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "QT_HAVE_MMX" /D "QT_HAVE_3DNOW" /D "QT_HAVE_SSE" /D "QT_HAVE_MMXEXT" /D "QT_HAVE_SSE2" /D "QT_THREAD_SUPPORT" /EHsc /GS- /Zc:wchar_t- /c /errorReport:prompt

!IF "$(CFG)" == "Release"

LINK32_FLAGS= /nologo /subsystem:windows /incremental:no /machine:$(MACHINE) /out:"$(BINDIR)\$(TARGETBASE).exe"

CFLAGS= $(CFLAGS) /D"NDEBUG"

RSC_FLAGS= /l 0x409 /d "_AFXDLL" 

!ELSE

LINK32_FLAGS= /debug /nologo /subsystem:windows /incremental:yes /machine:$(MACHINE) /out:$(BINDIR)\$(TARGETBASE).exe

DEFINES = $(DEFINES) -D"_DEBUG"

RSC_FLAGS= /l 0x409 /d "_DEBUG" /d "_AFXDLL" 

!ENDIF


MOCS = \
    $(GENDIR)\moc_vlviewer.cpp

UIS= \
    $(GENDIR)\ui_vlviewer.h 

RESOURCES = \
    $(INTDIR)\vlviewer.res

LINK32_OBJS= \
    $(INTDIR)\main.obj \
    $(INTDIR)\vlviewer.obj \
    $(INTDIR)\moc_vlviewer.obj \
    $(INTDIR)\vlviewer.res 

LINK32=link.exe

TARGET =$(BINDIR)\$(TARGETBASE).exe
MANIFEST=$(INTDIR)\$(TARGETBASE).exe.manifest

VCLINKFLAGS= /libpath:$(QTLIBDIR) /libpath:$(WSDKLIBDIR) /MANIFEST /MANIFESTFILE:$(MANIFEST) /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /SUBSYSTEM:WINDOWS /NXCOMPAT /ERRORREPORT:PROMPT


all: verbose $(TARGET)

$(TARGETBASE) $(TARGET): dir uis resources mocs $(LIBPDVPLUS) $(LIBIDISPATCH) $(LIBEDTIMAGE) $(LIBIMGFILES) $(LIBPDV_QT) $(LIBTIFF) $(LIBJPEG) $(LIBPDV) objs $(INCLUDES)
	$(LINK32) $(LINK32_FLAGS) $(VCLINKFLAGS) $(LINK32_OBJS) $(LIBS)
    if exist $(MANIFEST) mt -manifest $(MANIFEST) -outputresource:$(TARGET);#1
    if exist $(MANIFEST) del $(MANIFEST)

verbose:
    @echo PDVHOME $(PDVHOME)
    @echo LIBDIR $(LIBDIR)
    @echo BINDIR $(BINDIR)
    @echo INTDIR $(INTDIR)
    @echo QTDIR  $(QTDIR)
    @echo QTLIBDIR $(QTLIBDIR)

{}.c{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

{}.cpp{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

{$(GENDIR)}.cpp{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<


{}.rc{$(INTDIR)}.res:
	rc $(RSC_FLAGS) -fo$*.res $<

{}.ui{$INTDIR}.h:
    uic $< -o $*.h

mocs: $(MOCS)

uis: $(UIS)

resources: $(RESOURCES)

clean: objclean ntclean

dir: $(INTDIR) $(GENDIR)

ntclean: dir
	-del /Q $(INTDIR)\*.*  $(TARGET) GeneratedFiles\* 2>nul

objclean:
    -del /Q $(TARGET) 2>nul
    -del /Q $(MOCS) >NUL 2>nul
    -del /Q $(UIS) >NUL 2>nul
    -del /Q $(RESOURCES) 2>nul
    -del /Q *.ncb 2>nul
    -del /Q *.suo 2>nul
    -del /Q *.pdb 2>nul
    -del /Q *.idb 2>nul
 
$(INTDIR):
    @echo INTDIR $(INTDIR)
	@if not exist "$(INTDIR)/" mkdir $(INTDIR)

$(GENDIR):
    @echo INTDIR $(GENDIR)
	@if not exist "$(GENDIR)/" mkdir $(GENDIR)


objs: $(LINK32_OBJS)


$(GENDIR)\moc_vlviewer.cpp: vlviewer.h
    $(QTMOC) $(DEFINES) -I$(PDVHOME) vlviewer.h -o $@

$(GENDIR)\ui_vlviewer.h: vlviewer.ui
    $(QTUIC) vlviewer.ui -o $@


# RESOURCES
#
# don't know that we need -n-compress but makes it match the one that's generated when with the VCC QT plugin
#
QTRCCFLAGS= -no-compress


