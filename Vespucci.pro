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

################################################################################
##############                Vespucci Qt Profile                 ##############
################################################################################
# The configuration for windows assumes that you have downloaded the compiled
# windows libraries from the MinGW_libs branch of the Vespucci repository.
# To use these libraries, you must be using 64-bit MinGW-w64 toolkit, with SEH
# for exception handling. All libraries must be compiled in such a manner.

# Configuration settings for unix systems are based either on the Ubuntu package
# manager or the install scripts of the library when the package is not availible
# from the repository. I do not regularly compile on

QT       += core gui
QT       += widgets printsupport
CONFIG   += static
mac: CONFIG += app_bundle
mac: ICON = vespuccilogo.icns
# Set the installation directory
isEmpty(PREFIX) {
    PREFIX = $$PWD/../../Vespucci-install
}
# it is assumed that casual windows users will not use the build system to install
TARGET = vespucci
TEMPLATE = app


#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.
unix: !macx: QMAKE_CXXFLAGS += -std=gnu++11 \
                        -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack"
macx: QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+

macx: CONFIG +=c++11

macx: QMAKE_CXXFLAGS += -isystem "/Users/danielfoose/Vespucci/mac_libs/include" \
                        -isystem "/Users/danielfoose/Vespucci/mac_libs/include/armadillo_bits" \
                        -isystem "/Users/danielfoose/Vespucci/mac_libs/include/mlpack" \
                        -isystem "/Users/danielfoose/Vespucci/mac_libs/include/boost"
win32-g++: QMAKE_CXXFLAGS += -std=gnu++11 \
                         -isystem "C:/Projects/Vespucci/branches/MinGW_libs/include" \
                         -isystem "C:/Projects/Vespucci/branches/MinGW_libs/include/boost" \
                         -isystem "C:/Projects/Vespucci/branches/MinGW_libs/include/armadillo_bits" \
                         -isystem "C:/Projects/Vespucci/branches/MinGW_libs/include/mlpack"



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
    vespuccidataset.cpp \
    metadataset.cpp \
    plotviewer.cpp \
    metadatasetdialog.cpp \
    textimport.cpp \
    binaryimport.cpp \
    datasetlistmodel.cpp \
    maplistmodel.cpp

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
    vespuccidataset.h \
    metadataset.h \
    plotviewer.h \
    metadatasetdialog.h \
    textimport.h \
    binaryimport.h \
    datasetlistmodel.h \
    maplistmodel.h

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
    plotviewer.ui \
    metadatasetdialog.ui

RESOURCES += \
    resources.qrc

RC_ICONS = "vespuccilogo.ico"

#*nix (not Mac) Libraries


#Mac Libraries
#include paths
mac: INCLUDEPATH += $$PWD/../mac_libs/include
mac: DEPENDPATH += $$PWD/../mac_libs/include
mac: INCLUDEPATH += $$PWD/../mac_libs/include/libxml2
mac: DEPENDPATH += $$PWD/../mac_libs/include/libxml2

mac: LIBS += -L$$PWD/../mac_libs/lib -lmlpack
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libmlpack.dylib

mac: LIBS += -L$$PWD/../mac_libs/lib -larmadillo
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libarmadillo.dylib

mac: LIBS += -L$$PWD/../mac_libs/lib -larpack
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libarpack.dylib

mac: LIBS += -framework Accelerate

mac: LIBS += -L$$PWD/../mac_libs/lib -lboost_math_c99-mt
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libboost_math_c99-mt.dylib

mac: LIBS += -L$$PWD/../mac_libs/lib -lboost_program_options-mt
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libboost_program_options-mt.dylib

mac: LIBS += -L$$PWD/../mac_libs/lib -lboost_random-mt
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libboost_random-mt.dylib

mac: LIBS += -L$$PWD/../mac_libs/lib -lboost_unit_test_framework-mt
mac: PRE_TARGETDEPS += $$PWD/../mac_libs/lib/libboost_unit_test_framework-mt.dylib

macx: LIBS += -L$$PWD/../mac_libs/lib/ -lqcustomplot


#Windows Libraries
#Binaries for windows libraries are included in the MinGW_libs branch of the repository
win32: INCLUDEPATH += $$PWD/../MinGW_libs/include
win32: DEPENDPATH += $$PWD/../MinGW_libs/include

#MLPACK
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lmlpack
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libmlpack.a

#Armadillo
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -larmadillo
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libarmadillo.a

#ARPACK-NG
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -larpack
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libarpack.a

#OpenBLAS (linked dynamically because arpack links it dynamically)
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -llibopenblas
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libopenblas.dll.a

#Libgfortran
win32: LIBS += -L$$PWD/../MinGW_libs/usr/lib/ -lgfortran
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libgfortran.a

#Boost random (C99)
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lboost_math_c99-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libboost_math_c99-mgw48-mt-1_55.a

#Boost mat
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lboost_random-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libboost_random-mgw48-mt-1_55.a

#Boost test
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lboost_unit_test_framework-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libboost_unit_test_framework-mgw48-mt-1_55.a

#Boost program_options
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lboost_program_options-mgw48-mt-1_55
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libboost_program_options-mgw48-mt-1_55.a

#LibXML2
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lxml2
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libxml2.a

#LibICONV
win32: LIBS += -L$$PWD/../MinGW_libsusr/lib/ -liconv
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libiconv.a

#Zlib
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lz
else:win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libz.a

#The standard C++ library (linked dynmically by openblas)
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lstdc++
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libstdc++.dll.a


#QCustomPlot
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../MinGW_libs/lib/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../MinGW_libs/lib/ -lqcustomplotd
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libqcustomplotd.a





