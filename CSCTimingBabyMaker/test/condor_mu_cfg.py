import sys
sys.path.insert(1,'../python')  #this is required to access python files/functions in python dir

import argparse

import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

#import CSCOfflineTiming.CSCTimingBabyMaker.cmseostools
import use_dbs  # get_files has arguments dataset (string), command (string), and runs 

parser=argparse.ArgumentParser()
parser.add_argument("-i","--inputDataset",help="Dataset to run over",type=str,required=True)
parser.add_argument("-o","--outputDir",help="Output dir for ntuples",type=str,required=True)
parser.add_argument("-t","--globalTag",help="Global tag for dataset",type=str,required=True)
args = parser.parse_args()

#print(args.inputDataset)
#print(args.outputDir)
#print(args.globalTag)

files = use_dbs.get_files(dataset=args.inputDataset)
input_file_names = [f['file'] for f in files]

input_files_str = "', 'file:".join(input_file_names)

input_files = "'"+input_files_str+"'"  # To get the file string in the proper format

print(input_files)
print('output directory = ',args.outputDir)
print('global tag = ',args.globalTag)

process = cms.Process("CSCTIMING")

process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v4'
process.GlobalTag.globaltag = args.globalTag

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'eos/user/k/kdownham/CSCOfflineTiming/'+args.outputDir+'/test.root'
fileExtension = '.root'
outfileName = baseFileName
print(outfileName)

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
				#input_file_names,
			)
)



process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(True)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName+fileExtension

process.p = cms.Path(process.cscTimingBabyMaker)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))


process.e = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
