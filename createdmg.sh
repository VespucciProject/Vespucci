#!/bin/sh
if [ $# -eq 0 ]; then
    echo "Usage: $0 [in app] [out img] [src dir]";
    echo "[in app] path to Vespucci.app";
    echo "[out img] path to DMG file to create";
    echo "[src dir] location of README, etc.";
fi
appDir=$1
outImg=$2
srcDir=$3
#prepare a folder to create a disk image from.
dmgDir=/tmp/dmgdir
mkdir $dmgDir
cp $srcDir/LICENSE.txt $dmgDir
cp $srcDir/deploymentREADME.txt $dmgDir
mv $dmgDir/deploymentREADME.txt $dmgDir/README.txt
cp -r $appDir $dmgDir

#create the disk image
hdiutil create -volname Vespucci -srcfolder $dmgDir -ov -format UDZO $outImg
