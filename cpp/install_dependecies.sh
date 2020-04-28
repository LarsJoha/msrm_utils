#!/bin/sh -e

sudo apt-get install -y libeigen3-dev

# cmake (3.16.3)
wget http://www.cmake.org/files/v3.16/cmake-3.16.3.tar.gz
tar -xvzf cmake-3.16.3.tar.gz
cd cmake-3.16.3/
./configure
make -j$(nproc --all)
sudo make install

cd ..
sudo rm -r cmake-3.16.3

# http lib
git clone https://github.com/yhirose/cpp-httplib.git
cd cpp-httplib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r cpp-httplib

# nlohman json

git clone https://github.com/nlohmann/json.git
cd json
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r json

# json rpc
git clone https://github.com/jsonrpcx/json-rpc-cxx.git
cd json-rpc-cxx
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r json-rpc-cxx
