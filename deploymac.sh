#!/bin/sh
#This file is part of Vespucci.
#Vespucci is distributed under the terms of the GNU General Public License v3

#Vespucci Mac OS X deployment script
#Assumes that all libraries were installed using homebrew
#This should be run from the after building Vepsucci
#to $deploymentDir/Contents/Frameworks and then use install_name_tool

#this is based on my local setup (which is represented in the .pro file)
#This script should be run from the build directory

#make sure a "Frameworks" directory exists
mkdir -p "$deploymentDir/Contents/Frameworks"

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
cp -LR /usr/local/opt/qt5/lib/QtPrintSupport.framework $deploymentDir/Contents/Frameworks/
cp -LR /usr/local/opt/qt5/lib/QtWidgets.framework $deploymentDir/Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtGui.framework $deploymentDir/Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtCore.framework $deploymentDir/Contents/Frameworks
cp -LR /usr/local/opt/qt5/lib/QtSvg.framework $deploymentDir/Contents/Frameworks

#remove header files and symlinks to them
yes |rm -r $deploymentDir/Contents/Frameworks/QtPrintSupport.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/Frameworks/QtPrintSupport.framework/Headers
yes |rm -r $deploymentDir/Contents/Frameworks/QtWidgets.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/Frameworks/QtWidgets.framework/Headers
yes |rm -r $deploymentDir/Contents/Frameworks/QtGui.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/Frameworks/QtGui.framework/Headers
yes |rm -r $deploymentDir/Contents/Frameworks/QtCore.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/Frameworks/QtCore.framework/Headers
yes |rm -r $deploymentDir/Contents/Frameworks/QtSvg.framework/Versions/5/Headers
yes |rm $deploymentDir/Contents/Frameworks/QtSvg.framework/Headers

#copy over third-party libraries and the required symlinks
find $buildDir/VespucciLibrary -name \*.dylib -exec cp {} $deploymentDir/Contents/Frameworks \;
find $libDir/mlpack/lib -name \*.dylib -exec cp {} $deploymentDir/Contents/Frameworks \;
find $libDir/armadillo/lib -name \*.dylib -exec cp {} $deploymentDir/Contents/Frameworks \;
cp -L /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib $deploymentDir/Contents/Frameworks
cp /usr/local/opt/arpack/libexec/lib/libarpack.2.dylib $deploymentDir/Contents/Frameworks
cp -L /usr/local/lib/libhdf5.10.dylib $deploymentDir/Contents/Frameworks
cp /usr/local/lib/libhdf5.10.dylib $deploymentDir/Contents/Frameworks

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

