import time
import os
import subprocess
import json
import pandas as pd

def replace_and_submit(config):
	
       dataset = config['inputDataset']
       runNum = config['runNumber']
       jobTag = config['jobTag']
       cmssw = config['cmsswVer']
       scram = config['scramVer']
       output = config['outputDir']
       globalTag = config['globalTag']
       n_events = config['n_events']
       input_files = config['input_files']
       stream = config['stream']

       CMSSW_BASE = os.getenv('CMSSW_BASE')
       basedir = CMSSW_BASE + '/src/CSCOfflineTiming'
       rundir = basedir+'/CSCTimingBabyMaker/outputs/tasks/{}/{}/run_{}'.format(stream, jobTag, runNum)

       os.system('mkdir -p '+rundir)
       #os.system('cp -r '+basedir+'/CSCTiming.tar '+rundir+'/.')
       os.system('mkdir -p '+rundir+'/logs')

       # Now we need to copy over the necessary files into the run directory (i.e. the config file, and the condor submission files) 

       with open(basedir+'/CSCTimingBabyMaker/test/condor_template_cfg.py','r') as f:
        condor_template_cfg = f.read()

       #join the files in the appropriate format for the ESSource
       input_files_str = "', 'root://xrootd-cms.infn.it/".join(input_files)

       input_files_str = "root://xrootd-cms.infn.it/"+input_files_str

       #outputdir = '/eos/user/k/kdownham/CSCOfflineTiming/condor_output/'+jobTag

       condor_template_cfg = condor_template_cfg.replace('GLOBALTAG_REPLACETAG', globalTag)
       condor_template_cfg = condor_template_cfg.replace('FILENAME_REPLACETAG', input_files_str)
       condor_template_cfg = condor_template_cfg.replace('MAXEVENTS_REPLACETAG', str(-1))  # Number of events per file that you want to run over
       #condor_template_cfg = condor_template_cfg.replace('OUTPUTNAME_REPLACETAG', outputdir+'/'+runNum+'/output')
       condor_template_cfg = condor_template_cfg.replace('OUTPUTNAME_REPLACETAG', 'output_'+runNum) # This is the name/location where the finished ntuples are sent

       with open(rundir+'/condor_template_cfg.py','w') as f:
           f.write(condor_template_cfg)

       os.system('mkdir -p '+output)

       with open(basedir+'/CSCTimingBabyMaker/scripts/job_baby.sub','r') as f:
           job_sub = f.read()

       job_sub = job_sub.replace('SCRAMARCH_REPLACETAG', scram)
       job_sub = job_sub.replace('CMSSWVERSION_REPLACETAG', cmssw)
       job_sub = job_sub.replace('RUNDIR_REPLACETAG', rundir)
       job_sub = job_sub.replace('CONDOROUTDIR_REPLACETAG', 'root://eosuser.cern.ch/'+output)
       job_sub = job_sub.replace('RUNNUM_REPLACETAG', runNum)

       with open(rundir+'/job_baby.sub','w') as f:
           f.write(job_sub) 

       with open(basedir+'/CSCTimingBabyMaker/scripts/job_baby.sh','r') as f: 
           job_baby = f.read()

       with open(rundir+'/job_baby.sh','w') as f:
           f.write(job_baby)

       # Now submit the jobs from the appropriate run directory
       os.system('condor_submit '+rundir+'/job_baby.sub')
