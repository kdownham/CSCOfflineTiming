#!/bin/bash

CMSSWVERSION=$1
SCRAMARCH=$2
JOBTAG=$3
USER=$4
RUN=$5

JOBDIR=$(pwd)
OUTDIR="/eos/user/k/kdownham/CSCOfflineTiming/condor_output/$JOBTAG"

# set up environment
export SCRAM_ARCH=$SCRAMARCH
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 project CMSSW $CMSSWVERSION`
cd $CMSSWVERSION/src
eval `scramv1 runtime -sh`
tar -xf $JOBDIR/BabyMaker.tar
scramv1 b -j 1
cp -r BabyMaker/ $JOBDIR
cd $JOBDIR

mkdir RUNDIR
mv condor_template_cfg.py RUNDIR/
cd RUNDIR

# execute the ntuple maker
cmsRun condor_template_cfg.py 

RUN_STATUS=?
if [ $RUN_STATUS -neq 0 ]
then
  exit $RUN_STATUS
fi
