#########################################################################
#
# job_submit.py - script for submitting CSCTimingBabyMaker jobs to condor
#
#########################################################################
#
# Author:
#       Keegan Downham
#	University of california, Santa Barbara
#
#########################################################################

# Needs to get dataset, singlerun, jobtag, CMSSWVERSION, scram_arch, and outputdir from argparser

import time
import os

import use_dbs

from helper_functions import get_from_dataset, run_ntuple_jobs, initialize_ntuplejobs
from replace_and_submit import replace_and_submit

def job_submit(config):

    print("Debug 1")

    dataset = config['inputDataset']
    print("Debug 2")
    runNum = config['singleRun']
    jobTag = config['jobTag']
    cmssw = config['cmsswVer']
    scram = config['scramVer']
    output = config['outputDir']
    globalTag = config['globalTag']

    print("Dataset = {}".format(dataset))
    print("Run number = {}".format(runNum))
    print("Job tag = {}".format(jobTag))
    print("CMSSW Version = {}".format(cmssw))
    print("Scram arch = {}".format(scram))
    print("Output directory = {}".format(output))
    print("Global tag = {}".format(globalTag))
    
    [stream, eventContent] = get_from_dataset(dataset)

    # set up directories, check previous runs
    initialize_ntuplejobs(stream)

    # Add a split here for single run vs whole dataset
    if runNum:   
        files=use_dbs.get_files(dataset=dataset, runs=[runNum])
        n_events = sum([sum(eval(f['events'])) for f in files])
        print(f"\nNumber of events in all files listed in DAS for this run/dataset:{n_events}")
        input_files = [f['file'] for f in files]
        config_tmp = config.copy()
        config_tmp.update({"n_events": n_events, "input_files": input_files, "runNumber": str(runNum), "stream": stream})
        print(f"Processing run {runNum}") 
        replace_and_submit(config_tmp)
        runs_to_update = runNum
    else:
        blocks = use_dbs.get_blocks(dataset=dataset)
        run_list = set()

        for block in blocks:
            runs = use_dbs.get_runs(block=block)
            run_list.update(set(runs))

        run_list = sorted(run_list)
        fileRunMap = {}
        eventRunMap = {}
        files = use_dbs.get_files(dataset=dataset, runs=run_list)

        for run in run_list:
            #print("Run = {}, EventRunMap = {}".format(run,sum([sum(eval(f['events'])) for f in files if int(f['run'])==run])))
            eventRunMap[run] = sum([sum(eval(f['events'])) for f in files if int(f['run'])==run])
            fileRunMap[run] = [f['file'] for f in files if int(f['run'])==run]
            
        runs_to_update = [run for run in run_list if fileRunMap[run] and eventRunMap[run]>25000]

        print('Runs to update:')
        for run in runs_to_update:
            print(f'    Run {run}: {len(fileRunMap[run])} files, {eventRunMap[run]} events')


        for run in runs_to_update:
                #if int(run)<MINRUN: continue
                config_tmp = config.copy()
                config_tmp.update({"runNumber":str(run)})
                config_tmp.update({"n_events":str(eventRunMap[run])})
                config_tmp.update({"input_files":fileRunMap[run]})
                config_tmp.update({"stream":stream})
                print(f"Processing run {run}")
                #run_ntuple_jobs(config_tmp)
                #Print out the configuration for the individual jobs
                #print("Configuration: ")
                #print(config_tmp)
                #TODO: create the appropriate directories to store the necessary scripts for running the jobs and logs	    
                 
                #Write a function that takes the config_tmp parameters and uses them to replace the parameters in the job_baby.sub, job_baby.sh and condor_template_cfg.py files
                replace_and_submit(config_tmp)
    return runs_to_update

def main():

    import parse_args as p
    config = p.parser(return_dict=True)

    print("Config = {}".format(config))

    runs_to_update = job_submit(config)

    print("Runlist = {}".format(runs_to_update))

if __name__ == "__main__":
    main() 
