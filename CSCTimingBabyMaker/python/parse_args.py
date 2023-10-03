import sys

import argparse

def parse_arguments(argv):
	parser=argparse.ArgumentParser()
	parser.add_argument("-i","--inputDataset",help="Dataset to run over",type=str,required=True)
	parser.add_argument("-s","--singleRun",help="Run Number"            ,type=int, default=0,required=False)
	parser.add_argument("-j","--jobTag"   ,help="Tag associated with the job", type=str,required=True)
	parser.add_argument("-v","--cmsswVer" ,help="CMSSW version"         ,type=str,required=True)
	parser.add_argument("-a","--scramVer",help="scram_arch release"    ,type=str,required=True)	
	parser.add_argument("-o","--outputDir",help="Output dir for ntuples",type=str,required=True)
	parser.add_argument("-t","--globalTag",help="Global tag for dataset",type=str,required=True)
	parser.add_argument("-n","--newHeuristic",help="Derive new heuristic corrections?",type=str,required=True)
	parser.add_argument("-f","--heuristicFile",help="File containing heuristic corrections that you want to apply",type=str,required=True)
	parser.add_argument("-c","--outputAna",help="Output directory for new heuristic corrections",type=str,required=True)	
	args = parser.parse_args()
	return args

def parser(return_dict=False):
    argv = sys.argv[1:]
    args = parse_arguments(argv)

    #if not args.triggers:
    #    args.triggers = []

    print("args = {}".format(args))

    dataset = args.inputDataset.split('/')[1:]

    if len(dataset) != 3:
        msg = 'Invalid dataset. Argument should be in the form of a DAS query (/*/*/*).'
        raise Exception(msg)

    if return_dict:
        return dict(args.__dict__)

    return args

if __name__ == '__main__':
    args = parser()
    print(args)
