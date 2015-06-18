#!/bin/bash

if [ -z "$CMSSW_BASE" ];
then
    echo "Need to setup CMSSW environment. Exiting."
    exit 1
fi

if [ ! -f ${CMSSW_BASE}/src/RecoLocalMuon/CSCRecHitD/BuildFile.xml ];
then
    cd ${CMSSW_BASE}/src
    git cms-addpkg RecoLocalMuon/CSCRecHitD
fi

sed -i '15d' ${CMSSW_BASE}/src/RecoLocalMuon/CSCRecHitD/BuildFile.xml
echo "<export>" >> ${CMSSW_BASE}/src/RecoLocalMuon/CSCRecHitD/BuildFile.xml
echo "   <lib   name=\"1\"/>" >> ${CMSSW_BASE}/src/RecoLocalMuon/CSCRecHitD/BuildFile.xml
echo "</export>" >> ${CMSSW_BASE}/src/RecoLocalMuon/CSCRecHitD/BuildFile.xml
