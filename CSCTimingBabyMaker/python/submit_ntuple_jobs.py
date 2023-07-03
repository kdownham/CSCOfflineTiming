import time
import os

from helper_functions import get_from_dataset, run_ntuple_jobs, initialize_ntuplejobs

import use_dbs

def submit_ntuple_jobs(config):

    dataset = config['dataset']
    globaltag = config['globaltag']
    #outdir = config['outdir']
    force = config['force']
    dryRun = config['dryRun']
    maxJobNum = config['maxJobNum']
    MINRUN = config['minrun']
    singleRun = config['runNumber']
    time0 = time.time()

    print("dataset = {}".format(dataset))
    print("global tag = {}".format(globaltag))
    #print("output directory = {}".format(outdir))
    #print("run number = {}".format(runNumber))

    [stream, eventContent] = get_from_dataset(dataset)

    # set up directories, check previous runs
    initialize_ntuplejobs(stream)

    blocks = use_dbs.get_blocks(dataset=dataset)
    new_runs = set()

    for block in blocks:
        runs = use_dbs.get_runs(block=block)
        new_runs.update(set(runs))

    new_runs = sorted(new_runs)
    fileRunMap = {}
    eventRunMap = {}
    files = use_dbs.get_files(dataset=dataset, runs=new_runs)

    for run in new_runs:
        eventRunMap[run] = sum([sum(eval(f['events'])) for f in files if int(f['run'])==run])
        fileRunMap[run] = [f['file'] for f in files if int(f['run'])==run]

    runs_to_update = [run for run in new_runs if fileRunMap[run] and eventRunMap[run]>25000]

    print('Runs to update:')
    for run in runs_to_update:
        print(f'    Run {run}: {len(fileRunMap[run])} files, {eventRunMap[run]} events')

    for run in runs_to_update:
            if int(run)<MINRUN: continue
            config_tmp = config.copy()
            config_tmp.update({"runNumber":str(run)})
            config_tmp.update({"n_events":str(eventRunMap[run])})
            config_tmp.update({"input_files":fileRunMap[run]})
            print(f"Processing run {run}")
            run_ntuple_jobs(config_tmp)

    return runs_to_update

def main():
 
    import parsingtools as parser
    config = parser.parse_args(return_dict=True)

    runs_to_update = submit_ntuple_jobs(config)

    print("runs to update = {}".format(runs_to_update))
    

if __name__ == "__main__":
    main() 
