TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++11

SOURCES += main.cpp \
    arrayinterface.cpp

INCLUDEPATH += $$PWD/../../../../Octave/Octave-4.0.0/include/octave-4.0.0
DEPENDPATH += $$PWD/../../../../Octave/Octave-4.0.0/include/octave-4.0.0

INCLUDEPATH += $$PWD/../../MinGW_libs/boost/
DEPENDPATH += $$PWD/../../MinGW_libs/boost/

INCLUDEPATH += $$PWD/../../MinGW_libs/include/
DEPENDPATH += $$PWD/../../MinGW_libs/include/

win32: LIBS += -L$$PWD/../../../../Octave/Octave-4.0.0/lib/octave/4.0.0/ -lliboctave.dll
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Octave/Octave-4.0.0/lib/octave/4.0.0/libliboctave.dll.a

win32: LIBS += -L$$PWD/../../../../Octave/Octave-4.0.0/lib/octave/4.0.0/ -lliboctinterp.dll
win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Octave/Octave-4.0.0/lib/octave/4.0.0/libliboctinterp.dll.a

HEADERS += \
    arrayinterface.h


