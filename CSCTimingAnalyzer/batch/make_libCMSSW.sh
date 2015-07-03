#!/bin/bash

DIR=$PWD
cd $CMSSW_BASE
echo "Making the tarball..."
stuff1=`find src/ -name "data"`
stuff2=`find src/ -name "interface"`
stuff3=`find src/ -name "python"`
tar -chzvf lib_CMSSW.tar.gz lib/ python/ $stuff1 $stuff2 $stuff3
mv lib_CMSSW.tar.gz $DIR/lib_CMSSW.tar.gz
cd $DIR
echo "Your tarball is lib_CMSSW.tar.gz."
