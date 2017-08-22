#!/bin/bash

pushd $CMSSW_BASE/src

cmsenv
git cms-addpkg CalibMuon/CSCCalibration
git cms-addpkg CondFormats/CSCObjects
git cms-addpkg OnlineDB/CSCCondDB
git cms-addpkg RecoLocalMuon/CSCRecHitD
git cms-addpkg RecoLocalMuon/CSCValidation
git cms-addpkg RecoMuon/TrackingTools
git cms-addpkg CondCore/CondDB

# git clone git@github.com:fgolf/CSCOfflineTiming.git
./CSCOfflineTiming/CSCTimingBabyMaker/setup/setup.sh
./CSCOfflineTiming/CSCTimingAnalyzer/setup/setup.sh

cd $CMSSW_BASE/src 
scram b -j 12

popd
