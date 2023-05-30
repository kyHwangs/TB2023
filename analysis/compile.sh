#! /bin/bash

# g++ -I$INSTALL_DIR_PATH/include -L$INSTALL_DIR_PATH/lib $INSTALL_DIR_PATH/lib/libdrcTB.so `root-config --cflags --libs` $1.cc -o $1
g++ \
-I$INSTALL_DIR_PATH/include \
-L$INSTALL_DIR_PATH/lib $INSTALL_DIR_PATH/lib/libdrcTB.dylib $YAMLPATH/libyaml-cpp.dylib `root-config --cflags --libs` $1.cc -o $1

