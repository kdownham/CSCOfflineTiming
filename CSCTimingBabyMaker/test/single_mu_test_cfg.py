import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process("CSCTIMING")

process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# Express for 711 MWGR July 2014
#process.GlobalTag.globaltag = 'GR_E_V38::All'
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v4'
#process.GlobalTag.globaltag = '106X_dataRun2_v26'

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'csc_singleMu_test'
fileExtension = '.root'
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                'root://eoscms.cern.ch://eos/cms/tier0//store/data/Run2022C/Cosmics/AOD/PromptReco-v1/000/356/535/00000/e7219d5f-e9e1-46c9-8a9e-2d7b06538ea4.root')
)

#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring(
#                                'file:/eos/cms/store/data/Run2018D/Charmonium/AOD/12Nov2019_UL2018-v1/110002/366041D9-52D4-524D-B407-212037FCDD67.root')
#)

process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(True)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName+fileExtension
# outfileName = 'test.root'
#print outfileName

# From RAW
process.p = cms.Path(process.cscTimingBabyMaker)

#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string(outfileName),
#                               outputCommands = cms.untracked.vstring('drop *',
#                                                                      'keep *_*_*_CSCTIMING'))

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))


process.e = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
