#! /bin/bash
#
# Run the following to compile and execute the example.
#
rm ExampleScaleCorrection.exe >& /dev/null
g++ -std=c++11 -o ExampleScaleCorrection.exe \
    include/GeneralizedEndpoint.cc ExampleScaleCorrection.C
#
./ExampleScaleCorrection.exe
#
