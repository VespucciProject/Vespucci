#!/bin/sh
#This file is part of Vespucci.
#Vespucci is distributed under the terms of the GNU General Public License v3

#Vespucci Mac OS X deployment script
#Assumes that all libraries were installed using homebrew
#This should be run from the .app directory after building Vepsucci
#You will have to manually copy QCustomPlot.frameworks and Vespucci.frameworks
#to Vespucci/Vespucci.app/Contents/Frameworks and then use install_name_tool

#this is based on my local setup (which is represented in the .pro file)
#This script should be run from the build directory

#make sure a "Frameworks" directory exists
