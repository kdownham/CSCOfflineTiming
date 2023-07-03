#! /bin/bash
#################################################
# submitBabyMakerJobs
#
# Example for running CSC Timing nTuple maker
#
# Authors
#  Keegan Downham, UC Santa Barbara
#################################################

CMSSWVERSION="CMSSW_12_4_6"
DATASET="/Muon/Run2022D-PromptReco-v2/AOD"
GLOBALTAG="124X_dataRun3_Prompt_v4"
SINGLERUN=""
JOBTAG="Muon_2022D_Full"

# Tell the script where to run the jobs
JOBDIR="$(pwd)"
OUTDIR="/eos/user/k/kdownham/CSCOfflineTiming/condor_output/$JOBTAG"


INITIALDIR=$(pwd)

#source /cvmfs/cms.cern.ch/cmsset_default.sh
#eval `scramv1 project CMSSW $CMSSWVERSION`
#cd $CMSSWVERSION/src
#eval `scramv1 runtime -sh`
voms-proxy-init --rfc --voms cms -valid 192:00

#scram b -j 12
cd ..
#cd CSCOfflineTiming
scram b -j 12
tar -cf BabyMaker.tar CSCTimingBabyMaker
#
cd CSCTimingBabyMaker
mv ../BabyMaker.tar .

# submit ntuple maker jobs
# python3 python/submit_ntuple_jobs.py $DATASET $GLOBALTAG

echo $CMSSWVERSION
echo $DATASET
echo $GLOBALTAG
echo $OUTDIR
echo $JOBTAG

python3 python/submit_ntuple_jobs.py $DATASET $GLOBALTAG
