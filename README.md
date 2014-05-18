Vespucci
========
A not-yet-very-good chemical imaging application written in C++ using Qt and armadillo.  Vespucci creates false-color images from hyperspectral datasets using band intensities, band intensity ratios, and principal component analysis, with the intent to support partial least squares imaging and hierarchical clustering in development.  Vespucci is designed primarily for Raman and IR spectromicroscopy, but can (in theory, eventually) process any dataset consisting of spatial and spectral information.  Vespucci is designed to be as easy to use as possible, with a lower memory footprint than packages written for systems like MATLAB or R.  This focus on ease of use somewhat reduces the program's functionality, as the user does not have access to anything "under the hood".

Currently, Vespucci can only handle ASCII files in a "wide" format (as the Raman spectrometer currently used by my group, manufactured by Horiba, outputs ASCII files in this format):

		wavelength
x	y	spectra

The ability to import other data formats will be implemented in the near future.

Daniel P. Foose
Department of Chemistry
Wright State University
Dayton, OH, United States of America
