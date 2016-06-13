Vespucci 
========
[!["travis_ci_status"](https://travis-ci.org/VespucciProject/Vespucci.svg?branch=master)](https://travis-ci.org/VespucciProject/Vespucci) [![Build status](https://ci.appveyor.com/api/projects/status/3v1voa5sim7whv55?svg=true)](https://ci.appveyor.com/project/VespucciProject/vespucci)

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
Compiling Vespucci from source is easy on Unix-like systems. Assuming you
have the proper pre-requisites (and Vespucci.pro is properly edited to reflect
their paths):

    qmake && make && make install

Should do the trick. This does not require elevation as the default install path
is two directories above the source. You can modify PREFIX to install to a different
directory if you'd like. If you are not compiling with static Qt, and the Qt linked
libraries are not in your path, you will have to deploy Qt (using windeployqt or macdeployqt)

The file deploymac.sh can be run to make the .app file run on Mac OS X, assuming that you used homebrew to install all dependencies.

Pre-requisites:
--------------------
If you can compile and install MLPACK and have the Qt framework installed,
then you can compile and install Vespucci. You should have the following
libraries on your system:

* Qt
* MLPACK
* Armadillo (compiled shared or static library)
* LAPACK and BLAS (or high speed replacement like OpenBLAS, Accelerate, MKL, etc.)
* ARPACK (or ARPACK-ng)
* Boost (program_options, unit_test_framework, random, and math (c99))
* LibXML2
* Vespucci-QCP (compiled shared or static library, a fork of QCustomPlot https://github.com/dpfoose/Vespucci-QCP)

All of the above packages, with the exception of MLPACK and QCustomPlot, are 
readily available from most major GNU/Linux repositories. Installation on Mac
OS is similarly easy, so long as all dependencies are installed with homebrew. You may need to edit armadillo’s configure configuration to disable the wrapper.

Because it is difficult to find Windows binaries for Armadillo, MLPACK and ARPACK,
and to make development on Windows easier, I have included Windows binaries of all
requisite libraries in the MinGW_libs repo. These are compiled with MinGW-w64 
version 3 (GCC 4.8.2) with SEH for exception handling. The MSVC repo includes the prerequisites compiled using Visual Studio 14.0 (2015).

The included Qt profile uses the library paths used by Ubuntu's package manager.
Mac OS dependences are installed with homebrew.

For Unix-like systems, shared libraries are the default. You will need to
compile and install static versions of these libraries if you want a stand-alone executable.

Windows dependencies can mostly be installed using NuGet.


 
