#    Copyright (C) 2015 Wright State University - All Rights Reserved
#    Daniel P. Foose, author
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
# Default install paths for armadillo and mlpack (from make install).
# Others based on Ubuntu 15.04 package manager install directories
# Libvespucci should be built and linked.
# Vespucci-QCP should be checked out, built and linked.
QT       += core gui
QT       += widgets printsupport
QT       += svg
CONFIG   += static
mac: CONFIG += app_bundle
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

mac: ICON = vespuccilogo.icns
# Set the installation directory
isEmpty(PREFIX) {
    PREFIX = $$PWD/../../Vespucci-install
}
# it is assumed that casual windows users will not use the build system to install
mac: TARGET = Vespucci #Vespucci.app (which can be installed in Applications) for mac.
!mac: TARGET = vespucci #vespucci or vespucci.exe on linux, windows.
TEMPLATE = app
LIBS_USED_FOR_QT = QtCore QtSvg QtPrintSupport QtWidgets QtGui
for(somelib, $$list($$LIBS_USED_FOR_QT)) {
    mac: QMAKE_CXXFLAGS += -isystem $$(QTDIR)/lib/$${somelib}.framework/Versions/5/Headers/
    mac: QMAKE_CXXFLAGS += -isystem $$(QTDIR)/lib/$${somelib}.framework/Headers/
}
mac: QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override

#Boost, MLPACK, and Armadillo have code that produces warnings. Change the directory as appropriate.
unix:!macx: QMAKE_CXXFLAGS += -std=c++11 \
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
                        -isystem $$PWD/include \
                        -DARMA_DONT_USE_WRAPPER
mac: QMAKE_CXXFLAGS =  -mmacosx-version-min=10.7 \
                       -stdlib=libc++ -std=c++11 \
                       --system-header-prefix=/usr/ \
                       --system-header-prefix=$$PWD/../Vespucci-QCP-sharedlib/include \
                        -isystem "/usr/local/include" \
                        -isystem "/usr/local/include/armadillo_bits" \
                        -isystem "/usr/local/include/boost" \
                        -isystem "/usr/include/mlpack" \
                        -isystem "/usr/share/R/include" \
                        -isystem /usr/include \
                        -isystem /usr/local/include \
                        -isystem /usr/local/include/cminpack-1 \
                        -isystem /usr/local/opt/libxml2/include/libxml2 \
                        -isystem $$PWD/../../Vespucci-QCP-sharedlib/include \
                        -isystem /usr/local/Cellar/qt5/5.5.0/lib/QtPrintSupport.framework/Headers/QtPrintSupport \
                        -DARMA_DONT_USE_WRAPPER
mac: LIBS += -L/usr/lib -lc++

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
    GUI/Analysis/plsdialog.cpp \
    GUI/Analysis/principalcomponentsdialog.cpp \
    GUI/Processing/rangedialog.cpp \
    GUI/Display/scalebardialog.cpp \
    GUI/QAbstractItemModel/spectratablemodel.cpp \
    GUI/Display/spectrumselectiondialog.cpp \
    GUI/Display/spectrumviewer.cpp \
    GUI/Display/statsdialog.cpp \
    GUI/Processing/thresholddialog.cpp \
    GUI/Analysis/univariatedialog.cpp \
    GUI/Analysis/vcadialog.cpp \
    GUI/QAbstractItemModel/vespuccitablemodel.cpp \
    GUI/scriptdialog.cpp \
    Data/Analysis/cwtdata.cpp \
    GUI/Analysis/peakfindingdialog.cpp \
    GUI/Analysis/haspeaksdialog.cpp \
    GUI/Processing/multiimportdialog.cpp \
    GUI/Processing/bulkconversiondialog.cpp \
    GUI/Display/reportmessagedialog.cpp \
    GUI/Processing/abscissatransformdialog.cpp \
    GUI/Processing/fouriertransformdialog.cpp \
    GUI/Display/plotviewer.cpp \
    Data/Import/textimportqpd.cpp \
    GUI/Analysis/classicalleastsquaresdialog.cpp \
    GUI/settingsdialog.cpp \
    GUI/Processing/abscissainterpolationdialog.cpp \
    GUI/Processing/stitchimportdialog.cpp \
    Global/global.cpp \
    #External/interprocesssender.cpp \
    External/fileinterprocess.cpp

HEADERS  += \
    GUI/mainwindow.h \
    Global/vespucciworkspace.h \
    Data/Analysis/analysisresults.h \
    Data/Imaging/mapdata.h \
    Data/Analysis/mlpackpcadata.h \
    Data/Analysis/plsdata.h \
    Data/Analysis/principalcomponentsdata.h \
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
    GUI/Analysis/plsdialog.h \
    GUI/Analysis/principalcomponentsdialog.h \
    GUI/Processing/rangedialog.h \
    GUI/Display/scalebardialog.h \
    GUI/QAbstractItemModel/spectratablemodel.h \
    GUI/Display/spectrumselectiondialog.h \
    GUI/Display/spectrumviewer.h \
    GUI/Display/statsdialog.h \
    GUI/Processing/thresholddialog.h \
    GUI/Analysis/univariatedialog.h \
    GUI/Analysis/vcadialog.h \
    GUI/QAbstractItemModel/vespuccitablemodel.h \
    GUI/Analysis/plotmakerdialog.h \
    GUI/scriptdialog.h \
    Data/Analysis/cwtdata.h \
    GUI/Analysis/peakfindingdialog.h \
    GUI/Analysis/haspeaksdialog.h \
    GUI/Processing/multiimportdialog.h \
    GUI/Processing/bulkconversiondialog.h \
    GUI/Display/reportmessagedialog.h \
    GUI/Processing/abscissatransformdialog.h \
    GUI/Processing/fouriertransformdialog.h \
    GUI/Display/plotviewer.h \
    Data/Import/textimportqpd.h \
    GUI/Analysis/classicalleastsquaresdialog.h \
    GUI/settingsdialog.h \
    GUI/Processing/abscissainterpolationdialog.h \
    GUI/Processing/stitchimportdialog.h \
    Global/global.h \
    ##External/interprocesssender.h \
    External/fileinterprocess.h


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
    GUI/Analysis/plsdialog.ui \
    GUI/Analysis/principalcomponentsdialog.ui \
    GUI/Processing/rangedialog.ui \
    GUI/Display/scalebardialog.ui \
    GUI/Display/spectrumselectiondialog.ui \
    GUI/Display/spectrumviewer.ui \
    GUI/Display/statsdialog.ui \
    GUI/Processing/thresholddialog.ui \
    GUI/Analysis/univariatedialog.ui \
    GUI/Analysis/vcadialog.ui \
    GUI/scriptdialog.ui \
    GUI/Analysis/peakfindingdialog.ui \
    GUI/Analysis/haspeaksdialog.ui \
    GUI/Processing/multiimportdialog.ui \
    GUI/Processing/bulkconversiondialog.ui \
    GUI/Display/reportmessagedialog.ui \
    GUI/Processing/abscissatransformdialog.ui \
    GUI/Processing/fouriertransformdialog.ui \
    GUI/Display/plotviewer.ui \
    GUI/Analysis/classicalleastsquaresdialog.ui \
    GUI/settingsdialog.ui \
    GUI/Processing/abscissainterpolationdialog.ui \
    GUI/Processing/stitchimportdialog.ui

RESOURCES += \
    resources.qrc

RC_ICONS = "vespuccilogo.ico"

#for inclusion of LibVespucci headers
INCLUDEPATH += $$PWD/Library/include
DEPENDPATH += $$PWD/Library/include

#linux libraries, specific to my own install. This will be handled by CMake later
#I hope...

#linux and mac osx libraries, specific to my own install. This will be handled by CMake later
#I hope...
#For these paths to work, everything except for armadillo, mlpack and cminpack
#should be installed using the package manager for your distribution
# (these work for ubuntu and for the homebrew mac os package manager).
# include paths a
unix: INCLUDEPATH += /usr/include
unix: DEPENDPATH += /usr/include
unix: INCLUDEPATH += /usr/local/include
unix: DEPENDPATH += /usr/local/include
unix: INCLUDEPATH += /usr/local/include/cminpack-1
unix: DEPENDPATH += /usr/local/include/cminpack-1
unix:macx: INCLUDEPATH += /usr/local/opt/libxml2/include/libxml2
unix:macx: DEPENDPATH += /usr/local/opt/libxml2/include/libxml2

INCLUDEPATH += $$PWD/../Vespucci-QCP-sharedlib/include
DEPENDPATH += $$PWD/../Vespucci-QCP-sharedlib/include

mac: LIBS += -F$$PWD/../Frameworks/ -framework QCustomPlot

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

#mlpack and dependencies
#we use the Accelerate Framework on OS X but OpenBLAS on linux.
unix: LIBS += -L/usr/local/lib -lmlpack
unix!macx: LIBS += -L/usr/lib -larmadillo
unix: LIBS += -L/usr/local/lib -larpack
unix: PRE_TARGETDEPS += /usr/local/lib/libarpack.a
unix: LIBS += -L/usr/local/lib -lhdf5
unix: PRE_TARGETDEPS += /usr/local/lib/libhdf5.a
unix:!macx: LIBS += -L/usr/lib -lopenblas
unix:!macx: PRE_TARGETDEPS += /usr/lib/libopenblas.a
unix:macx: LIBS += -framework Accelerate

unix:!macx: LIBS += -L/usr/local/lib64/ -lcminpack
unix:!macx: PRE_TARGETDEPS += /usr/local/lib64/libcminpack.a

unix:macx: LIBS += -L/usr/local/lib/ -lcminpack
unix:macx: PRE_TARGETDEPS += /usr/local/lib/libcminpack.a

unix:!macx: CONFIG(release, debug|release): LIBS += -L$$PWD/../Vespucci-QCP/lib/ -lqcustomplot
else:unix:!macx: CONFIG(debug, debug|release): LIBS += -L$$PWD/../Vespucci-QCP/lib/ -lqcustomplotd

unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-VespucciLibrary/release -lvespucci
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-VespucciLibrary/debug -lvespucci
macx:CONFIG(release, debug|release): LIBS += -F$$PWD/../build-VespucciLibrary/release/ -framework Vespucci
macx:CONFIG(debug, debug|release): LIBS += -F$$PWD/../build-VespucciLibrary/debug/ -framework Vespucci

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
unix:!macx: INCLUDEPATH += $$PWD/../../R/x86_64-pc-linux-gnu-library/3.1/RInside/include
unix:!macx: DEPENDPATH += $$PWD/../../R/x86_64-pc-linux-gnu-library/3.1/RInside/include
unix:!macx: INCLUDEPATH += $$PWD/../../R/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include
unix:!macx: DEPENDPATH += $$PWD/../../R/x86_64-pc-linux-gnu-library/3.1/RcppArmadillo/include
unix:!macx: INCLUDEPATH += $$PWD/../../R/x86_64-pc-linux-gnu-library/3.1/Rcpp/include
unix:!macx: DEPENDPATH += $$PWD/../../R/x86_64-pc-linux-gnu-library/3.1/RCpp/include

unix: INCLUDEPATH += $$PWD/../Vespucci-QCP/include
unix: DEPENDPATH += $$PWD/../Vespucci-QCP/include


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


#levmar
win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -llevmar
INCLUDEPATH += $$PWD/../MinGW_libs/include/levmar
DEPENDPATH += $$PWD/../MinGW_libs/include/levmar
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/liblevmar.a

win32: LIBS += -L$$PWD/../MinGW_libs/lib/ -lcminpack
INCLUDEPATH += $$PWD/../MinGW_libs/include/cminpack-1
DEPENDPATH += $$PWD/../MinGW_libs/include/cminpack-1
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libcminpack.a

#libvespucci
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-library/release -lvespucci
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-library/release/libvespucci.a

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-library/debug -lvespucci
win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-library/debug/libvespucci.a
