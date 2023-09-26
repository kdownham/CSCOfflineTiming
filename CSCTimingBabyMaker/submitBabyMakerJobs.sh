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
SINGLERUN=""       #This should be the run number that you want to run the CSCTimingBabyMaker on (blank means all runs in dataset)
JOBTAG="Muon_2022D_Full"
SCRAMARCH="slc7_amd64_gcc10"

#TARFILEDIR="/afs/cern.ch/user/k/kdownham/CSCValidation/CMSSW_12_4_6/src/CSCOfflineTiming"


# Tell the script where to run the jobs
JOBDIR="$(pwd)"
#OUTDIR="/eos/user/k/kdownham/CSCOfflineTiming/condor_output/$JOBTAG"
OUTDIR="/eos/cms/store/group/dpg_csc/comm_csc/csctiming/Run3/2022$DATASET/$JOBTAG"


INITIALDIR=$(pwd)

#export SCRAM_ARCH=$SCRAMARCH
#
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#eval `scramv1 project CMSSW $CMSSWVERSION`
#cd $CMSSWVERSION/src
#eval `scramv1 runtime -sh`
##voms-proxy-init --rfc --voms cms -valid 192:00
#
#cp -r $TARDIR/CSCTiming.tar .
#
#echo "================================="
#echo "Should be in CMSSW/src directory"
#echo "================================="
#
#pwd
#ls
#tar -xvf CSCTiming.tar
#
#cd CSCOfflineTiming/CSCTimingBabyMaker
#echo "================================="
#echo "Building the BabyMaker..........."
#echo "================================="
#
#scram b -j 12

# submit ntuple maker jobs
# python3 python/submit_ntuple_jobs.py $DATASET $GLOBALTAG

echo "CMSSW version is: " $CMSSWVERSION
echo "Dataset to run on: " $DATASET
echo "Global Tag: " $GLOBALTAG
echo "Output Directory: " $OUTDIR
echo "Tag of the job: " $JOBTAG

python3 python/submit_ntuple_jobs.py $DATASET $GLOBALTAG
