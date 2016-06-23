#/bin/sh
#load Vespucci dependencies for osx using homebrew
brew tap homebrew/science
brew install qt5
brew install hdf5
brew install libxml2
brew install arpack
brew install superlu43
brew install wget
brew link qt5
brew link hdf5
brew link libxml2
brew link arpack
brew link superlu43
brew link wget
wget https://github.com/VespucciProject/Vespucci_dependencies/releases/download/1/Vespucci_dependencies_macOS.zip
unzip Vespucci_dependencies_macOS.zip

