#!/bin/bash

## This is to run the first program main.cpp

if [ -e "./dumbot" ]; then
    ./dumbot $1
else
    echo "First run compile.sh to compile the code"
fi
