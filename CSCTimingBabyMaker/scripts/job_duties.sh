#!/bin/bash

CMSSWVERSION=$1
SCRAMARCH=$2
JOBTAG=$3
USER=$4
RUN=$5

JOBDIR=$(pwd)
#OUTDIR="/eos/user/k/kdownham/CSCOfflineTiming/condor_output/$JOBTAG"
#OUTDIR="/eos/cms/store/group/dpg_csc/comm_csc/csctiming/Run3/2022/"

TARFILEDIR="/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming"

# set up environment
export SCRAM_ARCH=$SCRAMARCH
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 project CMSSW $CMSSWVERSION`
cd $CMSSWVERSION/src
eval `scramv1 runtime -sh`
#tar -xf $JOBDIR/BabyMaker.tar
cp -r $TARFILEDIR/CSCTiming.tar .
echo "================================="
echo "Should be in CMSSW/src directory "
echo "================================="
pwd
ls
#scramv1 b -j 1
#cp -r BabyMaker/ $JOBDIR
#cd $JOBDIR

tar -xvf CSCTiming.tar

cd CSCOffineTiming/CSCTimingBabyMaker
scram b -j 12
cd test/

#mkdir RUNDIR
#mv condor_template_cfg.py RUNDIR/
#cd RUNDIR

# execute the ntuple maker
cmsRun condor_template_cfg.py 

RUN_STATUS=?
if [ $RUN_STATUS -neq 0 ]
then
  exit $RUN_STATUS
fi
