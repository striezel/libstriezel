#!/bin/bash

# License: GNU General Public License 3+

# This script contains the commands for the build process on drone.io.
# drone.io currently uses Ubuntu 12.04 LTS.

# Add repository for g++ 4.8, because we use C++11 code here.
sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo apt-get update
# install libraries and GNU C++ Compiler 4.8
sudo apt-get install -y freeglut3-dev libglfw-dev libxml2-dev \
     libarchive-dev libzip-dev zlib1g-dev g++-4.8 gcc-4.8 cmake
# export environment variables for C++ compiler
export CXX="g++-4.8"
export CC="gcc-4.8"
# create and change into build directory
mkdir ./build
cd ./build
# build binaries for test cases with CMake, but skip all of the long-running
# SHA tests that take several minutes altogether
cmake ../ -DLONG_HASH_TESTS=OFF
if [[ $? -ne 0 ]]
then
  echo "CMake failed!"
  exit 1
fi

make -j2
if [[ $? -ne 0 ]]
then
  echo "Could not build binary files with make!"
  exit 1
fi

ctest -V
if [[ $? -ne 0 ]]
then
  echo "Some test cases failed!"
  exit 1
fi
