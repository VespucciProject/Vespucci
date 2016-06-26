#!/bin/sh
#This script is part of Vespucci (vespucciproject.org)
#Vespucci is distributed under the terms of the GNU General Public License version 3
if [ $# -eq 0 ]; then
    echo "Usage: $0 [source dir] [build dir] [deployment dir] [lib dir]";
    echo "[source dir] = Location of Vespucci source tree";
    echo "[build dir] = Location of the Vespucci build tree";
    echo "[deployment dir] = Where to deploy the Vespucci package (lib and bin dirs will be created)";
    echo "[lib dir] = Where the libraries are located, matching structure of Vespucci_dependencies_Linux.tar.gz";
fi
srcDir=$1
buildDir=$2
deploymentDir=$3
libDir=$4

if [ ! -f $deploymentDir ]; then mkdir $deploymentDir; fi
if [ ! -f $deploymentDir/bin ]; then mkdir $deploymentDir/bin; fi
if [ ! -f $deploymentDir/include ]; then mkdir $deploymentDir/include; fi
if [ ! -f $deploymentDir/lib ]; then mkdir $deploymentDir/lib; fi

find $buildDir/VespucciProject -name libvespucci.* -exec cp {} $deploymentDir/lib \;
find $libDir/mlpack/lib -name \*.so -exec cp {} $deploymentDir/lib \;
find $libDir/armadillo/lib -name \*.so -exec cp {} $deploymentDir/lib \;

