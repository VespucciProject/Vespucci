#    Copyright (C) 2015 Daniel P. Foose - All Rights Reserved

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
##############               LibVespucci Qt Profile               ##############
################################################################################
# The configuration for windows assumes that you have downloaded the compiled
# windows libraries from the MinGW_libs branch of the Vespucci repository.
# To use these libraries, you must be using 64-bit MinGW-w64 toolkit, with SEH
# for exception handling. All libraries must be compiled in such a manner.
# A link to download the exact compiler build I used is available on the GitHub
# page.
# R- and Octave-related libraries will be based on your installation of those
# tools.


# Configuration settings for unix systems are based on my own personal environment
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

QT       += core gui
QT       += widgets printsupport
QT       += svg
CONFIG   += shared debug_and_release c++11
#previously we handled this as a framework, but we're building dylib files now
#mac: CONFIG += lib_bundle
# Set the installation directory
isEmpty(PREFIX) {
    PREFIX = $$PWD/../../Vespucci-install
}
travis_ci = $$(TRAVIS_CI)
deploy_win64 = $$(DEPLOY_WIN64)
develop_win64 = $$(DEVELOP_WIN64)
# it is assumed that casual windows users will not use the build system to install
TARGET = vespucci
TEMPLATE = lib
DEFINES += VESPUCCI_LIBRARY

#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.
unix:!macx: QMAKE_CXXFLAGS += -std=c++0x \
                        -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/Rcpp/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.1/RInside/include" \
                        -isystem "/usr/share/R/include" \
                        -isystem /usr/include \
                        -isystem /usr/local/include \
                        -isystem /usr/local/include/cminpack-1 \
                        -isystem /usr/local/opt/libxml2/include/libxml2 \
                        -isystem $$PWD/../../Vespucci-QCP-sharedlib/include \
                        -isystem $$PWD/include
#Warnings in boost library not suppressed with -isystem in Clang++. These are always
macx: QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
macx: QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter


macx: CONFIG +=c++11


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
    src/Math/Fitting/linleastsq.cpp \
    #src/Math/Fitting/nonlinleastsq.cpp \
    src/Global/vespucci.cpp \
    src/Math/Transform/fft.cpp \
    src/Math/Quantification/correlation.cpp \
    src/Math/Quantification/quantification.cpp \
    src/Math/Stats/confidenceinterval.cpp \
    src/Math/Stats/hyptothesistests.cpp \
    src/Math/PeakFinding/kernelpeakfinding.cpp


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
    include/Math/Fitting/linleastsq.h \
    #include/Math/Fitting/nonlinleastsq.h \
    include/Global/vespucci.h \
    include/Global/libvespucci.h \
    include/Math/Transform/fft.h \
    include/Math/Smoothing/FIR.h \
    include/Math/Smoothing/whittaker.h \
    include/Math/Smoothing/nonlinear.h \
    include/Math/Quantification/integration.h \
    include/Math/Quantification/maximum.h \
    include/Math/Quantification/bandwidth.h \
    include/Math/Quantification/correlation.h \
    include/Math/Stats/confidenceinterval.h \
    include/Math/Stats/hypothesistests.h \
    include/Math/PeakFinding/kernelpeakfinding.h


#linux and mac osx libraries, specific to my own install. This will be handled by CMake later
#I hope...
#For these paths to work, everything except for armadillo, mlpack and cminpack
#should be installed using the package manager for your distribution
# (these work for ubuntu and for the homebrew mac os package manager).
# include paths a

unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include
unix:!macx: INCLUDEPATH += /usr/local/include
unix:!macx: DEPENDPATH += /usr/local/include
unix:macx: INCLUDEPATH += /usr/local/opt/libxml2/include/libxml2
unix:macx: DEPENDPATH += /usr/local/opt/libxml2/include/libxml2


INCLUDEPATH += $$PWD/../../Vespucci-QCP-sharedlib/include
DEPENDPATH += $$PWD/../../Vespucci-QCP-sharedlib/include

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
unix:!macx: INCLDEPATH += /usr/include/cminpack-1
unix:!macx: DEPENDPATH += /usr/include/cminpack-1
#mlpack and dependencies
#we use the Accelerate Framework on OS X but OpenBLAS on linux.


#travis-ci builds
count(travis_ci, 1){
    QMAKE_CXX=/usr/bin/g++-4.9
    unix:!macx: LIBS += -L/home/travis/depts/lib -lmlpack
    unix:!macx: LIBS += -L/home/travis/depts/lib -larmadillo
    unix:!macx: LIBS += -L/usr/lib -larpack
    unix:!macx: PRE_TARGETDEPS += /usr/lib/libarpack.a
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu -lhdf5
    unix:!macx: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libhdf5.a
    unix:!macx: LIBS += -L/usr/lib/ -lcminpack
    unix:!macx: LIBS += -L/usr/lib -lblas
    unix:!macx: LIBS += -L/usr/lib -llapack
    unix:!macx: INCLUDEPATH += /home/travis/depts/include
    unix:!macx: DEPENDPATH += /home/travis/depts/include
    unix:!macx: INCLUDEPATH += /home/travis/depts/include/armadillo_bits
    unix:!macx: DEPENDPATH += /home/travis/depts/include/armadillo_bits
    unix:!macx: INCLUDEPATH += /usr/include/cminpack-1
    unix:!macx: DEPENDPATH += /usr/include/cminpack-1
}
count(travis_ci, 0){
    unix:!macx: LIBS += -L/usr/local/lib -lmlpack
    unix:!macx: LIBS += -L/usr/lib -larmadillo
    unix:!macx: LIBS += -L/usr/local/lib -larpack
    unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libarpack.a
    unix:!macx: LIBS += -L/usr/local/lib -lhdf5
    unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libhdf5.a
    unix:!macx: LIBS += -L/usr/local/lib64/ -lcminpack
    unix:!macx: PRE_TARGETDEPS += /usr/local/lib64/libcminpack.a
    unix:!macx: LIBS += -L/usr/lib -lopenblas
    unix:!macx: PRE_TARGETDEPS += /usr/lib/libopenblas.a
    unix:!macx: LIBS += -L/usr/local/lib64/ -lcminpack
    unix:!macx: PRE_TARGETDEPS += /usr/local/lib64/libcminpack.a
    unix:!macx: INCLUDEPATH += /usr/local/include/cminpack-1
    unix:!macx: DEPENDPATH += /usr/local/include/cminpack-1
}


unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include

unix:!macx: INCLUDEPATH += /usr/include/libxml2
unix:!macx: DEPENDPATH += /usr/include/libxml2

#MSVC for deployment (used for build using static qt and all libraries compiled in msvc linked statically)
count(deploy_win64, 1){
    LIBS += -L$$PWD/../../MSVC_deployment_deps/MLPACK/ -lmlpack
    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/MLPACK/include
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/MLPACK/include
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/MLPACK/mlpack.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/Armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/Armadillo/include
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/Armadillo/include
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/Armadillo/lib/armadillo.lib

    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/boost_1_60_0
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/boost_1_60_0

    LIBS += -L$$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_math_c99-vc140-mt-s-1_60
    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/boost_1_60_0
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/boost_1_60_0
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_math_c99-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_serialization-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_serialization-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_random-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_random-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_unit_test_framework-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_unit_test_framework-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_program_options-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_program_options-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/LAPACK/ -llapack_x64
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/LAPACK/lapack_x64.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/OpenBLAS/ -llibopenblas
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/OpenBLAS/libopenblas.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/ARPACK/ -larpack_x64
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/ARPACK/arpack_x64.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/cminpack/ -lcminpack
    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/cminpack
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/cminpack
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/cminpack/cminpack.lib
}

count(develop_win64, 1){
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../../MSVC_development_deps/MLPACK/Debug -lmlpack
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../MSVC_development_deps/MLPACK/Release -lmlpack
    INCLUDEPATH += $$PWD/../../MSVC_development_deps/MLPACK/include
    DEPENDPATH += $$PWD/../../MSVC_development_deps/MLPACK/include
    CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/MLPACK/Debug/mlpack.lib
    CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/MLPACK/Release/mlpack.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/Armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../../MSVC_development_deps/Armadillo/include
    DEPENDPATH += $$PWD/../../MSVC_development_deps/Armadillo/include
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/Armadillo/lib/armadillo.lib

    INCLUDEPATH += $$PWD/../../MSVC_development_deps/boost_1_60_0
    DEPENDPATH += $$PWD/../../MSVC_development_deps/boost_1_60_0

    LIBS += -L$$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_math_c99-vc140-mt-s-1_60
    INCLUDEPATH += $$PWD/../../MSVC_development_deps/boost_1_60_0
    DEPENDPATH += $$PWD/../../MSVC_development_deps/boost_1_60_0
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/libboost_math_c99-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_serialization-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/libboost_serialization-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_random-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/libboost_random-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_unit_test_framework-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/libboost_unit_test_framework-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_program_options-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/boost_1_60_0/lib64-msvc-14.0/libboost_program_options-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/LAPACK/ -llapack_x64
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/LAPACK/lapack_x64.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/OpenBLAS/ -llibopenblas
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/OpenBLAS/libopenblas.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/ARPACK/ -larpack_x64
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/ARPACK/arpack_x64.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/cminpack/ -lcminpack
    INCLUDEPATH += $$PWD/../../MSVC_development_deps/cminpack
    DEPENDPATH += $$PWD/../../MSVC_development_deps/cminpack
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/cminpack/cminpack.lib
}


#mac libraries. These are the same in Travis-CI as in most local environments
#assuming that all dependencies for armadillo and mlpack are installed using homebrew
mac: LIBS += -L$$PWD/../../mlpack/lib/ -lmlpack
mac: INCLUDEPATH += $$PWD/../../mlpack/include
mac: DEPENDPATH += $$PWD/../../mlpack/include

mac: LIBS += -L$$PWD/../../armadillo/lib/ -larmadillo
mac: INCLUDEPATH += $$PWD/../../armadillo/include
mac: DEPENDPATH += $$PWD/../../armadillo/include

mac: LIBS += -L/usr/local/lib/ -larpack
mac: INCLUDEPATH += /usr/local/include
mac: DEPENDPATH += /usr/local/include

mac: LIBS += -framework Accelerate

mac: LIBS += -L/usr/local/lib/ -lhdf5
mac: PRE_TARGETDEPS += /usr/local/lib/libhdf5.a
