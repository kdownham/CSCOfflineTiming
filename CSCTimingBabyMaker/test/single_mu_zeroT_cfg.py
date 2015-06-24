import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMING")

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# Express for 711 MWGR July 2014
process.GlobalTag.globaltag = 'GR_E_V38::All'

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'csc_singleMu_zeroT'
fileExtension = '.root'
outfileName = baseFileName

outfileName = outfileName+fileExtension
outfileName = 'test.root'


#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 100


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                # '/store/data/Run2015A/MinimumBias/RECO/PromptReco-v1/000/246/970/00000/00685C66-000C-E511-8B6B-02163E0142D7.root')
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/FE6E0461-D10B-E511-A8D0-02163E014428.root')
)

process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(True)

# From RAW
process.p = cms.Path(process.cscTimingBabyMaker)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))

process.e = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
