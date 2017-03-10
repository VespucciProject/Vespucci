Vespucci 
========
[![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/yvo16f9ojkkkxi56?svg=true)](https://ci.appveyor.com/project/dpfoose/vespucci) [!["Travis-CI Build Status"](https://travis-ci.org/VespucciProject/Vespucci.svg?branch=master)](https://travis-ci.org/VespucciProject/Vespucci) 

Vespucci is a free/libre/open-source, cross-platform tool for spectroscopic
imaging. Vespucci is regularly built on Windows, Mac and Linux operating systems.

Vespucci is distributed under the terms of the GNU General Public License version 3.
A copy of this license is provided in LICENSE

A research article detailing Vespucci has been published in the 
[*Journal of Open Research Software*](http://openresearchsoftware.metajnl.com/articles/10.5334/jors.91/ "DOI:10.5334")

This branch is the active branch of the project and, while more stable than the branches of individual contributors, is **probably not stable**.

After the release of the first 1.0.0 beta, there will be tags established for releases.

Bug reports should be directed to the Issues tab.

Binary Releases:
-----------------
Releases for all three supported platforms are availible in the Releases tab.
The latest binaries are available at 
[Bintray](https://bintray.com/vespucciproject/Vespucci_automated_builds/Vespucci_latest)
The release package includes the executable, the Vespucci library and runtime pre-requisites
and the header files needed to use the library.
Versioning:
-----------
Versions up to 1.0.0 are not considered stable and changes between 0.n.0 and 0.n+1.0 may be major.
After 1.0.0, 1.n+1.0 will contain new features to 1.n.0, but will have an API that is mostly compatible. n+1.0.0 contains major changes in the API that may break programs built from it. 1.n.x+1 contains a bug fix or minor improvement to 1.n.x

Compiling Vespucci:
-------------------
Compiling Vespucci from source is not the easiest but still possible. Look at 
.travis.yml for linux and mac builds and appveyor.yml for windows builds.
Vespucci can be built using two different build systems, [CMake](https://cmake.org)
 and [QMake](http://doc.qt.io/qt-5/qmake-manual.html). You will most likely have to
 edit the .pro file to get qmake to work. CMake should work relatively easily.
Vespucci has the following prerequisites:
* Qt 5.5+
* Armadillo (with BLAS/LAPACK and HDF5)
* mlpack
* yaml-cpp
* EmfEngineGPL

Look at the scripts used by .travis.yml and appveyor.yml to figure out how to
build on your platform.

Contributing:
-------------
If you would like to contribute to Vespucci, please read CONTRIBUTING.md
If you need help setting up your development environment to build Vespucci from 
source, feel free to contact us.
