TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += test
QT += testlib widgets printsupport svg

SOURCES += \
    testfileio.cpp \
    testhelpers.cpp \
    testquantification.cpp \
    testnormalization.cpp \
    testfitting.cpp \
    testpeakfinding.cpp \
    testdatamodel.cpp \
    testworkspace.cpp \
    testdataset.cpp \
    testsession.cpp \
    testahca.cpp \
    main.cpp

INCLUDEPATH += ../VespucciLibrary/include
DEPENDPATH += ../VespucciLibrary/include
INCLUDEPATH += ../Vespucci
DEPENDPATH += ../Vespucci
#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.

HEADERS += test.h \
    testfileio.h \
    testhelpers.h \
    testquantification.h \
    testnormalization.h \
    testfitting.h \
    testpeakfinding.h \
    testdatamodel.h \
    testworkspace.h \
    testdataset.h \
    testsession.h \
    testahca.h

unix:!macx{
    #QMAKE_CXX=/usr/bin/g++-4.9
    CONFIG += c++11
    QMAKE_CXXFLAGS += -fopenmp
    linux-g++: QMAKE_CXXFLAGS += -fext-numeric-literals

    QMAKE_RPATHDIR += $$(QTDIR)/lib
    QMAKE_LFLAGS += -fopenmp
 
    LIBS += -L$$PWD/../../Vespucci_dependencies/mlpack/lib -lmlpack
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/mlpack/lib/libmlpack.a
    LIBS += -L$$PWD/../../Vespucci_dependencies/armadillo/lib -larmadillo
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/armadillo/lib/libarmadillo.a
    LIBS += -L$$PWD/../../Vespucci_dependencies/hdf5/lib -lhdf5
    LIBS += -L$$PWD/../../Vespucci_dependencies/hdf5/lib -lhdf5_cpp
    LIBS += -L$$PWD/../../Vespucci_dependencies/hdf5/lib -lz
    LIBS += -L$$PWD/../../Vespucci_dependencies/hdf5/lib -lsz
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/hdf5/lib/libhdf5.a
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/hdf5/lib/libhdf5_cpp.a
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/hdf5/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/hdf5/include
    LIBS += -L/usr/lib -lblas
    LIBS += -L/usr/lib -llapack
    
    LIBS += -L$(BOOST_LIB_DIR) -lboost_program_options
    PRE_TARGETDEPS += $(BOOST_LIB_DIR)/libboost_program_options.a

    LIBS += -L$(BOOST_LIB_DIR) -lboost_math_c99
    PRE_TARGETDEPS +=$(BOOST_LIB_DIR)/libboost_math_c99.a

    LIBS += -L$(BOOST_LIB_DIR) -lboost_random
    PRE_TARGETDEPS += $(BOOST_LIB_DIR)/libboost_random.a

    LIBS += -L$(BOOST_LIB_DIR) -lboost_serialization
    PRE_TARGETDEPS += $(BOOST_LIB_DIR)/libboost_serialization.a

    LIBS += -L$(BOOST_LIB_DIR) -lboost_unit_test_framework
    PRE_TARGETDEPS += $(BOOST_LIB_DIR)/libboost_unit_test_framework.a


    LIBS += -L$$PWD/../../Vespucci_dependencies/yaml-cpp/lib -lyaml-cpp
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/yaml-cpp/lib/libyaml-cpp.a
    
    LIBS += -L$$OUT_PWD/../VespucciLibrary -lvespucci

    LIBS += -L/usr/lib/ -lz

    LIBS += -L$$PWD/../../Vespucci_dependencies/EmfEngine/lib/ -lEmfEngine
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/EmfEngine/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/EmfEngine/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/EmfEngine/lib/libEmfEngine.a

    LIBS += -L$$PWD/../../Vespucci_dependencies/libemf/lib/ -lEMF
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/libemf/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/libemf/include
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/libemf/include/libemf
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/libemf/include/libemf
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/libemf/lib/libEMF.a

    INCLUDEPATH += /usr/include/libxml2
    DEPENDPATH += /usr/include/libxml2

    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/mlpack/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/mlpack/include

    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/armadillo/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/armadillo/include

    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/yaml-cpp/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/yaml-cpp/include
}

#mac libraries. These are the same in Travis-CI as in most local environments
#with all dependencies of armadillo and mlpack installed using homebrew
#and armadillo and mlpack installed to the ../armadillo and ../mlpack directories
macx{
    QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc++

    CONFIG += app_bundle c++11
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    QMAKE_CXXFLAGS += --system-header-prefix=/usr \
                      --system-header-prefix=$$PWD/../armadillo \
                      --system-header-prefix=$$PWD/../mlpack \
                      --system-header-prefix=$$PWD/../yaml-cpp \
                      --system-header-prefix=$$PWD/../quazip

    LIBS += -L/usr/lib -lc++

    LIBS += -L$$OUT_PWD/../VespucciLibrary/ -lvespucci
    LIBS += -L$$PWD/../../mlpack/lib/ -lmlpack
    INCLUDEPATH += $$PWD/../../mlpack/include
    DEPENDPATH += $$PWD/../../mlpack/include

    LIBS += -L$$PWD/../../armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../../armadillo/include
    DEPENDPATH += $$PWD/../../armadillo/include

    LIBS += -L/usr/local/lib/ -larpack
    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    LIBS += -framework Accelerate

    LIBS += -L$$PWD/../../hdf5/lib -lhdf5_cpp
    LIBS += -L$$PWD/../../hdf5/lib -lhdf5
    INCLUDEPATH += $$PWD/../../hdf5/include
    DEPENDPATH += $$PWD/../../hdf5/include

    LIBS += -L$$PWD/../../quazip/lib/ -lquazip
    INCLUDEPATH += $$PWD/../../quazip/include
    DEPENDPATH += $$PWD/../../quazip/include
    PRE_TARGETDEPS += $$PWD/../../quazip/lib/libquazip.a

    LIBS += -L$$PWD/../../yaml-cpp/lib/ -lyaml-cpp
    INCLUDEPATH += $$PWD/../../yaml-cpp/include
    DEPENDPATH += $$PWD/../../yaml-cpp/include
    PRE_TARGETDEPS += $$PWD/../../yaml-cpp/lib/libyaml-cpp.a

    LIBS += -lz.1

    INCLUDEPATH += /usr/local/opt/libxml2/include/libxml2
    DEPENDPATH += /usr/local/opt/libxml2/include/libxml2
}

#windows libraries for msvc (we don't currently build the libraries for g++ on windows)
win32:!win32-g++{
    CONFIG += release force_debug_info
    QMAKE_CXXFLAGS += /MP /openmp
    QMAKE_LFLAGS += /LIBPATH:C:\Libraries\boost_1_60_0\lib64-msvc-14.0

    LIBS += -L$$OUT_PWD/../VespucciLibrary/release -llibvespucci
    PRE_TARGETDEPS += $$OUT_PWD/../VespucciLibrary/release/libvespucci.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/mlpack/lib/ -lmlpack
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/mlpack/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/mlpack/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/mlpack/lib/mlpack.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/armadillo/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/armadillo/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/armadillo/lib/armadillo.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/OpenBLAS/ -llibopenblas
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/OpenBLAS/libopenblas.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/LAPACK/ -llapack_x64
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/LAPACK/lapack_x64.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -lhdf5
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/hdf5.lib

    INCLUDEPATH += C:/Libraries/boost_1_60_0
    DEPENDPATH += C:/Libraries/boost_1_60_0

    LIBS += -L$$PWD/../../Vespucci_dependencies/quazip/lib/ -lquazip
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/quazip/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/quazip/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/quazip/lib/quazip.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/yaml-cpp/lib/ -llibyaml-cppmdd
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/yaml-cpp/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/yaml-cpp/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/yaml-cpp/lib/libyaml-cppmdd.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -lzlib
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/zlib.lib

}
