#-------------------------------------------------
#
# Project created by QtCreator 2014-05-05T16:19:03
#
#-------------------------------------------------

QT       += core gui
QT       += widgets printsupport
CONFIG   += static
mac: CONFIG += app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vespucci
TEMPLATE = app

mac: QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -Wno-unused-variable

win32: INCLUDEPATH += "C:/Libraries/armadillo-4.300.6/include"
win32: DEPENDPATH += "C:/Libraries/armadillo-4.300.6/include"

mac: INCLUDEPATH += "/Users/danielfoose/lib/armadillo-4.300.9/include"
mac: DEPENDPATH += "/Users/danielfoose/lib/armadillo-4.300.9/include"


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
    filterdialog.cpp \
    baselinedialog.cpp \
    dataviewer.cpp \
    cropdialog.cpp \
    arma_ext.cpp \
    vespuccitablemodel.cpp

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
    filterdialog.h \
    cropdialog.h \
    dataviewer.h \
    baselinedialog.h \
    arma_ext.h \
    vespuccitablemodel.h

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
    filterdialog.ui \
    baselinedialog.ui \
    dataviewer.ui \
    cropdialog.ui

RESOURCES += \
    resources.qrc

win32: LIBS += -L$$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86_64-Win/lib/ -llibopenblas

win32: INCLUDEPATH += $$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86_64-Win/include
win32: DEPENDPATH += $$PWD/../../../../../Libraries/OpenBLAS-v0.2.8-x86_64-Win/include

mac: LIBS += -framework Accelerate
