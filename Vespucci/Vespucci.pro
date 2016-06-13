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
    Data/Dataset/vespuccidataset.cpp \
    Data/Analysis/analysisresults.cpp \
    Data/Imaging/mapdata.cpp \
    Data/Analysis/mlpackpcadata.cpp \
    Data/Analysis/plsdata.cpp \
    Data/Analysis/principalcomponentsdata.cpp \
    Data/Analysis/vcadata.cpp \
    Data/Analysis/univariatedata.cpp \
    Data/Dataset/metadataset.cpp \
    GUI/Display/aboutdialog.cpp \
    GUI/Analysis/bandratiodialog.cpp \
    GUI/Processing/baselinedialog.cpp \
    GUI/Processing/booleanizedialog.cpp \
    GUI/Display/citationdialog.cpp \
    GUI/Processing/cropdialog.cpp \
    GUI/Processing/dataextractordialog.cpp \
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
    External/fileinterprocess.cpp \
    GUI/QAbstractItemModel/datasettreemodel.cpp \
    GUI/QAbstractItemModel/treeitem.cpp \
    Data/Import/datasetloader.cpp \
    Data/Dataset/macroparser.cpp \
    Data/Import/datasetsaver.cpp \
    GUI/Analysis/transformdialog.cpp \
    GUI/Display/mapdialog.cpp \
    GUI/Display/datawidget.cpp \
    GUI/Display/plotwidget.cpp \
    GUI/macrodialog.cpp \
    Global/datamodel.cpp \
    GUI/QAbstractItemModel/datasetlistmodel.cpp \
    GUI/pythonshelldialog.cpp

HEADERS  += \
    GUI/mainwindow.h \
    Global/vespucciworkspace.h \
    Data/Dataset/vespuccidataset.h \
    Data/Analysis/analysisresults.h \
    Data/Imaging/mapdata.h \
    Data/Analysis/mlpackpcadata.h \
    Data/Analysis/plsdata.h \
    Data/Analysis/principalcomponentsdata.h \
    Data/Analysis/univariatedata.h \
    Data/Analysis/vcadata.h \
    Data/Dataset/metadataset.h \
    GUI/Analysis/bandratiodialog.h \
    GUI/Processing/baselinedialog.h \
    GUI/Processing/booleanizedialog.h \
    GUI/Processing/cropdialog.h \
    GUI/Processing/dataextractordialog.h \
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
    External/fileinterprocess.h \
    GUI/QAbstractItemModel/datasettreemodel.h \
    GUI/QAbstractItemModel/treeitem.h \
    Data/Import/datasetloader.h \
    Data/Dataset/macroparser.h \
    Data/Import/datasetsaver.h \
    GUI/Analysis/transformdialog.h \
    GUI/Display/mapdialog.h \
    GUI/Display/datawidget.h \
    GUI/Display/plotwidget.h \
    GUI/macrodialog.h \
    Global/datamodel.h \
    GUI/QAbstractItemModel/datasetlistmodel.h \
    GUI/pythonshelldialog.h


FORMS    += \
    GUI/mainwindow.ui \
    GUI/Display/aboutdialog.ui \
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
    GUI/Analysis/univariatedialog.ui \
    GUI/Analysis/transformdialog.ui \
    GUI/Display/mapdialog.ui \
    GUI/Display/datawidget.ui \
    GUI/Display/plotwidget.ui \
    GUI/macrodialog.ui \
    GUI/pythonshelldialog.ui

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


