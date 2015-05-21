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
unix: !macx: QMAKE_CXXFLAGS += -std=c++11 \
                        -static-libstdc++ \
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
                         -pthread \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/include" \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/boost/boost" \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/include/armadillo_bits" \
                         -isystem "C:/Projects/Vespucci/MinGW_libs/include/mlpack" \
                         -isystem "C:/Tools/R/R-3.1.3/include" \
                         -isystem "C:/Tools/R/R-3.1.3/library/Rcpp/include" \
                         -isystem "C:/Tools/R/R-3.1.3/library/RcppArmadillo/include/armadillo_bits"



SOURCES += main.cpp\
    GUI/mainwindow.cpp \
    Global/vespucciworkspace.cpp \
    Data/Analysis/analysisresults.cpp \
    Data/Imaging/mapdata.cpp \
    Data/Analysis/mlpackpcadata.cpp \
    Data/Analysis/plsdata.cpp \
    Data/Analysis/principalcomponentsdata.cpp \
    Data/Analysis/vcadata.cpp \
    Data/Analysis/univariatedata.cpp \
    Data/Import/binaryimport.cpp \
    Data/Import/textimport.cpp \
    Data/Dataset/metadataset.cpp \
    Data/Dataset/vespuccidataset.cpp \
    GUI/Display/aboutdialog.cpp \
    GUI/Analysis/analysisdialog.cpp \
    GUI/Analysis/bandratiodialog.cpp \
    GUI/Processing/baselinedialog.cpp \
    GUI/Processing/booleanizedialog.cpp \
    GUI/Display/citationdialog.cpp \
    GUI/Processing/cropdialog.cpp \
    GUI/Processing/dataextractordialog.cpp \
    GUI/QAbstractItemModel/datasetlistmodel.cpp \
    GUI/Display/dataviewer.cpp \
    GUI/Processing/filterdialog.cpp \
    GUI/Analysis/kmeansdialog.cpp \
    GUI/Processing/loaddataset.cpp \
    GUI/QAbstractItemModel/maplistmodel.cpp \
    GUI/Display/mapviewer.cpp \
    GUI/Processing/metadatasetdialog.cpp \
    GUI/Analysis/plotmakerdialog.cpp \
    GUI/Display/plotviewer.cpp \
    GUI/Analysis/plsdialog.cpp \
    GUI/Analysis/principalcomponentsdialog.cpp \
    GUI/Processing/rangedialog.cpp \
    GUI/Display/scalebardialog.cpp \
    GUI/QAbstractItemModel/spectratablemodel.cpp \
    GUI/Display/spectrumselectiondialog.cpp \
    GUI/Display/spectrumviewer.cpp \
    GUI/Display/statsdialog.cpp \
    GUI/Processing/thresholddialog.cpp \
    GUI/Analysis/univariateanalysisdialog.cpp \
    GUI/Analysis/univariatedialog.cpp \
    GUI/Analysis/vcadialog.cpp \
    GUI/QAbstractItemModel/vespuccitablemodel.cpp \
    Math/Accessory/accessory.cpp \
    Math/DimensionReduction/pls.cpp \
    Math/DimensionReduction/svds.cpp \
    Math/DimensionReduction/VCA.cpp \
    Math/Normalization/normalization.cpp \
    Math/Quantification/bandwidth.cpp \
    Math/Quantification/integration.cpp \
    Math/Quantification/maximum.cpp \
    Math/Smoothing/FIR.cpp \
    Math/Smoothing/nonlinear.cpp \
    Math/Smoothing/whittaker.cpp \
    Math/Transform/cwt.cpp \
    Math/PeakFinding/peakfinding.cpp \
    Math/Quantification/misc.cpp \
    External/R/VespucciR.cpp \
    External/Octave/VespucciOctave.cpp \
    GUI/scriptdialog.cpp \
    Math/PeakFinding/cwtridge.cpp \
    Data/Analysis/cwtdata.cpp


HEADERS  += \
    Global/enums.h \
    GUI/mainwindow.h \
    Global/vespucciworkspace.h \
    Data/Analysis/analysisresults.h \
    Data/Import/binaryimport.h \
    Data/Imaging/mapdata.h \
    Data/Analysis/mlpackpcadata.h \
    Data/Analysis/plsdata.h \
    Data/Analysis/principalcomponentsdata.h \
    Data/Import/textimport.h \
    Data/Analysis/univariatedata.h \
    Data/Analysis/vcadata.h \
    Data/Dataset/metadataset.h \
    Data/Dataset/vespuccidataset.h \
    GUI/Analysis/analysisdialog.h \
    GUI/Analysis/bandratiodialog.h \
    GUI/Processing/baselinedialog.h \
    GUI/Processing/booleanizedialog.h \
    GUI/Processing/cropdialog.h \
    GUI/Processing/dataextractordialog.h \
    GUI/QAbstractItemModel/datasetlistmodel.h \
    GUI/Display/aboutdialog.h \
    GUI/Display/citationdialog.h \
    GUI/Display/dataviewer.h \
    GUI/Processing/filterdialog.h \
    GUI/Analysis/kmeansdialog.h \
    GUI/Processing/loaddataset.h \
    GUI/QAbstractItemModel/maplistmodel.h \
    GUI/Display/mapviewer.h \
    GUI/Processing/metadatasetdialog.h \
    GUI/Display/plotviewer.h \
    GUI/Analysis/plsdialog.h \
    GUI/Analysis/principalcomponentsdialog.h \
    GUI/Processing/rangedialog.h \
    GUI/Display/scalebardialog.h \
    GUI/QAbstractItemModel/spectratablemodel.h \
    GUI/Display/spectrumselectiondialog.h \
    GUI/Display/spectrumviewer.h \
    GUI/Display/statsdialog.h \
    GUI/Processing/thresholddialog.h \
    GUI/Analysis/univariateanalysisdialog.h \
    GUI/Analysis/univariatedialog.h \
    GUI/Analysis/vcadialog.h \
    GUI/QAbstractItemModel/vespuccitablemodel.h \
    GUI/Analysis/plotmakerdialog.h \
    Math/VespucciMath.h \
    Math/Accessory/accessory.h \
    Math/DimensionReduction/dimensionreduction.h \
    Math/Normalization/normalization.h \
    Math/Quantification/quantification.h \
    Math/Smoothing/smoothing.h \
    Math/Transform/cwt.h \
    Math/PeakFinding/peakfinding.h \
    Math/Accessory/accessory_impl.h \
    External/R/VespucciR.h \
    External/Octave/VespucciOctave.h \
    GUI/scriptdialog.h \
    Math/PeakFinding/cwtridge.h \
    Data/Analysis/cwtdata.h


FORMS    += \
    GUI/mainwindow.ui \
    GUI/Display/aboutdialog.ui \
    GUI/Analysis/analysisdialog.ui \
    GUI/Analysis/bandratiodialog.ui \
    GUI/Processing/booleanizedialog.ui \
    GUI/Processing/baselinedialog.ui \
    GUI/Display/citationdialog.ui \
    GUI/Processing/cropdialog.ui \
    GUI/Processing/dataextractordialog.ui \
    GUI/Display/dataviewer.ui \
    GUI/Processing/filterdialog.ui \
    GUI/Analysis/kmeansdialog.ui \
    GUI/Processing/loaddataset.ui \
    GUI/Display/mapviewer.ui \
    GUI/Processing/metadatasetdialog.ui \
    GUI/Analysis/plotmakerdialog.ui \
    GUI/Display/plotviewer.ui \
    GUI/Analysis/plsdialog.ui \
    GUI/Analysis/principalcomponentsdialog.ui \
    GUI/Processing/rangedialog.ui \
    GUI/Display/scalebardialog.ui \
    GUI/Display/spectrumselectiondialog.ui \
    GUI/Display/spectrumviewer.ui \
    GUI/Display/statsdialog.ui \
    GUI/Processing/thresholddialog.ui \
    GUI/Analysis/univariateanalysisdialog.ui \
    GUI/Analysis/univariatedialog.ui \
    GUI/Analysis/vcadialog.ui \
    GUI/scriptdialog.ui

RESOURCES += \
    resources.qrc

RC_ICONS = "vespuccilogo.ico"


unix:!macx: INCLUDEPATH += $$PWD/../../libraries/include
unix:!macx: DEPENDPATH += $$PWD/../../libraries/include

unix:!macx: LIBS += -L$$PWD/../../libraries/lib/ -llevmar
unix:!macx: PRE_TARGETDEPS += $$PWD/../../libraries/lib/liblevmar.a

unix:!macx: LIBS += -L$$PWD/../../libraries/lib/ -lmlpack
unix:!macx: LIBS += -L$$PWD/../../libraries/lib/ -larmadillo
unix:!macx: LIBS += -L$$PWD/../../libraries/lib/ -larpack
unix:!macx: PRE_TARGETDEPS += $$PWD/../../libraries/lib/libarpack.la

unix:!macx: LIBS += -L$$PWD/../../libraries/lib/ -lopenblas


unix:!macx: INCLUDEPATH += $$PWD/../../libraries/include
unix:!macx: DEPENDPATH += $$PWD/../../libraries/include

unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include

unix:!macx: INCLUDEPATH += /usr/include/libxml2
unix:!macx: DEPENDPATH += /usr/include/libxml2

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
win32: INCLUDEPATH += $$PWD/../MinGW_libs/boost/
win32: DEPENDPATH += $$PWD/../MinGW_libs/boost/

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
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lgfortran
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libgfortran.a

#Boost random (C99)
win32: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost math
win32: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost test
win32: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_unit_test_framework-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_unit_test_framework-mgw49-mt-1_57.a

#Boost program_options
win32: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_program_options-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_program_options-mgw49-mt-1_57.a

#LibXML2
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lxml2
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libxml2.a

#LibICONV
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -liconv
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libiconv.a

#Zlib
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lz
else:win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libz.a

#The standard C++ library (linked dynmically by openblas)
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lstdc++
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libstdc++.a

#QCustomPlot
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../MinGW_libs/lib/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../MinGW_libs/lib/ -lqcustomplotd
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libqcustomplotd.a

#R Integration (these libraries will depend on your R installation)
win32-g++: INCLUDEPATH += C:\Tools\R\R-3.1.3\Library\RInside\include
win32-g++: INCLUDEPATH += C:\Tools\R\R-3.1.3\Library\RCpp\include
win32-g++: INCLUDEPATH += C:\Tools\R\R-3.1.3\Library\RCppArmadillo\include
win32-g++: INCLUDEPATH += C:\Tools\R\R-3.1.3\include

win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\library\RInside\libs\x64 -lRInside
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\library\RCppArmadillo\libs\x64 -lRCppArmadillo
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\library\RCpp\libs\x64 -lRCpp
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lR
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRlapack
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRblas
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRiconv
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRzlib
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRgraphapp

#levmar
#LEVMAR
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -llevmar
INCLUDEPATH += $$PWD/../MinGW_libs/include/levmar
DEPENDPATH += $$PWD/../MinGW_libs/include/levmar
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/liblevmar.a




