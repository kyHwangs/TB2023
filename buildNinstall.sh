#! /bin/bash

# source envset.sh
source envset_test.sh
mkdir build install
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install 
make -j4 install
cd ..
