#!/bin/bash

## This is to run the first program main.cpp

if [ -e "./cmake-build-debug/dumbot" ]; then
    ./cmake-build-debug/dumbot $1
else
    echo "First run compile.sh to compile the code"
fi
