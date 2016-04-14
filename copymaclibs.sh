#!/bin/sh
#This file is part of Vespucci.
#Vespucci is distributed under the terms of the GNU General Public License v3

#Vespucci Mac OS X deployment script
#Assumes that all libraries were installed using homebrew
#This should be run from the .app directory after building Vepsucci
#You will have to manually copy QCustomPlot.frameworks and Vespucci.frameworks
#to Vespucci/Vespucci.app/Contents/Frameworks and then use install_name_tool

#this is based on my local setup (which is represented in the .pro file)
#This script should be run from the build directory

#make sure a "Frameworks" directory exists
mkdir -p "Vespucci/Vespucci.app/Contents/Frameworks"

#copy over the Qt Frameworks
cp -LR /usr/local/opt/qt5/lib/QtPrintSupport.framework Vespucci/Vespucci.app/Contents/Frameworks/
cp -LR /usr/local/opt/qt5/lib/QtWidgets.framework Vespucci/Vespucci.app/Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtGui.framework Vespucci/Vespucci.app/Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtCore.framework Vespucci/Vespucci.app/Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtSvg.framework Vespucci/Vespucci.app/Contents/Frameworks

#remove header files and symlinks to them
yes |rm -r Vespucci/Vespucci.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/Headers
yes |rm Vespucci/Vespucci.app/Contents/Frameworks/QtPrintSupport.framework/Headers
yes |rm -r Vespucci/Vespucci.app/Contents/Frameworks/QtWidgets.framework/Versions/5/Headers
yes |rm Vespucci/Vespucci.app/Contents/Frameworks/QtWidgets.framework/Headers
yes |rm -r Vespucci/Vespucci.app/Contents/Frameworks/QtGui.framework/Versions/5/Headers
yes |rm Vespucci/Vespucci.app/Contents/Frameworks/QtGui.framework/Headers
yes |rm -r Vespucci/Vespucci.app/Contents/Frameworks/QtCore.framework/Versions/5/Headers
yes |rm Vespucci/Vespucci.app/Contents/Frameworks/QtCore.framework/Headers
yes |rm -r Vespucci/Vespucci.app/Contents/Frameworks/QtSvg.framework/Versions/5/Headers
yes |rm Vespucci/Vespucci.app/Contents/Frameworks/QtSvg.framework/Headers

#copy over third-party libraries and the required symlinks
find VespucciLibrary -name \*.dylib -exec cp {} Vespucci/Vespucci.app/Contents/Frameworks \;
find ../mlpack/lib -name \*.dylib -exec cp {} Vespucci/Vespucci.app/Contents/Frameworks \;
find ../armadillo/lib -name \*.dylib -exec cp {} Vespucci/Vespucci.app/Contents/Frameworks \;
cp -R ../Vespucci-QCP/release/QCustomPlot.framework Vespucci/Vespucci.app/Contents/Frameworks
cp -L /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib Vespucci/Vespucci.app/Contents/Frameworks
cp /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib Vespucci/Vespucci.app/Contents/Frameworks
cp -L /usr/local/lib/libhdf5.10.dylib Vespucci/Vespucci.app/Contents/Frameworks
cp /usr/local/lib/libhdf5.10.dylib Vespucci/Vespucci.app/Contents/Frameworks
