#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T11:27:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BatchVCA
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp


#Windows Libraries
#Binaries for windows libraries are included in the MinGW_libs branch of the repository
win32: INCLUDEPATH += $$PWD/../../../MinGW_libs/include
win32: DEPENDPATH += $$PWD/../../../MinGW_libs/include
win32: INCLUDEPATH += $$PWD/../../../MinGW_libs/boost/
win32: DEPENDPATH += $$PWD/../../../MinGW_libs/boost/

#include headers from the Vespucci Project
win32: INCLUDEPATH += $$PWD/../../
win32: DEPENDPATH += $$PWD/../../

#libvespucci
win32: LIBS += -L$$PWD/../../../build-VespucciLibrary-MinGW_w64_4_9_1-Release/release/ -llibvespucci
win32-g++: PRE_TARGETDEPS += $$PWD/../../../build-VespucciLibrary-MinGW_w64_4_9_1-Release/release/liblibvespucci.a

#MLPACK
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -lmlpack
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libmlpack.a

#Armadillo
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -larmadillo
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libarmadillo.a

#HDF5
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib -lhdf5
wind32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libhdf5.a

#ARPACK-NG
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -larpack
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libarpack.a

#OpenBLAS (linked dynamically because arpack links it dynamically)
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -llibopenblas
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libopenblas.dll.a

#Libgfortran
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -lgfortran
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libgfortran.a

#Boost random (C99)
win32: LIBS += -L$$PWD/../../../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost math
win32: LIBS += -L$$PWD/../../../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost test
win32: LIBS += -L$$PWD/../../../MinGW_libs/boost/stage/lib/ -lboost_unit_test_framework-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/boost/stage/lib/libboost_unit_test_framework-mgw49-mt-1_57.a

#Boost program_options
win32: LIBS += -L$$PWD/../../../MinGW_libs/boost/stage/lib/ -lboost_program_options-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/boost/stage/lib/libboost_program_options-mgw49-mt-1_57.a

#LibXML2
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -lxml2
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libxml2.a

#LibICONV
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -liconv
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libiconv.a

#Zlib
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -lz
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libz.a

#The standard C++ library (linked dynmically by openblas)
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -lstdc++
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libstdc++.a


#CMinpack
win32: LIBS += -L$$PWD/../../../MinGW_libs/lib/ -lcminpack
INCLUDEPATH += $$PWD/../../../MinGW_libs/include/cminpack-1
DEPENDPATH += $$PWD/../../../MinGW_libs/include/cminpack-1
win32-g++: PRE_TARGETDEPS += $$PWD/../../../MinGW_libs/lib/libcminpack.a


