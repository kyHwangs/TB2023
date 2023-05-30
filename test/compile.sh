#! /bin/bash

g++ -I$INSTALL_DIR_PATH/include -L$INSTALL_DIR_PATH/lib64 $INSTALL_DIR_PATH/lib64/libdrcTB.so `root-config --cflags --libs` $1.cc -o $1
