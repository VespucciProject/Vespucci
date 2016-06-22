#!/bin/sh
if [ $# -eq 0 ]; then
    echo "Usage: $0 [deployment dir] [src dir] [tarball]";
fi
deploymentDir=$1
srcDir=$2
outTarball=$3
cp $srcDir/deploymentREADME.txt $deploymentDir
cp $srcDir/LICENSE.txt $deploymentDir
mv $deploymentDir/deploymentREADME.txt $deploymentDir/README
mv $deploymentDir/LICENSE.txt $deploymentDir/LICENSE
tar -cvzf $outTarball $deploymentDir
