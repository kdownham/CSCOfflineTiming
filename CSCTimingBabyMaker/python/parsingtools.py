

import sys
import argparse

def parse_command_line(argv):


    parser = argparse.ArgumentParser(description="Submit CSCTimingBabyMaker Jobs")

    parser.add_argument('dataset', type=str, help='/*/*/* (stream and event content)')
    parser.add_argument('globaltag', type=str, help='promptrecoGlobalTag for Era')
    parser.add_argument('-rn','--runNumber', type=int, default=0, help='Process a specific run (defult is all runs)')
    #parser.add_argument('-od','--outdir', type=str, help='Output directory for the BabyMaker condor jobs (where the root files are sent)')
    parser.add_argument('-ro','--retrieveOutput', action='store_true', help='Retrieve the output of a previous run and produce the HTML.')
    parser.add_argument('-br','--buildRunList', action='store_true', help='Build the runlist.json file for website.')
    parser.add_argument('-dr','--dryRun', action='store_true', help='Don\'t submit, just create the objects')
    parser.add_argument('-f','--force', action='store_true', help='Force a recipe.')
    parser.add_argument('-t','--triggers', nargs='*', help='Optionally run on additional triggers.')
    parser.add_argument('-mj','--maxJobNum', type=int, default=200, help='Number of batch jobs.')
    parser.add_argument('-mr','--minrun', type=int, default=300576, help='Min run number.')
    parser.add_argument('-jt','--jobtag', type=str, default='test', help='Unique tagname for job.')

    args = parser.parse_args(argv)
    return args

def parse_args(return_dict=False):
    argv = sys.argv[1:]
    args = parse_command_line(argv)

    #if not args.triggers:
    #    args.triggers = []

    dataset = args.dataset.split('/')[1:]

    if len(dataset) != 3:
        msg = 'Invalid dataset. Argument should be in the form of a DAS query (/*/*/*).'
        raise Exception(msg)

    if return_dict:
        return dict(args.__dict__)

    return args

if __name__ == '__main__':
    args = parse_args()
    print(args)
