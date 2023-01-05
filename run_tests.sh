#!/bin/bash
#
# Script to run unit tests.
#

if [ ! -d "./build" ]; then
    echo "Please run build_all.sh first."
    exit 1
fi

cd build
if [ $# -eq 1 ]; then
    ./test/tool_path_test --log_level=test_suite --run_test=$1
else
    ./test/tool_path_test --log_level=test_suite
fi
cd ..
