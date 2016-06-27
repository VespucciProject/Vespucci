Vespucci 
========
[![Build status](https://ci.appveyor.com/api/projects/status/yvo16f9ojkkkxi56?svg=true)](https://ci.appveyor.com/project/dpfoose/vespucci) [!["travis_ci_status"](https://travis-ci.org/VespucciProject/Vespucci.svg?branch=master)](https://travis-ci.org/VespucciProject/Vespucci)  

This branch is the active branch of the project and, while more stable than the branches of individual contributors, is **probably not stable**.

Vespucci is a free/libre/open-source, cross-platform tool for spectroscopic
imaging
Vespucci is distributed under the terms of the GNU General Public License version 3.
A copy of this license is provided in LICENSE

Binary Releases:
-----------------
Because the members of my group primarily use Windows machines, most releases 
will only include Windows binaries.  Major releases include binaries for 
Windows, Mac OS X, and GNU/Linux.

Nightly builds will be availible for Windows soon.

Versioning:
-----------
Versions up to 1.0.0 are not considered stable and changes between 0.n.0 and 0.n+1.0 may be major.
After 1.0.0, 1.n+1.0 will contain new features to 1.n.0, but will have an API that is mostly compatible. n+1.0.0 contains major changes in the API that may break programs built from it. 1.n.x+1 contains a bug fix or minor improvement to 1.n.x

Compiling Vespucci:
===================
Compiling Vespucci from source is not the easiest but still possible. Look at 
.travis.yml for linux and mac builds and appveyor.yml for windows builds.

Pre-requisites:
--------------------
Vespucci has the following prerequisites:
* Qt 5.5+
* Armadillo (with BLAS/LAPACK and HDF5)
* mlpack
* yaml-cpp
* quazip

Look at the scripts used by .travis.yml and appveyor.yml to figure out how to
build on your platform.

Binary Distributions:
======================
View the releases tab to download the latest binaries for your platform. Included
in the distribution is the Vespucci binary, the Vespucci C++ library, and the
Vespucci library headers.


 
