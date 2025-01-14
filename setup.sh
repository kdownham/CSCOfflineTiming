#!/bin/bash

CMSSW_VERSION=CMSSW_12_5_0_pre4

if [ ! -d $CMSSW_VERSION ]; then
    cmsrel $CMSSW_VERSION
fi

cd $CMSSW_VERSION/src
cmsenv
git cms-addpkg CalibMuon/CSCCalibration
git cms-addpkg CondFormats/CSCObjects
git cms-addpkg OnlineDB/CSCCondDB
git cms-addpkg RecoLocalMuon/CSCRecHitD
git cms-addpkg RecoLocalMuon/CSCValidation
git cms-addpkg RecoMuon/TrackingTools
git cms-addpkg CondCore/CondDB

git clone https://github.com/kdownham/CSCOfflineTiming.git
#./CSCOfflineTiming/CSCTimingBabyMaker/setup/setup.sh
#./CSCOfflineTiming/CSCTimingAnalyzer/setup/setup.sh

#scram b -j 12
