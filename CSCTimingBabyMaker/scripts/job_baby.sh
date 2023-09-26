#!/bin/bash

# Load in the arguments provided in the job_baby.sub script
###########################################################
#
# The following arguments are needed:
#	SCRAMARCH
#	CMSSWVERSION
#
#
###########################################################

# set the proper environment variables

SCRAMARCH=$1
CMSSWVERSION=$2
RUNDIR=$3

TARDIR="/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming"

# set up the environment for running jobs
export SCRAM_ARCH=$SCRAMARCH
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 project CMSSW $CMSSWVERSION`
cd $CMSSWVERSION/src
eval `scramv1 runtime -sh`
cp -r $TARDIR/CSCTiming.tar .
tar -xvf CSCTiming.tar
cd CSCOfflineTiming/CSCTimingBabyMaker
scram b -j 12
cp -r $RUNDIR/condor_template_cfg.py test/condor_cfg.py

cd test/
cmsRun condor_cfg.py

# At some point I need to copy over the necessary files from the run directory (config file and condor scripts (.sh and .sub))

