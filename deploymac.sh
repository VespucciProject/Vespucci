#!/bin/sh
#This file is part of Vespucci.
#Vespucci is distributed under the terms of the GNU General Public License v3

#Vespucci Mac OS X deployment script
#Assumes that all libraries were installed using homebrew
#This should be run from the .app directory after building Vepsucci
#You will have to manually copy QCustomPlot.frameworks and Vespucci.frameworks
#to Contents/Frameworks and then use install_name_tool

#make sure a "Frameworks" directory exists
if [ -d "Contents/Frameworks" ]; then
  mkdir "Contents/Frameworks"
fi

#copy over the Qt Frameworks
cp -LR /usr/local/opt/qt5/lib/QtPrintSupport.framework Contents/Frameworks/
cp -LR /usr/local/opt/qt5/lib/QtWidgets.framework Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtGui.framework Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtCore.framework Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtSvg.framework Contents/Frameworks

#remove header files and symlinks to them
rm -r Contents/Frameworks/QtPrintSupport.framework/Versions/5/Headers
rm Contents/Frameworks/QtPrintSupport.framework/Headers
rm -r Contents/Frameworks/QtWidgets.framework/Versions/5/Headers
rm Contents/Frameworks/QtWidgets.framework/Headers
rm -r Contents/Frameworks/QtGui.framework/Versions/5/Headers
rm Contents/Frameworks/QtGui.framework/Headers
rm -r Contents/Frameworks/QtCore.framework/Versions/5/Headers
rm Contents/Frameworks/QtCore.framework/Headers
rm -r Contents/Frameworks/QtSvg.framework/Versions/5/Headers
rm Contents/Frameworks/QtSvg.framework/Headers

#copy over third-party libraries and the required symlinks
cp -L /usr/local/lib/libmlpack.1.dylib Contents/Frameworks
cp /usr/local/lib/libmlpack.1.dylib Contents/Frameworks
cp -L /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib Contents/Frameworks
cp /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib Contents/Frameworks
cp -L /usr/local/lib/libhdf5.9.dylib Contents/Frameworks
cp /usr/local/lib/libhdf5.9.dylib Contents/Frameworks

#change the search locations for required libraries
install_name_tool -change /usr/local/lib/libmlpack.1.dylib @executable_path/../Frameworks/libmlpack.1.dylib Contents/MacOS/Vespucci
install_name_tool -change /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib @executable_path/../Frameworks/libarpack.2.dylib Contents/MacOS/Vespucci
install_name_tool -change /usr/local/lib/libhdf5.9.dylib @executable_path/../Frameworks/libhdf5.9.dylib Contents/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport Contents/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets Contents/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui Contents/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore Contents/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtSvg.framework/Versions/5/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/5/QtSvg Contents/MacOS/Vespucci
