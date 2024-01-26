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
cp -r $RUNDIR/Cert_Collisions2022_355100_362760_Muon.json CSCTimingAnalyzer/test/.

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

echo "============================================================================================================================================="
echo "MOVING INTO THE MACROS DIRECTORY"
echo "============================================================================================================================================="
cd ../macros

#UNCOMMENT THE FOLLOWING BLOCK ONLY AFTER CONFIRMING THAT THE ABOVE WORKS===========================================================================
if [ "$NEWHEURISTIC" = "True" ];
then
	echo "You have chosen to remove heuristic corrections!"
	#root -l 'determineHeuristicCorrections.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root","heuristicCorrections_'$RUNNUM'.txt")'
        ##tail -f heuristicCorrections_"$RUNNUM".txt
	#echo "What is the name of the output file?"
	#ls -la
	#echo "Now let's compute new anode times!"
	#root -l 'determineAnodeOffsets.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root","anode_bx_offsets_'$RUNNUM'.txt")'
	#echo "Making sure that the file was actually made....."
	#ls -la
        echo "Making plots for your viewing pleasure!"
	root -l 'combineAnodeTimingDistributions.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root")'
	root -l 'combineRechitTimingDistributions.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root")'
	root -l 'combineSegmentTimingDistributions.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root")'
	root -l 'makeAnodeTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root",true,"")'
	root -l 'makeAnodeTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root",false,"")'
	root -l 'makeMeanTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root",true,"")'
        root -l 'makeMeanTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root",false,"")'
        root -l 'makeMuonTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root")'
        root -l 'makeSegmentMeanTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList_removeHeuristicCorr.root",true)'
else
	echo "You are making plots with heuristic corrections applied!"
	echo "The cathode times should be centered at zero if you are using the proper corrections!"
	echo "Electing instead to make plots! "
        #root -l 'combineAnodeTimingDistributions.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root")'
        #root -l 'combineRechitTimingDistributions.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root")'
        #root -l 'combineSegmentTimingDistributions.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root")'
        #root -l 'makeAnodeTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root",true,"")'
        #root -l 'makeAnodeTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root",false,"")'
        #root -l 'makeMeanTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root",true,"")'
        #root -l 'makeMeanTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root",false,"")'
        #root -l 'makeMuonTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root")'
        #root -l 'makeSegmentMeanTimingPlot.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root",true)'
        #echo "Let's derive some new anode times!"
	#root -l 'determineAnodeOffsets.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root","anode_bx_offsets_'$RUNNUM'.txt")'
	root -l 'dumpPlots.C("../test/output_'$RUNNUM'_timing_applyGoodRunList.root","'$RUNNUM'","removeAnodeCorr")'
fi

# At some point I need to copy over the necessary files from the run directory (config file and condor scripts (.sh and .sub))

