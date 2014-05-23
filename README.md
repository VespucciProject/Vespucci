Vespucci
========
Because the members of my group primarily use Windows machines, most releases will only include Windows binaries.  Major releases may include binaries for Windows, Mac OSX,  and GNU/Linux.  


Compiling Vespucci:
This program requires the armadillo template library. Armadillo must be configured to use LAPACK and BLAS or a replacement for them (such as OpenBLAS or the commercial equivalent from your CPU manufacturer).  This program incorporates a modified version of the QCustomPlot library, which is included in the repository.

The Qt Creator profile in this repository is designed for my personal use for compiling the program on a Windows machine.
Windows:
This should compile from Qt Creator on your platform.  The program requires armadillo and OpenBLAS.  You will have to modify the .pro file to direct the compiler to your copy of the libraries.  If you wish to compile on Windows, you probably know what you are doing 

GNU/Linux
Armadillo and OpenBLAS should be availible from your distribution.  There are no makefiles included in the master branch, but future releases may include them.  You can open the program from Qt Creator and compile it there if you wish.  
