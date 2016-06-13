TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
QT += testlib
QT += widgets
QT += printsupport
QT += svg

SOURCES += main.cpp \
    testfileio.cpp \
    testhelpers.cpp \
    testquantification.cpp \
    testnormalization.cpp \
    testsmoothing.cpp \
    testfitting.cpp \
    testpeakfinding.cpp \
    testdatamodel.cpp \
    testworkspace.cpp \
    testdataset.cpp \
    testsession.cpp
INCLUDEPATH += ../VespucciLibrary/include
DEPENDPATH += ../VespucciLibrary/include
INCLUDEPATH += ../Vespucci
DEPENDPATH += ../Vespucci
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
macx: QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc++

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

HEADERS += test.h \
    testfileio.h \
    testhelpers.h \
    testquantification.h \
    testnormalization.h \
    testsmoothing.h \
    testfitting.h \
    testpeakfinding.h \
    testdatamodel.h \
    testworkspace.h \
    testdataset.h \
    testsession.h

#linux libraries, specific to my own install. This will be handled by CMake later
#I hope...

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

#for inclusion of LibVespucci headers
INCLUDEPATH += $$PWD/../VespucciLibrary/include
DEPENDPATH += $$PWD/../VespucciLibrary/include

count(travis_ci, 1){
  unix:!macx: QMAKE_CXX=/usr/bin/g++-4.9
  unix:!macx: LIBS += -L/home/travis/depts/lib -lmlpack
  unix:!macx: LIBS += -L/home/travis/depts/lib -larmadillo
  unix:!macx: LIBS += -L/usr/lib -larpack
  unix:!macx: PRE_TARGETDEPS += /usr/lib/libarpack.a
  unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu -lhdf5
  unix:!macx: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libhdf5.a
  unix:!macx: LIBS += -L/usr/lib -lcminpack
  unix:!macx: LIBS += -L/usr/lib -lblas
  unix:!macx: LIBS += -L/usr/lib -llapack
  unix:!macx: LIBS += -L/home/travis/depts/lib -lqcustomplot
  unix:!macx: LIBS += -L/home/travis/build/VespucciProject/Vespucci/build-VespucciLibrary -lvespucci
  unix:!macx: INCLUDEPATH += /home/travis/depts/include
  unix:!macx: DEPENDPATH += /home/travis/depts/include
  unix:!macx: INCLUDEPATH += /home/travis/depts/include/armadillo_bits
  unix:!macx: DEPENDPATH += /home/travis/depts/include/armadillo_bits
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
    INCLUDEPATH += $$PWD/../Vespucci-QCP-sharedlib/include
    DEPENDPATH += $$PWD/../Vespucci-QCP-sharedlib/include
    unix:!macx: CONFIG(release, debug|release): LIBS += -L$$PWD/../Vespucci-QCP/lib/ -lqcustomplot
    else:unix:!macx: CONFIG(debug, debug|release): LIBS += -L$$PWD/../Vespucci-QCP/lib/ -lqcustomplotd
    unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-VespucciLibrary/release -lvespucci
    else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-VespucciLibrary/debug -lvespucci
}


unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include

unix:!macx: INCLUDEPATH += /usr/include/libxml2
unix:!macx: DEPENDPATH += /usr/include/libxml2

unix:!macx: INCLUDEPATH += /usr/include
unix:!macx: DEPENDPATH += /usr/include
unix:!macx: INCLUDEPATH += /usr/local/include
unix:!macx: DEPENDPATH += /usr/local/include
unix:!macx: INCLUDEPATH += /usr/local/include/cminpack-1
unix:!macx: DEPENDPATH += /usr/local/include/cminpack-1


#we will no longer support MinGW-w64 for Windows. Users who want to use it will have to
#reckon the config themselves

#MSVC for deployment (used for build using static qt and all libraries compiled in msvc linked statically)
count(deploy_win64, 1){
    LIBS += -L$$PWD/../build-VespucciLibrary/release/ -lvespucci
    PRE_TARGETDEPS += $$PWD/../build-VespucciLibrary/release/vespucci.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/MLPACK/ -lmlpack
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/MLPACK/include
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/MLPACK/include
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/MLPACK/mlpack.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/Armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/Armadillo/include
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/Armadillo/include
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/Armadillo/lib/armadillo.lib

    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/boost_1_60_0
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/boost_1_60_0

    LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_math_c99-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_math_c99-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_serialization-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_serialization-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_random-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_random-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_unit_test_framework-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_unit_test_framework-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_program_options-vc140-mt-s-1_60
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_program_options-vc140-mt-s-1_60.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/LAPACK/ -llapack_x64
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/LAPACK/lapack_x64.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/OpenBLAS/ -llibopenblas
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/OpenBLAS/libopenblas.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/ARPACK/ -larpack_x64
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/ARPACK/arpack_x64.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/cminpack/ -lcminpack
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/cminpack
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/cminpack
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/cminpack/cminpack.lib

    LIBS += -L$$PWD/../MSVC_deployment_deps/QCustomPlot/Release/ -lqcustomplot
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/QCustomPlot
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/QCustomPlot
    PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/QCustomPlot/Release/qcustomplot.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/yaml-cpp/Release/ -llibyaml-cppmt
    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/yaml-cpp/Release
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/yaml-cpp/Release
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/yaml-cpp/Release/libyaml-cppmt.lib

    LIBS += -L$$PWD/../../MSVC_deployment_deps/quazip/Release/ -lquazip
    INCLUDEPATH += $$PWD/../../MSVC_deployment_deps/quazip/Release
    DEPENDPATH += $$PWD/../../MSVC_deployment_deps/quazip/Release
    PRE_TARGETDEPS += $$PWD/../../MSVC_deployment_deps/quazip/Release/quazip.lib
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

    LIBS += -L$$PWD/../MSVC_development_deps/QCustomPlot/Release/ -lqcustomplot
    INCLUDEPATH += $$PWD/../MSVC_development_deps/QCustomPlot
    DEPENDPATH += $$PWD/../MSVC_development_deps/QCustomPlot
    PRE_TARGETDEPS += $$PWD/../MSVC_development_deps/QCustomPlot/Release/qcustomplot.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/yaml-cpp/Release/ -llibyaml-cppmt
    INCLUDEPATH += $$PWD/../../MSVC_development_deps/yaml-cpp/Release
    DEPENDPATH += $$PWD/../../MSVC_development_deps/yaml-cpp/Release
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/yaml-cpp/Release/libyaml-cppmt.lib

    LIBS += -L$$PWD/../../MSVC_development_deps/quazip/Release/ -lquazip
    INCLUDEPATH += $$PWD/../../MSVC_development_deps/quazip/Release
    DEPENDPATH += $$PWD/../../MSVC_development_deps/quazip/Release
    PRE_TARGETDEPS += $$PWD/../../MSVC_development_deps/quazip/Release/quazip.lib
}

#mac libraries. These are the same in Travis-CI as in most local environments
#with all dependencies of armadillo and mlpack installed using homebrew
macx: LIBS += -L$$OUT_PWD/../VespucciLibrary/ -lvespucci

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

macx: LIBS += -L/usr/local/lib/ -lhdf5
macx: PRE_TARGETDEPS += /usr/local/lib/libhdf5.a

mac: LIBS += -F$$PWD/../../Vespucci-QCP/lib/ -framework QCustomPlot
mac: INCLUDEPATH += $$PWD/../../Vespucci-QCP/include
mac: DEPENDPATH += $$PWD/../../Vespucci-QCP/include

macx: LIBS += -L$$PWD/../../quazip/lib/ -lquazip
INCLUDEPATH += $$PWD/../../quazip/include
DEPENDPATH += $$PWD/../../quazip/include
macx: PRE_TARGETDEPS += $$PWD/../../quazip/lib/libquazip.a

macx: LIBS += -L$$PWD/../../yaml-cpp/lib/ -lyaml-cpp
INCLUDEPATH += $$PWD/../../yaml-cpp/include
DEPENDPATH += $$PWD/../../yaml-cpp/include
macx: PRE_TARGETDEPS += $$PWD/../../yaml-cpp/lib/libyaml-cpp.a

macx: LIBS += -lz.1





