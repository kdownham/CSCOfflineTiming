#!/bin/bash

# set the proper environment variables
CMSSWVERSION="CMSSW_12_4_6"
SCRAMARCH="slc7_amd64_gcc10"
JOBDIR=$(pwd)
TARDIR="/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming"


# set up environment for running jobs
export SCRAM_ARCH=$SCRAMARCH
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 project CMSSW $CMSSWVERSION`
cd $CMSSWVERSION/src
eval `scramv1 runtime -sh`
#tar cvf CSCTiming.tar $TARDIR
cp -r $TARDIR/CSCTiming.tar .
echo "============Should be in CMSSW/src directory============="
pwd
ls
#scramv1 b -j 1
tar -xvf CSCTiming.tar
echo "==================================="
pwd
ls
#scramv1 b -j 1
echo "============Going into BabyMaker====================="
cd CSCOfflineTiming/CSCTimingBabyMaker
scram b -j 12
cd test/
cmsRun single_mu_test_cfg.py

#mkdir RUNDIR
#mv BabyMaker/test/single_mu_test_cfg.py RUNDIR/
#cd RUNDIR

#python3 test/single_mu_test_cfg.py 
