#!/bin/bash
#
# Script to run unit tests.
#

if [ ! -d "./build" ]; then
    echo "Please run build_all.sh first."
    exit 1
fi

cd build
make test
cd ..
