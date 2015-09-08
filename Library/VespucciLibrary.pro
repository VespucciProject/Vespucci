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
# A link to download the exact compiler build I used is availible on the GitHub
# page.
# R- and Octave-related libraries will be based on your installation of those
# tools.


# Configuration settings for unix systems are based on my own personal environment
# I use Clang on linux for the diagnostics.

QT       += core gui
QT       += widgets printsupport
QT       += svg
CONFIG   += shared
# Set the installation directory
isEmpty(PREFIX) {
    PREFIX = $$PWD/../../Vespucci-install
}
# it is assumed that casual windows users will not use the build system to install
TARGET = vespucci
TEMPLATE = lib
DEFINES += VESPUCCI_LIBRARY

#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.
unix: !macx: QMAKE_CXXFLAGS += -std=c++11 \
                        -isystem "/home/dan/libraries/include" \
                        -isystem "/home/dan/libraries/include/armadillo_bits" \
                        -isystem "/home/dan/libraries/include/mlpack" \
                        -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/Rcpp/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/RInside/include" \
                        -isystem "/usr/share/R/include"

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



SOURCES +=\
    src/Data/Import/binaryimport.cpp \
    src/Data/Import/textimport.cpp \
    src/Math/Accessory/accessory.cpp \
    src/Math/DimensionReduction/pls.cpp \
    src/Math/DimensionReduction/svds.cpp \
    src/Math/DimensionReduction/VCA.cpp \
    src/Math/Normalization/normalization.cpp \
    src/Math/Quantification/bandwidth.cpp \
    src/Math/Quantification/integration.cpp \
    src/Math/Quantification/maximum.cpp \
    src/Math/Smoothing/FIR.cpp \
    src/Math/Smoothing/nonlinear.cpp \
    src/Math/Smoothing/whittaker.cpp \
    src/Math/Transform/cwt.cpp \
    src/Math/PeakFinding/peakfinding.cpp \
    src/Math/PeakFinding/cwtridge.cpp \
    src/Math/Fitting/linleastsq.cpp \
    src/Math/Fitting/nonlinleastsq.cpp \
    src/Global/vespucci.cpp \
    src/Math/Transform/fft.cpp \
    src/Math/Quantification/correlation.cpp


HEADERS  += \
    include/Global/enums.h \
    include/Data/Import/binaryimport.h \
    include/Data/Import/textimport.h \
    include/Math/VespucciMath.h \
    include/Math/Accessory/accessory.h \
    include/Math/DimensionReduction/dimensionreduction.h \
    include/Math/Normalization/normalization.h \
    include/Math/Quantification/quantification.h \
    include/Math/Smoothing/smoothing.h \
    include/Math/Transform/cwt.h \
    include/Math/PeakFinding/peakfinding.h \
    include/Math/Accessory/accessory_impl.h \
    include/Math/PeakFinding/cwtridge.h \
    include/Math/Fitting/linleastsq.h \
    include/Math/Fitting/nonlinleastsq.h \
    include/Global/vespucci.h \
    include/Global/libvespucci.h \
    include/Math/Transform/fft.h \
    include/Math/Smoothing/FIR.h \
    include/Math/Smoothing/whittaker.h \
    include/Math/Smoothing/nonlinear.h \
    include/Math/Quantification/integration.h \
    include/Math/Quantification/maximum.h \
    include/Math/Quantification/bandwidth.h \
    include/Math/Quantification/correlation.h


#linux libraries, specific to my own install. This will be handled by CMake later
#I hope...

unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include
unix:!macx: INCLUDEPATH += /usr/local/include
unix:!macx: DEPENDPATH += /usr/local/include
unix:!macx: INCLUDEPATH += /usr/local/include/cminpack-1
unix:!macx: DEPENDPATH += /usr/local/include/cminpack-1


INCLUDEPATH += $$PWD/../Vespucci-QCP-sharedlib/include
DEPENDPATH += $$PWD/../Vespucci-QCP-sharedlib/include


#mlpack and dependencies
unix:!macx: LIBS += -L/usr/local/lib/ -lmlpack
unix:!macx: LIBS += -L/usr/lib/ -larmadillo
unix:!macx: LIBS += -L/usr/local/lib/ -larpack
unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libarpack.a
unix:!macx: LIBS += -L/usr/lib/ -lopenblas
unix:!macx: PRE_TARGETDEPS += /usr/lib/libopenblas.a
unix:!macx: LIBS += -L/usr/local/lib64/ -lcminpack
unix:!macx: PRE_TARGETDEPS += /usr/local/lib64/libcminpack.a

unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../Vespucci-QCP-sharedlib/lib/ -lqcustomplot
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Vespucci-QCP-sharedlib/lib/ -lqcustomplotd

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

#HDF5
win32: LIBS += -L$$PWD/../MinGW_libs/lib -lhdf5
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libhdf5.a

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

#CMinpack
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lcminpack
INCLUDEPATH += $$PWD/../MinGW_libs/include/cminpack-1
DEPENDPATH += $$PWD/../MinGW_libs/include/cminpack-1
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libcminpack.a









