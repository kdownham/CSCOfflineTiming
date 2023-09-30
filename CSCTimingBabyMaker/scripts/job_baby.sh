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
ANADIR=$4
ANAOUTPUT=$5
NEWHEURISTIC=$6
RUNNUM=$7
HEURISTICFILE=$8

TARDIR="/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming"

# set up the environment for running jobs
export SCRAM_ARCH=$SCRAMARCH
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 project CMSSW $CMSSWVERSION`
cd $CMSSWVERSION/src
eval `scramv1 runtime -sh`
cp -r $TARDIR/CSCTiming.tar .
tar -xvf CSCTiming.tar
#cd CSCOfflineTiming/CSCTimingBabyMaker
cd CSCOfflineTiming
scram b -j 12
cp -r $RUNDIR/condor_template_cfg.py CSCTimingBabyMaker/test/condor_cfg.py
cp -r $RUNDIR/condor_template_analyzer_cfg.py CSCTimingAnalyzer/test/.
cp -r $RUNDIR/$HEURISTICFILE ../.

echo "Printing working directory after copying config files"
pwd
cd CSCTimingBabyMaker/test/
cmsRun condor_cfg.py

echo "What is the name of the output file?"
ls

mv *.root ../../CSCTimingAnalyzer/test/.

# Location of output of CSCTimingBabyMaker: CMSSWVERSION_REPLACETAG/src/CSCOfflineTiming/CSCTimingBabyMaker/test/output_RUNNUM_REPLACETAG_useMuonSegmentMatcher.root 
echo "Printing working directory after running the CSCTimingBabyMaker"
pwd
cd ../../CSCTimingAnalyzer/test/
echo "Let's make sure that the root file we copied over is actually here....."
ls
cmsRun condor_template_analyzer_cfg.py

# Let's see what the name of the output file is....
ls 

# Copy the output file from the timing analyzer to the macros area, where we will either derive new corrections or make plots with existing corrections
#mv 'output_'+$RUNNUM+'useMuonSegmentMatcher.root' ../macros/ana_output.root


#UNCOMMENT THE FOLLOWING BLOCK ONLY AFTER CONFIRMING THAT THE ABOVE WORKS===========================================================================
#if [ $NEWHEURISTIC -eq true ]
#then
#	echo "You have chosen to derive new heuristic corrections!"
#	root -l 'determineHeuristicCorrections.C("ana_output.root","heuristicCorrections_"+$RUNNUM+".txt")'
#        tail -f heuristicCorrections_"$RUNNUM".txt
#else
#	echo "You have declined to derive new heuristic corrections! SHAME ON YOU!"
#fi

# At some point I need to copy over the necessary files from the run directory (config file and condor scripts (.sh and .sub))

