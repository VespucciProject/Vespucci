#!/bin/sh
wget https://github.com/VespucciProject/Vespucci-QCP/releases/download/1.3.1/vespucci_travis_depts.tar.gz
tar xvf vespucci_travis_depts.tar.gz -C /home/travis
sudo apt-add-repository -y ppa:ubuntu-sdk-team
sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-add-repository -y ppa:beineri/opt-qt551-trusty
sudo apt-get -y update
sudo apt-get -y install qt55base qt55tools qt55imageformats qt55location qt55declarative libsqlite3-dev qt55svg
sudo apt-get -y install libhdf5-dev libarpack2-dev
sudo apt-get -y install libxml2-dev libboost1.55-dev libboost-math1.55-dev libboost-program-options1.55-dev libboost-test1.55-dev libboost-random1.55-dev libcminpack-dev
source /opt/qt55/bin/qt55-env.sh
