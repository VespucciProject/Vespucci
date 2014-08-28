Vespucci
========
Vespucci is a free/libre/open-source, cross-platform tool for spectroscopic
imaging


Binary Releases:
Because the members of my group primarily use Windows machines, most releases 
will only include Windows binaries.  Major releases may include binaries for 
Windows, Mac OSX, and GNU/Linux.  


Compiling Vespucci:
===================
Compiling Vespucci from source is easy on Unix-like systems. Assuming you
have the proper pre-requisites (and Vespucci.pro is properly edited to reflect
their paths):

qmake && make && make install

Should do the trick. This does not require elevation as the default install path
is $HOME/Vespucci. You can modify INSTALL to install for all users if you'd
like.

Pre-requisites:
If you can compile and install MLPACK and have the Qt framework installed,
then you can compile and install Vespucci. You should have the following
libraries on your system:

Qt
MLPACK
ARMADILLO (compiled shared library)
LAPACK and BLAS (or high speed replacement like OpenBLAS, Accelerate, MKL, etc.)
ARPACK (or ARPACK-ng)
Boost (program_options, unit_test_framework, random, and math (c99))
LibXML2
QCustomPlot (compiled shared library)

All of the above packages, with the exception of MLPACK and QCustomPlot, are 
readily available from most major GNU/Linux repositories. Installation on Mac
OS is similarly easy, though some packages may need to be compiled and installed
manually.

The included Qt profile uses the library paths used by Ubuntu's package manager.
Frameworks are used for Mac OS when possible.

For Unix-like systems, shared libraries are the default. You will need to
compile and install static versions of these libraries if you want a stand-
alone executable.

