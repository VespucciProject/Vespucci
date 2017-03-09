#!/bin/sh
wget https://github.com/VespucciProject/Vespucci_dependencies/releases/download/1/Vespucci_dependencies_linux.tar.gz
tar xvf Vespucci_dependencies_linux.tar.gz
sudo apt-add-repository -y ppa:ubuntu-sdk-team
sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test
sudo add-apt-repository -y ppa:beineri/opt-qt58-trusty
sudo apt-get -y update
sudo apt-get -y install g++-4.9 build-essential
sudo apt-get -y install qt58base qt58tools qt58imageformats 
sudo apt-get -y install qt58location qt58declarative libsqlite3-dev qt58svg
sudo apt-get -y install mesa-common-dev freeglut3.dev
sudo apt-get -y install libarpack2-dev
sudo apt-get -y install libxml2-dev
sudo apt-get -y install libboost1.55-all-dev
sudo apt-get -y install zlib1g zlib1g-dev
wget https://support.hdfgroup.org/ftp/HDF5/current18/bin/linux-centos6-x86_64-gcc447/hdf5-1.8.18-linux-centos6-x86_64-gcc447-shared.tar.gz
tar xvf hdf5-1.8.18-linux-centos6-x86_64-gcc447-shared.tar.gz
cd hdf5-1.8.18-linux-centos6-x86_64-gcc447-shared
sudo cp include/* /usr/include
sudo cp lib/* /usr/lib
