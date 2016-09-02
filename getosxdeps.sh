#!/bin/sh
#load Vespucci dependencies for osx using homebrew
brew tap homebrew/science
brew install hdf5 --with-unsupported
brew link hdf5

if [ ! -f /usr/local/lib/QtCore.framework ]; then
    brew install qt5
    brew link qt5 --force
fi
if [ ! -f /usr/local/lib/libarpack.dylib ]; then
    brew install arpack
    brew link arpack
fi
if [ ! -f /usr/local/lib/libxml2.dylib ]; then
    brew install libxml2
    brew link libxml2
fi
if [ ! -f /usr/local/bin/wget ]; then
    brew install wget
    brew link wget
fi
if [ ! -f /usr/local/include/superlu/supermatrix.h ]; then
    brew install superlu43
    brew link superlu43 --force
fi
wget https://github.com/VespucciProject/Vespucci_dependencies/releases/download/1/Vespucci_dependencies_macOS.zip
unzip Vespucci_dependencies_macOS.zip

