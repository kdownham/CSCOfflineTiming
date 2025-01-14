##############################################################################################################
#
# runBabyMakerJobs.sh - A shell script for running CSCTimingBabyMaker jobs on condor
# 
# Inputs:
#	CMSSWVERSION   - The version of CMSSW that you are using
#       SCRAMARCH      - The version of SCRAM_ARCH that you are using (should be tied to the CMSSWVERSION)
#       DATASET        - The dataset that you would like to run over
#       GLOBALTAG      - The Globaltag corresponding to the dataset (can be found on DAS)
#       SINGLERUN      - The run number that you would like to process (empty means all runs in a given dataset)
#       JOBTAG         - The unique tag that you want to give to your job (for your own reference)
#       OUTPUTDIR      - The directory where you want the output of your jobs to go
#       NEWHEURISTIC   - Do you want to derive new heuristic corrections?
#       HEURISTICFILE  - Which heuristic corrections do you want to use?
#       ANALYZEROUTPUT - The location that you want to send the output of the CSCTimingAnalyzer 
#
##############################################################################################################
#
# Author:
#       Keegan Downham
#	University of California, Santa Barbara
#
##############################################################################################################

CMSSWVERSION="CMSSW_12_4_6"
#SCRAMARCH="slc7_amd64_gcc10"
SCRAMARCH="el8_amd64_gcc10"
DATASET="/Muon0/Run2023C-PromptReco-v1/AOD"
SINGLERUN=367229
JOBTAG="2023C_newAnodes_v1"
OUTPUTDIR="/eos/cms/store/group/dpg_csc/comm_csc/csctiming/Run3/2023$DATASET/$JOBTAG"
GLOBALTAG="130X_dataRun3_Prompt_v2"
NEWHEURISTIC="False"
HEURISTICFILE="heuristicCorrections_byChamber_357900.txt"
ANALYZEROUTPUT="/afs/cern.ch/user/k/kdownham/CSC_Validation/$CMSSWVERSION/src/CSCOfflineTiming/CSCTimingAnalyzer/data/heuristic_corrections/"

#voms-proxy-init --rfc --voms cms -valid 192:00
#cp -r /tmp/x509up_u115282 .

export X509_USER_PROXY=/afs/cern.ch/user/k/kdownham/private/.x509up_u115282
echo "$PASSWORD" | voms-proxy-init -voms cms -rfc -out /afs/cern.ch/user/k/kdownham/private/.x509up_u115282 -valid 192:00

python3 python/job_submit.py -i $DATASET -j $JOBTAG -v $CMSSWVERSION -a $SCRAMARCH -o $OUTPUTDIR -t $GLOBALTAG -s $SINGLERUN -n $NEWHEURISTIC -f $HEURISTICFILE -c $ANALYZEROUTPUT
