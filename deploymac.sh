#!/bin/sh
#This file is part of Vespucci.
#Vespucci is distributed under the terms of the GNU General Public License v3

#Vespucci Mac OS X deployment script
#Assumes that all libraries were installed using homebrew
#This should be run from the after building Vepsucci
#to $deploymentDir/Contents/lib and then use install_name_tool

#this is based on my local setup (which is represented in the .pro file)
#This script should be run from the build directory

#make sure a "Frameworks" directory exists
mkdir -p "$deploymentDir/Contents/lib"

if [ $# -eq 0 ]; then
    echo "Usage: $0 [source dir] [build dir] [deployment dir] [lib dir]";
    echo "[source dir] = Location of Vespucci source tree";
    echo "[build dir] = Location of the Vespucci build tree";
    echo "[app dir] = The Vespucci.app package ";
    echo "[lib dir] = Where the libraries are located, matching structure of Vespucci_dependencies_macOS.zip";
fi
srcDir=$1
buildDir=$2
deploymentDir=$3
libDir=$4

#copy over the Qt Frameworks
cp -LR /usr/local/opt/qt5/lib/QtPrintSupport.framework $deploymentDir/Contents/lib/
cp -LR /usr/local/opt/qt5/lib/QtWidgets.framework $deploymentDir/Contents/lib
cp -LR /usr/local/opt/qt5/lib/QtGui.framework $deploymentDir/Contents/lib
cp -LR /usr/local/opt/qt5/lib/QtCore.framework $deploymentDir/Contents/lib
cp -LR /usr/local/opt/qt5/lib/QtSvg.framework $deploymentDir/Contents/lib

#remove Qt header files and symlinks to them
yes |rm -r $deploymentDir/Contents/lib/QtPrintSupport.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/lib/QtPrintSupport.framework/Headers
yes |rm -r $deploymentDir/Contents/lib/QtWidgets.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/lib/QtWidgets.framework/Headers
yes |rm -r $deploymentDir/Contents/lib/QtGui.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/lib/QtGui.framework/Headers
yes |rm -r $deploymentDir/Contents/lib/QtCore.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/lib/QtCore.framework/Headers
yes |rm -r $deploymentDir/Contents/lib/QtSvg.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/lib/QtSvg.framework/Headers

#copy over third-party libraries and the required symlinks
find $buildDir/VespucciLibrary -name \*.dylib -exec cp {} $deploymentDir/Contents/lib \;
find $libDir/mlpack/lib -name \*.dylib -exec cp {} $deploymentDir/Contents/lib \;
find $libDir/armadillo/lib -name \*.dylib -exec cp {} $deploymentDir/Contents/lib \;
cp -L /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib $deploymentDir/Contents/lib
cp /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib $deploymentDir/Contents/lib
cp -L /usr/local/lib/libhdf5.10.dylib $deploymentDir/Contents/lib
cp /usr/local/lib/libhdf5.10.dylib $deploymentDir/Contents/lib
ln -s $deploymentDir/Contents/lib/libhdf5.10.dylib $deploymentDir/Contents/lib/libhdf5.dylib

#Configure the executable to use the libraries in Frameworks directory
install_name_tool -change libmlpack.2.dylib \
    @executable_path/../Frameworks/libmlpack.2.dylib \
    $deploymentDir/Contents/MacOS/Vespucci

install_name_tool -change libvespucci.1.dylib \
    @executable_path/../lib/libvespucci.1.dylib\
    $deploymentDir/Contents/MacOS/Vespucci

install_name_tool -change libarmadillo.6.dylib \
    @executable_path/../lib/libarmadillo.6.dylib \
    $deploymentDir/Contents/MacOS/Vespucci

install_name_tool -change /usr/local/opt/hdf5/lib/libhdf5.10.dylib \
    @executable_path/../lib/libhdf5.dylib

#set the application icon
iconSource=$srcDir/Vespucci/vespuccilogo.icns
iconDestination=$deploymentDir
icon=/tmp/vespuccilogo.icns
rsrc=/tmp/icon.rsrc
cp $iconSource $icon
sips -i $icon
DeRez -only icns $icon > $rsrc
SetFile -a C $iconDestination
if [ -f $iconDestination ]; then
    Rez -append $rsrc -o iconDestination
elif [-d $iconDestination ]; then
    touch $iconDestination/$'Icon\r'
    Rez -append $rsrc -o $iconDestination/Icon?
    setfile -a V $iconDestination/Icon?
fi
rm $rsrc $icon

#copy header files from the source directory
cp -r $srcDir/VespucciLibrary/include $deploymentDir/Contents/


