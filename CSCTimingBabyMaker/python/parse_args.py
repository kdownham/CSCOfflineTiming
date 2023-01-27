import argparse

def parse_arguments():
	parser=argparse.ArgumentParser()
	parser.add_argument("-i","--inputDataset",help="Dataset to run over",type=str,required=True)
	parser.add_argument("-o","--outputDir",help="Output dir for ntuples",type=str,required=True)
	parser.add_argument("-t","--globalTag",help="Global tag for dataset",type=str,required=True)
	args = parser.parse_args()
	return args
