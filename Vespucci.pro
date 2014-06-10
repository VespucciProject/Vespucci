#-------------------------------------------------
#
# Project created by QtCreator 2014-05-05T16:19:03
#
#-------------------------------------------------

QT       += core gui
QT       += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vespucci
TEMPLATE = app


INCLUDEPATH += "C:/Libraries/armadillo-4.300.6/include"
DEPENDPATH += "C:/Libraries/armadillo-4.300.6/include"

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    specmap.cpp \
    loaddataset.cpp \
    mapdata.cpp \
    univariatemap.cpp \
    vespucciworkspace.cpp \
    aboutdialog.cpp \
    citationdialog.cpp \
    univariatedialog.cpp \
    spectrumviewer.cpp \
    bandratiodialog.cpp \
    mapviewer.cpp \
    principalcomponentsdata.cpp \
    principalcomponentsdialog.cpp \
    scalebardialog.cpp \
    filterdialog.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    specmap.h \
    loaddataset.h \
    mapdata.h \
    univariatemap.h \
    vespucciworkspace.h \
    aboutdialog.h \
    citationdialog.h \
    univariatedialog.h \
    spectrumviewer.h \
    bandratiodialog.h \
    mapviewer.h \
    principalcomponentsdata.h \
    principalcomponentsdialog.h \
    scalebardialog.h \
    filterdialog.h

FORMS    += mainwindow.ui \
    loaddataset.ui \
    aboutdialog.ui \
    citationdialog.ui \
    univariatedialog.ui \
    spectrumviewer.ui \
    bandratiodialog.ui \
    mapviewer.ui \
    principalcomponentsdialog.ui \
    scalebardialog.ui \
    filterdialog.ui

RESOURCES += \
    resources.qrc

LIBS += -L$$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86_64-Win/lib/ -llibopenblas

INCLUDEPATH += $$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86_64-Win/include
DEPENDPATH += $$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86_64-Win/include
