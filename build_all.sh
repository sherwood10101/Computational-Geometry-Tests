#!/bin/bash
#
# Script to build an application
#

if [ ! -d "./build" ]; then
    mkdir build
fi

cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install && make && make install
cd ..
