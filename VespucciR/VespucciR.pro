TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
QMAKE_CXXFLAGS += -std=c++11
#R Integration (these libraries will depend on your R installation)
win32-g++: INCLUDEPATH += C:\Tools\R\R-3.1.3\Library\RCppArmadillo\include
win32-g++: INCLUDEPATH += C:\Tools\R\R-3.1.3\include

win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\library\RCppArmadillo\libs\x64 -lRCppArmadillo
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lR
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRlapack
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRblas
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRiconv
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRzlib
win32-g++: LIBS += -LC:\Tools\R\R-3.1.3\bin\x64 -lRgraphapp


win32: LIBS += -L$$PWD/../../MinGW_libs/lib/ -lhdf5
INCLUDEPATH += $$PWD/../../MinGW_libs/include
DEPENDPATH += $$PWD/../../MinGW_libs/include
win32-g++: PRE_TARGETDEPS += $$PWD/../../MinGW_libs/lib/libhdf5.a

win32: LIBS += -L$$PWD/../../../../Tools/R/RInside/libs/x64/ -lRInside
INCLUDEPATH += $$PWD/../../../../Tools/R/RInside/include
DEPENDPATH += $$PWD/../../../../Tools/R/RInside/include
