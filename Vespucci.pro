#-------------------------------------------------
#
# Project created by QtCreator 2014-05-05T16:19:03
#
#-------------------------------------------------


#Vespucci makefile
#The configuration settings for Windows are very specific to my configuration
#Configuration for unix (and consequently Linux and MacOS assume the default
#install directory for all external libraries. If you installed them using
#make install or from a package manager then you're probably ok.

#Vespucci depends on some BLAS and LAPACK libraries. On Windows and Linux, I use
#OpenBLAS. On Mac, the Accelerate framework is used by default. If you have some
#other BLAS or LAPACK substitute you would like to use, just add the library and
#its include directories below. Armadillo and MLPACK also depend on BLAS and
#LAPACK.

QT       += core gui
QT       += widgets printsupport
CONFIG   += static
mac: CONFIG += app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vespucci
TEMPLATE = app

mac: QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -Wno-unused-variable
win32: QMAKE_CXXFLAGS += -isystem "C:/Libraries/OpenBLAS/include" \
               -isystem "C:/Libraries/armadillo/include" \
        -isystem "C:/Libraries/mlpack/include" \
    -isystem "C:/Boost/include/" \
    -isystem "C:/QtSDK/include/"

#Libraries
#OpenBLAS
win32: LIBS += -L$$PWD/../../../Libraries/OpenBLAS/lib/ -lopenblas
win32: INCLUDEPATH += $$PWD/../../../Libraries/OpenBLAS/include
win32: DEPENDPATH += $$PWD/../../../Libraries/OpenBLAS/include
win32: PRE_TARGETDEPS += $$PWD/../../../Libraries/OpenBLAS/lib/libopenblas.a



#ARPACK
win32: LIBS += -L$$PWD/../../../../../Libraries/ARPACK/ -larpack_win64
win32: INCLUDEPATH += $$PWD/
win32: DEPENDPATH += $$PWD/
win32: PRE_TARGETDEPS += $$PWD/../../../../../Libraries/ARPACK/libarpack_win64.a

#gfortran
win32: LIBS += -L$$PWD/../../../../../QtSDK/lib/gcc/x86_64-w64-mingw32/4.8.2/ -lgfortran
win32: INCLUDEPATH += $$PWD/../../../../../QtSDK/lib/gcc/x86_64-w64-mingw32/4.8.2
win32: DEPENDPATH += $$PWD/../../../../../QtSDK/lib/gcc/x86_64-w64-mingw32/4.8.2
win32: PRE_TARGETDEPS += $$PWD/../../../../../QtSDK/lib/gcc/x86_64-w64-mingw32/4.8.2/libgfortran.a

#MLPACK
win32: LIBS += -L$$PWD/../../../../../Libraries/mlpack/lib/ -lmlpack
win32: INCLUDEPATH += C:/Libraries/mlpack/include
win32: DEPENDPATH += C:/Libraries/mlpack/include
win32: PRE_TARGETDEPS += $$PWD/../../../../../Libraries/mlpack/lib/libmlpack.a

#Boost win32: LIBS
#Boost include paths
win32: INCLUDEPATH += $$PWD/../../../../../Boost/include/
win32: DEPENDPATH += $$PWD/../../../../../Boost/include/
#Boost math
win32: LIBS += -L$$PWD/../../../../../Boost/lib/ -lboost_math_c99-mgw48-mt-1_55
win32: PRE_TARGETDEPS += $$PWD/../../../../../Boost/lib/libboost_math_c99-mgw48-mt-1_55.a
#Boost program options
win32: LIBS += -L$$PWD/../../../../../Boost/lib/ -lboost_program_options-mgw48-mt-1_55
win32: PRE_TARGETDEPS += $$PWD/../../../../../Boost/lib/libboost_program_options-mgw48-mt-1_55.a
#Boost random
win32: LIBS += -L$$PWD/../../../../../Boost/lib/ -lboost_random-mgw48-mt-1_55
win32: PRE_TARGETDEPS += $$PWD/../../../../../Boost/lib/libboost_random-mgw48-mt-1_55.a
#Boost test
win32: LIBS += -L$$PWD/../../../../../Boost/lib/ -lboost_unit_test_framework-mgw48-mt-1_55
win32: PRE_TARGETDEPS += $$PWD/../../../../../Boost/lib/libboost_unit_test_framework-mgw48-mt-1_55.a

#LibXML2 (a MLPACK dependency)
win32: LIBS += -L$$PWD/../../../../../QtSDK/lib/ -lxml2
win32: INCLUDEPATH += $$PWD/../../../../../QtSDK/include/libxml2
win32: DEPENDPATH += $$PWD/../../../../../QtSDK/include/libxml2
win32: PRE_TARGETDEPS += $$PWD/../../../../../QtSDK/lib/libxml2.a

#QCustomPlot
win32: LIBS += -L$$PWD/../../Libraries/QCustomPlot/lib/release/ -lqcustomplot1
win32: INCLUDEPATH += $$PWD/../../Libraries/QCustomPlot/include
win32: DEPENDPATH += $$PWD/../../Libraries/QCustomPlot/include
win32: PRE_TARGETDEPS += $$PWD/../../Libraries/QCustomPlot/lib/release/libqcustomplot1.a


#Armadillo
win32: LIBS += -L$$PWD/../../../../../Libraries/armadillo/lib/ -larmadillo
win32: INCLUDEPATH += C:/Libraries/armadillo/include
win32: DEPENDPATH += C:/Libraries/armadillo/include
win32: PRE_TARGETDEPS += $$PWD/../../../../../Libraries/armadillo/lib/libarmadillo.a












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



mac: LIBS += -framework Accelerate



