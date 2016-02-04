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
# A link to download the exact compiler build I used is available on the GitHub
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
CONFIG   += static debug_and_release
mac: CONFIG += app_bundle
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

mac: ICON = vespuccilogo.icns
# Set the installation directory
isEmpty(PREFIX) {
    PREFIX = $$PWD/../../Vespucci-install
}
travis_ci = $$(TRAVIS_CI)
deploy_win64 = $$(DEPLOY_WIN64)

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
    GUI/Processing/stitchimportdialog.ui \
    GUI/Analysis/univariatedialog.ui

RESOURCES += \
    resources.qrc

RC_ICONS = "vespuccilogo.ico"


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
unix:macx: INCLUDEPATH += /usr/local/opt/libxml2/include/libxml2
unix:macx: DEPENDPATH += /usr/local/opt/libxml2/include/libxml2
#for inclusion of LibVespucci headers
unix: INCLUDEPATH += $$PWD/Library/include
unix: DEPENDPATH += $$PWD/Library/include

mac: LIBS += -F$$PWD/../Frameworks/ -framework QCustomPlot
mac: INCLUDEPATH += $$PWD/../Vespucci-QCP/include
mac: DEPENDPATH += $$PWD/../Vespucci-QCP/include

INCLUDEPATH += $$PWD/Library/include
DEPENDPATH += $$PWD/Library/include

#Travis-CI automated builds for OS X and Linux
#see .travis.yml for details on how these are installed
count(travis_ci, 1){
  unix:!macx: QMAKE_CXX=/usr/bin/g++-4.8
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
  unix: INCLUDEPATH += /home/travis/depts/include/armadillo_bits
  unix: DEPENDPATH += /home/travis/depts/include/armadillo_bits
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
unix:macx: LIBS += -framework Accelerate






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


#Windows Libraries for MinGW-w64
#Binaries for windows libraries are included in the MinGW_libs branch of the repository
win32-g++: INCLUDEPATH += $$PWD/../MinGW_libs/include
win32-g++: DEPENDPATH += $$PWD/../MinGW_libs/include
win32-g++: INCLUDEPATH += $$PWD/../MinGW_libs/boost/
win32-g++: DEPENDPATH += $$PWD/../MinGW_libs/boost/

#MLPACK
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -lmlpack
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libmlpack.a

#Armadillo
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -larmadillo
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libarmadillo.a

#HDF5
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib -lhdf5
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libhdf5.a

#ARPACK-NG
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -larpack
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libarpack.a

#OpenBLAS (linked dynamically because arpack links it dynamically)
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -llibopenblas
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libopenblas.dll.a

#Libgfortran
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -lgfortran
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libgfortran.a

#Boost random (C99)
win32-g++: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost math
win32-g++: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_math_c99-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_math_c99-mgw49-mt-1_57.a

#Boost test
win32-g++: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_unit_test_framework-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_unit_test_framework-mgw49-mt-1_57.a

#Boost program_options
win32-g++: LIBS += -L$$PWD/../MinGW_libs/boost/stage/lib/ -lboost_program_options-mgw49-mt-1_57
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/boost/stage/lib/libboost_program_options-mgw49-mt-1_57.a

#LibXML2
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -lxml2
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libxml2.a

#LibICONV
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -liconv
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libiconv.a

#Zlib
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -lz
else:win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libz.a

#The standard C++ library (linked dynmically by openblas)
win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -lstdc++
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libstdc++.a

#QCustomPlot
win32-g++:CONFIG(release, debug|release): LIBS += -L$$PWD/../MinGW_libs/lib/ -lqcustomplot
else:win32-g++:CONFIG(debug, debug|release): LIBS += -L$$PWD/../MinGW_libs/lib/ -lqcustomplotd
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libqcustomplotd.a

win32-g++: LIBS += -L$$PWD/../MinGW_libs/lib/ -lcminpack
INCLUDEPATH += $$PWD/../MinGW_libs/include/cminpack-1
DEPENDPATH += $$PWD/../MinGW_libs/include/cminpack-1
win32-g++: PRE_TARGETDEPS += $$PWD/../MinGW_libs/lib/libcminpack.a

#libvespucci
win32-g++:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-VespucciLibrary/release -lvespucci
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-VespucciLibrary/release/libvespucci.a

win32-g++:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-VespucciLibrary/debug -lvespucci
win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-VespucciLibrary/debug/libvespucci.a

#MSVC for deployment (used for build using static qt and all libraries compiled in msvc linked statically)
count(deploy_win64, 1){
    win32:!win32-g++: LIBS += -L$$PWD/../build-VespucciLibrary/release/ -lvespucci
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-VespucciLibrary/release/vespucci.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/MLPACK/ -lmlpack
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/MLPACK/include
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/MLPACK/include
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/MLPACK/mlpack.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/Armadillo/lib/ -larmadillo
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/Armadillo/include
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/Armadillo/include
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/Armadillo/lib/armadillo.lib

    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/boost_1_60_0
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/boost_1_60_0

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_math_c99-vc140-mt-s-1_60
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_math_c99-vc140-mt-s-1_60.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_serialization-vc140-mt-s-1_60
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_serialization-vc140-mt-s-1_60.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_random-vc140-mt-s-1_60
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_random-vc140-mt-s-1_60.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_unit_test_framework-vc140-mt-s-1_60
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_unit_test_framework-vc140-mt-s-1_60.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/ -llibboost_program_options-vc140-mt-s-1_60
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/boost_1_60_0/lib64-msvc-14.0/libboost_program_options-vc140-mt-s-1_60.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/LAPACK/ -llapack_x64
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/LAPACK/lapack_x64.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/OpenBLAS/ -llibopenblas
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/OpenBLAS/libopenblas.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/ARPACK/ -larpack_x64
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/ARPACK/arpack_x64.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/cminpack/ -lcminpack
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/cminpack
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/cminpack
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/cminpack/cminpack.lib

    win32:!win32-g++: LIBS += -L$$PWD/../MSVC_deployment_deps/QCustomPlot/Release/ -lqcustomplot
    INCLUDEPATH += $$PWD/../MSVC_deployment_deps/QCustomPlot
    DEPENDPATH += $$PWD/../MSVC_deployment_deps/QCustomPlot
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../MSVC_deployment_deps/QCustomPlot/Release/qcustomplot.lib
}






