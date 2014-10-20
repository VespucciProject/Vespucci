Vespucci Master Branch
========
Commits to this branch are generally only made at each official release.

Vespucci is a free/libre/open-source, cross-platform tool for spectroscopic
imaging

Vespucci is distributed under the terms of the GNU General Public License version 3.
A copy of this license is provided in LICENSE

Binary Releases:
-----------------
Because the members of my group primarily use Windows machines, most releases 
will only include Windows binaries.  Major releases may include binaries for 
Windows, Mac OSX, and GNU/Linux (most likely as .deb and .rpm).

A relatively stable Windows development build is availible from Dropbox:
https://www.dropbox.com/sh/at9zo6udw1oeaao/AADWCdwKD87Z7Fq9Tm6_Wk1fa

This is the binary most often used by my research group.


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
OS is similarly easy, though some packages may need to be compiled and installed
manually, or installed through MacPorts (https://www.macports.org/) or Fink 
(http://www.finkproject.org/). I haven't included a configure script, but setting
directories should be easy enough.

Because it is difficult to find Windows binaries for Armadillo, MLPACK and ARPACK,
and to make development on Windows easier, I have included Windows binaries of all
requisite libraries in the MinGW_libs branch. These are compiled with MinGW-w64 
version 3 (GCC 4.8.2) with SEH for exception handling. A 7z archive containing the
Qt distribution and MinGW-w64 toolkit I use is also included in the toolkit branch.

The included Qt profile uses the library paths used by Ubuntu's package manager.
Frameworks are used for Mac OS when possible.

For Unix-like systems, shared libraries are the default. You will need to
compile and install static versions of these libraries if you want a stand-
alone executable. Linux packages, if made availible will require Qt, OpenBLAS, and
ARPACK (armadillo and MLPACK will be included).

