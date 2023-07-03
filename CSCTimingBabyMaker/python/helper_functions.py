######################################################
# Helper functions for setting up and running
# the CSCTimingBabyMaker code via condor submission
#
######################################################
# Code based on scripts from CSCValidation repo
# with contributions from:
#
# Ben Knapp, Northeastern University
# Devin Taylor, UW-Madison
# Joe Crowley, UC Santa Barbara
#
######################################################
#
# Author:
#	Keegan Downham,
#	UC Santa Barbara
#
######################################################

import time
import os
import subprocess
import json
import pandas as pd
date_format = "%Y/%m/%d %H:%M:%S"

def get_from_dataset(dataset, streamQ=True, versionQ=False, eventContentQ=True):
    [stream, version, eventContent] = dataset.split('/')[1:]
    if "GEN" in dataset:
        stream = stream+version.replace("_","")
    return [s for s, b in zip([stream,version,eventContent], [streamQ,versionQ,eventContentQ]) if b]

def run_ntuple_jobs(config):
    '''
      config parameters:
	parameter   type   default   comment
	dataset     str    ''	     REQUIRED, /*/*/*
        outdir      str    ''        absolute path for the output root files
	globaltag   str    ''        REQUIRED
	runNumber   int    0         process a single run (default to all runs)
        jobtag      str    ''        unique tagname for jobs 
    '''

    dataset = config['dataset']
    globaltag = config['globaltag']
    #outputdir = config['outdir']
    input_files = config['input_files']
    run = config['runNumber']
    maxJobNum = config['maxJobNum']
    force = config['force']
    dryRun = config['dryRun']
    triggers = config['triggers']
    n_events = config['n_events']
    jobtag = config['jobtag']

    time0 = time.time()

    [stream, eventContent] = get_from_dataset(dataset)

    CMSSW_BASE = os.getenv('CMSSW_BASE')
    basedir = CMSSW_BASE + '/src/CSCOfflineTiming/CSCTimingBabyMaker'
    rundir = basedir+'/outputs/tasks/{}/run_{}'.format(stream, run)
    os.system('mkdir -p '+rundir + "/logs")
    os.chdir(rundir)

    os.system('ln -sf '+basedir+'/BabyMaker.tar {}/'.format(rundir))
    os.system('cp -f '+basedir+'/scripts/job_duties.sh {}/'.format(rundir))

    replace_template_parameters(basedir, input_files, dataset, globaltag, rundir, CMSSW_BASE, run, stream, jobtag, n_events=str(min(int(n_events),10**2)))

    os.system('condor_submit '+rundir+'/job.sub')
    os.chdir(basedir)

def replace_template_parameters(basedir, input_files, dataset, globaltag, rundir, CMSSW_BASE, run, stream, jobtag, n_events=100):
    # replace template parameters in ../test/condor_template_cfg.py
    with open(basedir+'/test/condor_template_cfg.py','r') as f:
        condor_template_cfg = f.read()

    # format the input files for the ESSource in the config template
    input_files_str = "', '".join(input_files)

    # May need to play around with this to create different directories for the output jobs
    #outputdir = os.getenv("OUTDIR")
    # For now I am going to do the lazy option and hardcode in the output directory
    outputdir = '/eos/user/k/kdownham/CSCOfflineTiming/condor_output/'+jobtag

    condor_template_cfg = condor_template_cfg.replace('GLOBALTAG_REPLACETAG', globaltag)
    condor_template_cfg = condor_template_cfg.replace('FILENAME_REPLACETAG', input_files_str)
    condor_template_cfg = condor_template_cfg.replace('MAXEVENTS_REPLACETAG', n_events)
    condor_template_cfg = condor_template_cfg.replace('OUTPUTNAME_REPLACETAG', outputdir+'/output.root')

    with open(rundir+'/condor_template_cfg.py','w') as f:
        f.write(condor_template_cfg)

    # Replace template parameters in job_duties.sh
    with open(basedir+'/scripts/job_duties.sh','r') as f:
        job_duties = f.read()

    job_duties = job_duties.replace('RUN_REPLACETAG', run)
    job_duties = job_duties.replace('STREAM_REPLACETAG', stream)

    with open(basedir+'/scripts/job_duties.sh','w') as f:
        f.write(job_duties)

    # Replace template parameters in job.sub
    with open(basedir+'/scripts/job.sub','r') as f:
        job_sub = f.read()

    job_sub = job_sub.replace('CONDOROUTDIR_REPLACETAG', rundir)  # may want to change this to the outputdir.....
    job_sub = job_sub.replace('UID_REPLACETAG', str(os.getuid()))

    # args for job.sub
    job_sub = job_sub.replace('CMSSWVERSION_REPLACETAG', os.getenv("CMSSW_VERSION"))
    job_sub = job_sub.replace('SCRAMARCH_REPLACETAG', os.getenv("SCRAM_ARCH"))
    job_sub = job_sub.replace('USER_REPLACETAG', os.getlogin())
    job_sub = job_sub.replace('JOBTAG_REPLACETAG', jobtag)
    job_sub = job_sub.replace('RUN_REPLACETAG', run)

    with open(rundir+'/job.sub','w') as f:
        f.write(job_sub)

def initialize_ntuplejobs(stream):
    # setup working directory for stream
    CMSSW_BASE = os.getenv('CMSSW_BASE')
    basedir = CMSSW_BASE + '/src/CSCOfflineTiming/CSCTimingBabyMaker'
    if not os.path.exists(basedir+'/'+stream):
        os.system('mkdir -p {}/outputs/processedRuns/{}'.format(basedir,stream))

    # begin running
    start=time.strftime(date_format, time.localtime())
    print(f'CSCBabyMaker job initiated at {start}')
    os.chdir(basedir + '/outputs/processedRuns')

    print('Reading previously processed runs')
    procFile = basedir + '/outputs/processedRuns/processedRuns.txt'
    procRuns = []
    if os.path.exists(procFile):
        with open(procFile, 'r') as file:
            procRuns = file.readlines()
    procRuns = [x.rstrip() for x in procRuns] # format RUNNUM_NUMEVENTS

    print('Reading previous process time')
    timeFile = basedir + '/outputs/processedRuns/processTime.txt'
    procTimes = []
    if os.path.exists(timeFile):
        with open(timeFile, 'r') as file:
            procTimes = file.readlines()
    procTimes = [x.rstrip() for x in procTimes]
    prevTime = float(procTimes[-1]) - 12*60*60 if procTimes else float(time.time()) - 7*24*60*60
    prevdate = pd.to_datetime(prevTime, unit='s').strftime(date_format)
    print(f'Last run: {prevdate}')
