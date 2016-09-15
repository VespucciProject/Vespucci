#!/bin/sh
wget https://github.com/VespucciProject/Vespucci_dependencies/releases/download/1/Vespucci_dependencies_linux.tar.gz
tar xvf Vespucci_dependencies_linux.tar.gz
sudo apt-add-repository -y ppa:ubuntu-sdk-team
sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-add-repository -y ppa:beineri/opt-qt561-trusty
sudo apt-get -y update
sudo apt-get -y install g++-4.9 build-essential
sudo apt-get -y install qt56base qt56tools qt56imageformats 
sudo apt-get -y install qt56location qt56declarative libsqlite3-dev qt56svg
sudo apt-get -y install mesa-common-dev freeglut3.dev
sudo apt-get -y install libarpack2-dev
sudo apt-get -y install libxml2-dev
sudo apt-get -y install libboost1.55-all-dev
sudo apt-get -y install zlib1g zlib1g-dev
