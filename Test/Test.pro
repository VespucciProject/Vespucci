TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    test_dataset_stitching.cpp \
    test_math_accessory.cpp \
    test_ols.cpp \
    test_pls.cpp \
    test_quantification.cpp \
    test_smoothing.cpp \
    test.cpp

#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.
unix: !macx: QMAKE_CXXFLAGS += -std=c++11 \
                        -isystem "/home/dan/libraries/include" \
                        -isystem "/home/dan/libraries/include/armadillo_bits" \
                        -isystem "/home/dan/libraries/include/mlpack" \
                        -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.0/RcppArmadillo/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.0/Rcpp/include" \
                        -isystem "/home/dan/x86_64-pc-linux-gnu-library/3.0/RInside/include" \
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

#for inclusion of LibVespucci headers
INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

#linux libraries, specific to my own install. This will be handled by CMake later
#I hope...

unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include
unix:!macx: INCLUDEPATH += /usr/local/include
unix:!macx: DEPENDPATH += /usr/local/include
unix:!macx: INCLUDEPATH += /usr/local/include/cminpack-1
unix:!macx: DEPENDPATH += /usr/local/include/cminpack-1
unix:!macx: INCLUDEPATH += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RInside/include
unix:!macx: DEPENDPATH += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RInside/include
unix:!macx: INCLUDEPATH += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include
unix:!macx: DEPENDPATH += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include
unix:!macx: INCLUDEPATH += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/Rcpp/include
unix:!macx: DEPENDPATH += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RCpp/include

unix:!macx: INCLUDEPATH += $$PWD/../../Vespucci-QCP-sharedlib/include
unix:!macx: DEPENDPATH += $$PWD/../../Vespucci-QCP-sharedlib/include


#mlpack and dependencies
unix:!macx: LIBS += -L/usr/local/lib/ -lmlpack
unix:!macx: LIBS += -L/usr/lib/ -larmadillo
unix:!macx: LIBS += -L/usr/local/lib/ -larpack
unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libarpack.a
unix:!macx: LIBS += -L/usr/lib/ -lopenblas
unix:!macx: PRE_TARGETDEPS += /usr/lib/libopenblas.a
unix:!macx: LIBS += -L/usr/local/lib64/ -lcminpack
unix:!macx: PRE_TARGETDEPS += /usr/local/lib64/libcminpack.a

unix:!macx: LIBS += -L$$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RInside/lib/ -lRInside
#unix:!macx: LIBS += -L$$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/Rcpp/libs/ -lRcpp
#unix:!macx: LIBS += -L$$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/libs/ -lRCppArmadillo
unix:!macx: LIBS += -L/usr/lib/R/lib/ -lR
unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Vespucci-QCP-sharedlib/lib/ -lqcustomplot
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Vespucci-QCP-sharedlib/lib/ -lqcustomplotd

unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include

unix:!macx: INCLUDEPATH += /usr/include/libxml2
unix:!macx: DEPENDPATH += /usr/include/libxml2


unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-VespucciLibrary-Desktop-Release/ -lvespucci
unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-VespucciLibrary-Desktop-Debug/ -lvespucci

unix:!macx: LIBS += -L$$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RInside/lib/ -lRInside
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../R/x86_64-pc-linux-gnu-library/3.1/RInside/lib/libRInside.a



#Mac Libraries
#include paths
mac: INCLUDEPATH += $$PWD/../../mac_libs/include
mac: DEPENDPATH += $$PWD/../../mac_libs/include
mac: INCLUDEPATH += $$PWD/../../mac_libs/include/libxml2
mac: DEPENDPATH += $$PWD/../../mac_libs/include/libxml2

mac: LIBS += -L$$PWD/../../mac_libs/lib -lmlpack
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libmlpack.dylib

mac: LIBS += -L$$PWD/../../mac_libs/lib -larmadillo
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libarmadillo.dylib

mac: LIBS += -L$$PWD/../../mac_libs/lib -larpack
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libarpack.dylib

mac: LIBS += -framework Accelerate

mac: LIBS += -L$$PWD/../../mac_libs/lib -lboost_math_c99-mt
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libboost_math_c99-mt.dylib

mac: LIBS += -L$$PWD/../../mac_libs/lib -lboost_program_options-mt
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libboost_program_options-mt.dylib

mac: LIBS += -L$$PWD/../../mac_libs/lib -lboost_random-mt
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libboost_random-mt.dylib

mac: LIBS += -L$$PWD/../../mac_libs/lib -lboost_unit_test_framework-mt
mac: PRE_TARGETDEPS += $$PWD/../../mac_libs/lib/libboost_unit_test_framework-mt.dylib

macx: LIBS += -L$$PWD/../../mac_libs/lib/ -lqcustomplot


#Windows Libraries
#Binaries for windows libraries are included in the MinGW_libs branch of the repository
win32: INCLUDEPATH += $$PWD/../../MinGW_libs/include
win32: DEPENDPATH += $$PWD/../../MinGW_libs/include
win32: INCLUDEPATH += $$PWD/../../MinGW_libs/boost/
win32: DEPENDPATH += $$PWD/../../MinGW_libs/boost/

#MLPACK
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lmlpack
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libmlpack.a

#Armadillo
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -larmadillo
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libarmadillo.a

#HDF5
win32: LIBS += -L$$PWD/../../MinGW_libs/lib -lhdf5
wind32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libhdf5.a

#ARPACK-NG
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -larpack
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libarpack.a

#OpenBLAS (linked dynamically because arpack links it dynamically)
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -llibopenblas
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libopenblas.dll.a

#Libgfortran
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lgfortran
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libgfortran.a

#Boost random (C99)
win32: LIBS += -L$$PWD/../../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost math
win32: LIBS += -L$$PWD/../../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost test
win32: LIBS += -L$$PWD/../../MinGW_libs/boost/stage/lib/ -lboost_unit_test_framework-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/boost/stage/lib/libboost_unit_test_framework-mgw49-mt-1_57.a

#Boost program_options
win32: LIBS += -L$$PWD/../../MinGW_libs/boost/stage/lib/ -lboost_program_options-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/boost/stage/lib/libboost_program_options-mgw49-mt-1_57.a

#LibXML2
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lxml2
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libxml2.a

#LibICONV
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -liconv
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libiconv.a

#Zlib
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lz
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libz.a

#The standard C++ library (linked dynmically by openblas)
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lstdc++
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libstdc++.a

#QCustomPlot
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../MinGW_libs/lib/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../MinGW_libs/lib/ -lqcustomplotd
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libqcustomplotd.a

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
win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -llevmar
INCLUDEPATH += $$PWD/../../MinGW_libs/include/levmar
DEPENDPATH += $$PWD/../../MinGW_libs/include/levmar
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/liblevmar.a

win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lcminpack
INCLUDEPATH += $$PWD/../../MinGW_libs/include/cminpack-1
DEPENDPATH += $$PWD/../../MinGW_libs/include/cminpack-1
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libcminpack.a

#libvespucci
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-library/release -lvespucci
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../build/release/libvespucci.a

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-library/debug -lvespucci
win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../build/debug/libvespucci.a

HEADERS += \
    test.h
