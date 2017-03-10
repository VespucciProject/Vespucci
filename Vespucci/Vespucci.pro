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
#This profile is based on what the Vespucci Project uses for continous integration
#Travis-CI for macOS and Linux, Appveyor for Windows.
#On Windows, all prerequisites except Qt and Boost are built and provided on github
#in the VespucciProject/Vespucci_dependencies project
#On macOS and Linux, only armadillo and mlpack are built and provided in the same place.
#On macOS, all other prerequisites are installed and linked using homebrew
#On Linux, all other prerequisites are installed using apt (Ubuntu 14.04 LTS)

QT       += core gui widgets printsupport svg network
CONFIG   += static debug_and_release

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
unix:!macx: QMAKE_CXXFLAGS += -std=c++0x -DARMA_DONT_USE_WRAPPER
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
    GUI/Display/citationdialog.cpp \
    GUI/Processing/cropdialog.cpp \
    GUI/Display/dataviewer.cpp \
    GUI/Processing/filterdialog.cpp \
    GUI/Analysis/kmeansdialog.cpp \
    GUI/QAbstractItemModel/maplistmodel.cpp \
    GUI/Display/mapviewer.cpp \
    GUI/Processing/metadatasetdialog.cpp \
    GUI/Analysis/plotmakerdialog.cpp \
    GUI/Analysis/plsdialog.cpp \
    GUI/Analysis/principalcomponentsdialog.cpp \
    GUI/Processing/rangedialog.cpp \
    GUI/Display/scalebardialog.cpp \
    GUI/QAbstractItemModel/spectratablemodel.cpp \
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
    GUI/Analysis/classicalleastsquaresdialog.cpp \
    GUI/settingsdialog.cpp \
    GUI/Processing/abscissainterpolationdialog.cpp \
    GUI/Processing/stitchimportdialog.cpp \
    Global/global.cpp \
    External/fileinterprocess.cpp \
    GUI/QAbstractItemModel/datasettreemodel.cpp \
    GUI/QAbstractItemModel/treeitem.cpp \
    Data/Dataset/macroparser.cpp \
    GUI/Analysis/transformdialog.cpp \
    GUI/Display/mapdialog.cpp \
    GUI/Display/datawidget.cpp \
    GUI/Display/plotwidget.cpp \
    GUI/macrodialog.cpp \
    Global/datamodel.cpp \
    GUI/QAbstractItemModel/datasetlistmodel.cpp \
    GUI/pythonshelldialog.cpp \
    qcustomplot.cpp \
    Data/Analysis/multianalyzer.cpp \
    GUI/Analysis/multianalysisdialog.cpp \
    GUI/Analysis/hypothesistestdialog.cpp \
    GUI/Display/mapplot.cpp \
    GUI/Display/globalgradientdialog.cpp \
    GUI/Display/colorrangedialog.cpp \
    GUI/Display/spectrumeditor.cpp \
    GUI/Processing/datasetimportdialog.cpp \
    GUI/Processing/matrixselectiondialog.cpp \
    GUI/Analysis/representativespectrumdialog.cpp \
    GUI/Analysis/ahcadialog.cpp \
    GUI/Display/historydialog.cpp \
    GUI/Processing/univariateconcatenationdialog.cpp \
    GUI/Analysis/metaanalysisdialog.cpp \
    GUI/Processing/datasetextractordialog.cpp \
    GUI/Analysis/dimensionalityestimationdialog.cpp \
    Data/Analysis/abstractdataanalyzer.cpp \
    Data/Analysis/matrixanalyzer.cpp \
    Data/Analysis/metaanalyzer.cpp \
    GUI/Processing/binarizedialog.cpp \
    Global/tcprequesthandler.cpp \
    GUI/Processing/textimportdialog.cpp \
    GUI/QAbstractItemModel/filetablemodel.cpp \
    GUI/Display/mapresizedialog.cpp
    GUI/Display/mapresizedialog.cpp

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
    GUI/Processing/cropdialog.h \
    GUI/Display/aboutdialog.h \
    GUI/Display/citationdialog.h \
    GUI/Display/dataviewer.h \
    GUI/Processing/filterdialog.h \
    GUI/Analysis/kmeansdialog.h \
    GUI/QAbstractItemModel/maplistmodel.h \
    GUI/Display/mapviewer.h \
    GUI/Processing/metadatasetdialog.h \
    GUI/Analysis/plsdialog.h \
    GUI/Analysis/principalcomponentsdialog.h \
    GUI/Processing/rangedialog.h \
    GUI/Display/scalebardialog.h \
    GUI/QAbstractItemModel/spectratablemodel.h \
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
    GUI/Analysis/classicalleastsquaresdialog.h \
    GUI/settingsdialog.h \
    GUI/Processing/abscissainterpolationdialog.h \
    GUI/Processing/stitchimportdialog.h \
    Global/global.h \
    External/fileinterprocess.h \
    GUI/QAbstractItemModel/datasettreemodel.h \
    GUI/QAbstractItemModel/treeitem.h \
    Data/Dataset/macroparser.h \
    GUI/Analysis/transformdialog.h \
    GUI/Display/mapdialog.h \
    GUI/Display/datawidget.h \
    GUI/Display/plotwidget.h \
    GUI/macrodialog.h \
    Global/datamodel.h \
    GUI/QAbstractItemModel/datasetlistmodel.h \
    GUI/pythonshelldialog.h \
    qcustomplot.h \
    Data/Analysis/multianalyzer.h \
    GUI/Analysis/multianalysisdialog.h \
    GUI/Analysis/hypothesistestdialog.h \
    GUI/Display/mapplot.h \
    GUI/Display/globalgradientdialog.h \
    GUI/Display/colorrangedialog.h \
    GUI/Display/spectrumeditor.h \
    GUI/Processing/datasetimportdialog.h \
    GUI/Processing/matrixselectiondialog.h \
    GUI/Analysis/representativespectrumdialog.h \
    GUI/Analysis/ahcadialog.h \
    GUI/Display/historydialog.h \
    GUI/Processing/univariateconcatenationdialog.h \
    GUI/Analysis/metaanalysisdialog.h \
    GUI/Processing/datasetextractordialog.h \
    Data/Analysis/abstractdataanalyzer.h \
    GUI/Analysis/dimensionalityestimationdialog.h \
    Data/Analysis/matrixanalyzer.h \
    Data/Analysis/metaanalyzer.h \
    GUI/Processing/binarizedialog.h \
    Global/tcprequesthandler.h \
    GUI/Processing/textimportdialog.h \
    GUI/QAbstractItemModel/filetablemodel.h \
    GUI/Display/mapresizedialog.h
    GUI/Display/mapresizedialog.h


FORMS    += \
    GUI/mainwindow.ui \
    GUI/Display/aboutdialog.ui \
    GUI/Analysis/bandratiodialog.ui \
    GUI/Processing/baselinedialog.ui \
    GUI/Display/citationdialog.ui \
    GUI/Processing/cropdialog.ui \
    GUI/Display/dataviewer.ui \
    GUI/Processing/filterdialog.ui \
    GUI/Analysis/kmeansdialog.ui \
    GUI/Display/mapviewer.ui \
    GUI/Processing/metadatasetdialog.ui \
    GUI/Analysis/plotmakerdialog.ui \
    GUI/Analysis/plsdialog.ui \
    GUI/Analysis/principalcomponentsdialog.ui \
    GUI/Processing/rangedialog.ui \
    GUI/Display/scalebardialog.ui \
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
    GUI/pythonshelldialog.ui \
    GUI/Analysis/multianalysisdialog.ui \
    GUI/Analysis/hypothesistestdialog.ui \
    GUI/Display/globalgradientdialog.ui \
    GUI/Display/colorrangedialog.ui \
    GUI/Display/spectrumeditor.ui \
    GUI/Processing/datasetimportdialog.ui \
    GUI/Processing/matrixselectiondialog.ui \
    GUI/Analysis/representativespectrumdialog.ui \
    GUI/Analysis/ahcadialog.ui \
    GUI/Display/historydialog.ui \
    GUI/Processing/univariateconcatenationdialog.ui \
    GUI/Analysis/metaanalysisdialog.ui \
    GUI/Processing/datasetextractordialog.ui \
    GUI/Analysis/dimensionalityestimationdialog.ui \
    GUI/Processing/binarizedialog.ui \
    GUI/Processing/textimportdialog.ui \
    GUI/Display/mapresizedialog.ui
    GUI/Display/mapresizedialog.ui

RESOURCES += \
    resources.qrc

#for inclusion of LibVespucci headers
INCLUDEPATH += $$PWD/../VespucciLibrary/include
DEPENDPATH += $$PWD/../VespucciLibrary/include

unix:!macx{
    CONFIG += c++11
    QMAKE_CXXFLAGS += -fopenmp
    linux-g++: QMAKE_CXXFLAGS += -fext-numeric-literals
    QMAKE_LFLAGS += -fopenmp
    QMAKE_RPATHDIR += $$(QTDIR)/lib
    LIBS += -L$$PWD/../../Vespucci_dependencies/mlpack/lib -lmlpack
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/mlpack/lib/libmlpack.a
    LIBS += -L$$PWD/../../Vespucci_dependencies/armadillo/lib -larmadillo
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/armadillo/lib/libarmadillo.a

    LIBS += -L/usr/lib/ -lhdf5
    LIBS += -L/usr/lib/ -lhdf5_cpp
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
    PRE_TARGETDEPS += $$OUT_PWD/../VespucciLibrary/libvespucci.a
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
    CONFIG += app_bundle
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
    QMAKE_CXXFLAGS += --system-header-prefix=/usr \
                      --system-header-prefix=$$PWD/../../armadillo \
                      --system-header-prefix=$$PWD/../../mlpack \
                      --system-header-prefix=$$PWD/../../yaml-cpp \
                      --system-header-prefix=$$PWD/../../quazip


    ICON = $$PWD/vespuccilogo.icns
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

    LIBS += -L$$PWD/../../hdf5/lib -lhdf5
    PRE_TARGETDEPS += $$PWD/../../hdf5/lib/libhdf5.a

    LIBS += -L$$PWD/../../hdf5/lib/ -lhdf5_cpp
    PRE_TARGETDEPS += $$PWD/../../hdf5/lib/libhdf5_cpp.a
    
    INCLUDEPATH += $$PWD/../../hdf5/include
    DEPENDPATH += $$PWD/../../hdf5/include

    LIBS += -L/usr/local/lib -lhdf5

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

    LIBS += -L$$PWD/../../EmfEngine/lib/ -lEmfEngine
    INCLUDEPATH += $$PWD/../../EmfEngine/include
    DEPENDPATH += $$PWD/../../EmfEngine/include
    PRE_TARGETDEPS += $$PWD/../../EmfEngine/lib/libEmfEngine.a

    LIBS += -L$$PWD/../../libemf/lib/ -lEMF
    INCLUDEPATH += $$PWD/../../libemf/include
    DEPENDPATH += $$PWD/../../libemf/include
    INCLUDEPATH += $$PWD/../../libemf/include/libemf
    DEPENDPATH += $$PWD/../../libemf/include/libemf
    PRE_TARGETDEPS += $$PWD/../../libemf/lib/libEMF.a
}

#windows libraries for msvc (we don't currently build the libraries for g++ on windows)
win32:!win32-g++{
    CONFIG += release force_debug_info
    QMAKE_CXXFLAGS += /MP /openmp
    QMAKE_LFLAGS += /LIBPATH:C:\Libraries\boost_1_60_0\lib64-msvc-14.0

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -llibhdf5_cpp
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    DEPENDPATH += $$PWD/../../Vespucci_dependencies/HDF5/include
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/libhdf5_cpp.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -llibhdf5
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/libhdf5.lib

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

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -llibzlib
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/libzlib.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/HDF5/lib/ -llibszip
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/HDF5/lib/libszip.lib

    LIBS += -L$$PWD/../../Vespucci_dependencies/EmfEngine/lib -lEmfEngine
    PRE_TARGETDEPS += $$PWD/../../Vespucci_dependencies/EmfEngine/lib/EmfEngine.lib
    INCLUDEPATH += $$PWD/../../Vespucci_dependencies/EmfEngine/include
    DEPENDPATH += $$PWD/../../Vespuci_dependencies/EmfEngine/include

    LIBS += -lgdiplus -lgdi32 -luser32

    RC_ICONS=$$PWD/vespuccilogo.ico
}








