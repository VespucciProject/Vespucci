#!/bin/sh

if [ -d "Content/Frameworks" ]; then
  mkdir "Content/Frameworks"
fi
cp -LR /usr/local/opt/qt5/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport Content/Frameworks/
cp -LR /usr/local/opt/qt5/lib/QtWidgets.framework/Versions/5/QtWidgets Content/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtGui.framework/Versions/5/QtGui Content/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore Content/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtSvg.framework/Versions/5/QtSvg Content/Frameworks
cp -L /usr/local/lib/libmlpack.1.dylib Content/Frameworks
cp /usr/local/lib/libmlpack.1.dylib Content/Frameworks
cp -L /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib Content/Frameworks
cp /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib Content/Frameworks
cp -L /usr/local/lib/libhdf5.9.dylib Content/Frameworks
cp /usr/local/lib/libhdf5.9.dylib Content/Frameworks


install_name_tool -change /usr/local/lib/libmlpack.1.dylib @executable_path/../Frameworks/libmlpack.1.dylib Content/MacOS/Vespucci
install_name_tool -change /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib @executable_path/../Frameworks/libarpack.2.dylib Content/MacOS/Vespucci
install_name_tool -change /usr/local/lib/libhdf5.9.dylib @executable_path/../Frameworks/libhdf5.9.dylib Content/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport Content/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets Content/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui Content/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore Content/MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtSvg.framework/Versions/5/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/5/QtSvg Content/MacOS/Vespucci
