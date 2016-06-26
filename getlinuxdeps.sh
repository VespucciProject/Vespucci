#!/bin/sh
wget https://github.com/VespucciProject/Vespucci_dependencies/releases/download/1/Vespucci_dependencies_linux.tar.gz
tar xvf Vespucci_dependencies_linux.tar.gz  
sudo apt-add-repository -y ppa:ubuntu-sdk-team
sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-add-repository -y ppa:beineri/opt-qt561-trusty
sudo apt-get -y update
sudo apt-get -y install g++-4.9
sudo apt-get -y install qt56base qt56tools qt56imageformats qt56location qt56declarative libsqlite3-dev qt56svg
sudo apt-get -y install mesa-common-dev
sudo apt-get -y install libhdf5-dev libarpack2-dev
sudo apt-get -y install libxml2-dev libboost1.55-dev libboost-math1.55-dev libboost-program-options1.55-dev libboost-test1.55-dev libboost-random1.55-dev
