#!/bin/bash
sudo add-apt-repository --yes ppa:gstreamer-developers/ppa
sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install -qq mm-common
sudo apt-get install -qq cmake
sudo apt-get install -qq qt5-qmake qtbase5-dev qtdeclarative5-dev libqt5webkit5-dev libsqlite3-dev
sudo apt-get install -qq libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt-get install -qq git
sudo apt-get install -qq libffi-dev
GST_CREATOR_DIRECTORY=$(pwd)
sudo apt-get install -qq libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/
cd $GST_CREATOR_DIRECTORY
git clone git://git.gnome.org/glib
cd glib
git checkout 2.38
./autogen.sh
make
sudo make install
cd ..
sudo apt-get install libxml-parser-perl
git clone git://git.gnome.org/glibmm
cd glibmm
git checkout 2.38.0
./autogen.sh
make
sudo make install
cd ..
git clone git://git.gnome.org/gstreamermm
cd gstreamermm
./autogen.sh
make
sudo make install
cd ..
mkdir build
cd build
cmake ..
make
