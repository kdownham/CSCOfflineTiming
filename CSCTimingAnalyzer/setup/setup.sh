#!/bin/bash

if [ -z "$CMSSW_BASE" ];
then
    echo "Need to setup CMSSW environment. Exiting."
    exit 1
fi

if [ ! -f ${CMSSW_BASE}/src/RecoLocalMuon/CSCValidation/BuildFile.xml ];
then
    cd ${CMSSW_BASE}/src
    git cms-addpkg RecoLocalMuon/CSCValidation
fi

sed -i '19d' ${CMSSW_BASE}/src/RecoLocalMuon/CSCValidation/BuildFile.xml
echo "<export>" >> ${CMSSW_BASE}/src/RecoLocalMuon/CSCValidation/BuildFile.xml
echo "   <lib   name=\"1\"/>" >> ${CMSSW_BASE}/src/RecoLocalMuon/CSCValidation/BuildFile.xml
echo "</export>" >> ${CMSSW_BASE}/src/RecoLocalMuon/CSCValidation/BuildFile.xml

if [ ! -f ${CMSSW_BASE}/src/OnlineDB/CSCCondDB/BuildFile.xml ];
then
    cd ${CMSSW_BASE}/src
    git cms-addpkg OnlineDB/CSCCondDB
fi
