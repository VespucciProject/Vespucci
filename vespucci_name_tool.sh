#!/bin/sh
install_name_tool -change libvespucci.1.dylib @executable_path/../Frameworks/libvespucci.1.dylib MacOS/Vespucci
install_name_tool -change libmlpack.2.dylib @executable_path/../Frameworks/libmlpack.2.dylib MacOS/Vespucci
install_name_tool -change libarmadillo.7.dylib @executable_path/../Frameworks/libarmadillo.7.dylib MacOSVespucci
install_name_tool -change /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib @executable_path/../Frameworks/libarpack.2.dylib MacOS/Vespucci
install_name_tool -change /usr/local/opt/hdf5/lib/libhdf5_cpp.11.dylib @executable_path/../Frameworks/libhdf5_cpp.11.dylib MacOS/Vespucci
install_name_tool -change /usr/local/opt/hdf5/lib/libhdf5.10.dylib @executable_path/../Frameworks/libhdf5.10.dylib MacOS/Vespucci
install_name_tool -change /usr/lib/libz.1.dylib @executable_path/../Frameworks/libz.1.dylib MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore MacOS/Vespucci
install_name_tool -change /usr/local/opt/qt5/lib/QtSvg.framework/Versions/5/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/5/QtSvg MacOS/Vespucci

install_name_tool -change libvespucci.1.dylib @executable_path/../Frameworks/libvespucci.1.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change libmlpack.2.dylib @executable_path/../Frameworks/libmlpack.2.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change libarmadillo.7.dylib @executable_path/../Frameworks/libarmadillo.7.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib @executable_path/../Frameworks/libarpack.2.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/hdf5/lib/libhdf5_cpp.11.dylib @executable_path/../Frameworks/libhdf5_cpp.11.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/hdf5/lib/libhdf5.10.dylib @executable_path/../Frameworks/libhdf5_cpp.11.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/lib/libz.1.dylib @executable_path/../Frameworks/libz.1.dylib Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/qt5/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/qt5/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/qt5/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore Frameworks/libvespucci.1.0.0.dylib
install_name_tool -change /usr/local/opt/qt5/lib/QtSvg.framework/Versions/5/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/5/QtSvg Frameworks/libvespucci.1.0.0.dylib
