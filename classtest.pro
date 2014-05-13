#-------------------------------------------------
#
# Project created by QtCreator 2014-05-05T16:19:03
#
#-------------------------------------------------

QT       += core gui
QT       += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = classtest
TEMPLATE = app

win32: LIBS += -L$$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86-Win/lib/ -llibopenblas

INCLUDEPATH += $$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86-Win/include/
DEPENDPATH += $$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86-Win/include/



INCLUDEPATH += "C:/Libraries/armadillo-4.300.0/include"
DEPENDPATH += "C:/Libraries/armadillo-4.300.0/include"


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    specmap.cpp \
    loaddataset.cpp \
    mapdata.cpp \
    univariatemap.cpp \
    vespucciworkspace.cpp \
    mapviewer.cpp \
    aboutdialog.cpp \
    citationdialog.cpp \
    univariatedialog.cpp \
    spectrumviewer.cpp \
    bandratiodialog.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    specmap.h \
    loaddataset.h \
    mapdata.h \
    univariatemap.h \
    vespucciworkspace.h \
    mapviewer.h \
    aboutdialog.h \
    citationdialog.h \
    univariatedialog.h \
    spectrumviewer.h \
    bandratiodialog.h

FORMS    += mainwindow.ui \
    loaddataset.ui \
    mapviewer.ui \
    aboutdialog.ui \
    citationdialog.ui \
    univariatedialog.ui \
    spectrumviewer.ui \
    bandratiodialog.ui

RESOURCES += \
    resources.qrc


