#    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

#    This file is part of Vespucci.

#    Vespucci is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    Vespucci is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.

#Vespucci Qt Profile
#The configuration settings for Windows are very specific to my configuration
#and very likely will have to be changed. Compiling on Windows is not recommended
#as binary releases for Windows will occur frequently (this is what we use in our
#research group). It took me about a week just to compile all the libraries on
#MinGW and

#Configuration settings for unix systems are based either on the Ubuntu package
#manager or the install scripts of the library when the package is not availible
#from the repository

QT       += core gui
QT       += widgets printsupport
CONFIG   += static
mac: CONFIG += app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vespucci
TEMPLATE = app

unix: QMAKE_CXXFLAGS += -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack"

win32-g++: QMAKE_CXXFLAGS += -isystem "C:/usr/include" \
                         -isystem "C:/usr/include/boost" \
                         -isystem "C:/usr/include/armadillo_bits" \
                         -isystem "C:/usr/include/mlpack"



CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O3

SOURCES += main.cpp\
        mainwindow.cpp \
    loaddataset.cpp \
    mapdata.cpp \
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
    vespuccitablemodel.cpp \
    plsdata.cpp \
    plsdialog.cpp \
    kmeansdialog.cpp \
    vcadata.cpp \
    vcadialog.cpp \
    dataextractordialog.cpp \
    analysiswizard.cpp \
    vespuccidataset.cpp \
    metadataset.cpp \
    plotviewer.cpp

HEADERS  += mainwindow.h \
    loaddataset.h \
    mapdata.h \
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
    vespuccitablemodel.h \
    plsdata.h \
    plsdialog.h \
    kmeansdialog.h \
    vcadata.h \
    vcadialog.h \
    dataextractordialog.h \
    analysiswizard.h \
    vespuccidataset.h \
    metadataset.h \
    plotviewer.h

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
    cropdialog.ui \
    plsdialog.ui \
    kmeansdialog.ui \
    vcadialog.ui \
    dataextractordialog.ui \
    analysiswizard.ui \
    plotviewer.ui

RESOURCES += \
    resources.qrc

RC_ICONS = "vespuccilogo.ico"

#*nix Libraries
#Include paths for *nix
unix: INCLUDEPATH += /usr/include
unix: DEPENDPATH += /usr/include

#BLAS/LAPACK Libraries
unix: !macx: LIBS += /usr/lib/openblas-base/libopenblas.so
unix: !macx: PRE_TARGETDEPS += /usr/lib/openblas-base/libopenblas.so
macx: LIBS += -framework Accelerate


#ARPACK
unix: LIBS += /usr/local/lib/libarpack.so
unix: PRE_TARGETDEPS += /usr/local/lib/libarpack.so

#MLPACK
unix: LIBS += -L/usr/local/lib/libmlpack.a
unix: PRE_TARGETDEPS += /usr/local/lib/libmlpack.a

#Boost path for mac (also default nix path, but not used by ubuntu's package manager)
macx: INCLUDEPATH += /usr/local/include
macx: DEPENDPATH += /usr/local/include

#Boost math
unix: !macx: LIBS += /usr/local/lib/x86_64-linux-gnu/libboost_math_c99.so
unix: !macx: PRE_TARGETDEPS += /usr/local/lib/x86_64-linux-gnu/libboost_math_c99.so

macx: LIBS += /usr/local/lib/libboost_math_c99.so
macx: PRE_TARGETDEPS += /usr/local/lib/libboost_math_c99.so

#Boost program options
unix: !macx: LIBS += /usr/local/lib/x86_64-linux-gnu/libboost_program_options.so
unix: !macx: PRE_TARGETDEPS += /usr/local/lib/x86_64-linux-gnu/libboost_program_options.so

macx: LIBS += /usr/local/lib/libboost_program_options.so
macx: PRE_TARGETDEPS += /usr/local/lib/libboost_program_options.so

#Boost random
unix: !macx: LIBS += /usr/local/lib/x86_64-linux-gnu/libboost_random.so
unix: !macx: PRE_TARGETDEPS += /usr/local/lib/x86_64-linux-gnu/libboost_random.so

macx: LIBS += /usr/local/lib/libboost_random.so
macx: PRE_TARGETDEPS += /usr/local/lib/libboost_random.so

#Boost test
unix: !macx: LIBS += /usr/local/lib/x86_64-linux-gnu/libboost_unit_test_framework.so
unix: !macx: PRE_TARGETDEPS += /usr/local/lib/x86_64-linux-gnu/libboost_unit_test_framework.so

macx: LIBS += /usr/local/lib/libboost_unit_test_framework.so
macx: PRE_TARGETDEPS += /usr/local/lib/libboost_unit_test_framework.so

#LibXML2
unix: !macx: LIBS += /usr/local/lib/x86_64-linux-gnu/libxml2.so
unix: !macx: PRE_TARGETDEPS += /usr/local/lib/x86_64-linux-gnu/libxml2.so
macx: LIBS += -framework libxml2

#QCustomPlot (linking statically for windows)
unix: LIBS += /usr/local/lib/libqcustomplot1.so
unix: PRE_TARGETDEPS += /usr/local/lib/libqcustomplot1.so

#Armadillo
unix: LIBS += /usr/local/lib/libarmadillo.so
unix: PRE_TARGETDEPS += /usr/local/lib/libarmadillo.so

#Windows Libraries
INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

win32: LIBS += -L$$PWD/../../../../usr/lib/ -larmadillo
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libarmadillo.a

win32: LIBS += -L$$PWD/../../../../usr/lib/ -larpack_win64
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libarpack_win64.a


win32: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_math_c99-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libboost_math_c99-mgw48-mt-1_55.a


win32: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_random-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libboost_random-mgw48-mt-1_55.a

win32: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_unit_test_framework-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libboost_unit_test_framework-mgw48-mt-1_55.a

win32: LIBS += -L$$PWD/../../../../usr/lib/ -lgfortran
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libgfortran.a

win32: LIBS += -L$$PWD/../../../../usr/lib/ -lmlpack
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libmlpack.a


win32: LIBS += -L$$PWD/../../../../usr/lib/ -lopenblas
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libopenblas.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/ -lqcustomplotd
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libqcustomplotd.a


win32: LIBS += -L$$PWD/../../../../usr/lib/ -lxml2
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libxml2.a


win32: LIBS += -L$$PWD/../../../../usr/lib/ -liconv
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libiconv.a

win32: LIBS += -L$$PWD/../../../../usr/lib/ -lz
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libz.a


win32: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_program_options-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libboost_program_options-mgw48-mt-1_55.a
