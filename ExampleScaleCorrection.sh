#! /bin/bash
#
# Run the following to compile and execute the example.
#
rm ExampleScaleCorrection.exe >& /dev/null
ROOTLIBS="-L/usr/lib `$ROOTSYS/bin/root-config --glibs` -lMathCore -lMinuit"
ROOTINCDIR=`$ROOTSYS/bin/root-config --incdir`
g++ -std=c++14 -I${ROOTINCDIR} -o ExampleScaleCorrection.exe \
    include/GeneralizedEndpoint.cc ${ROOTLIBS} ExampleScaleCorrection.C
#
./ExampleScaleCorrection.exe
#
