import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process("CSCTIMING",eras.Run2_2017)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Data Run 2017
process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v4'

# Config CSC for postls1
# process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
# process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
# process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'csc_singleMu'
fileExtension = '.root'
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.suppressError = cms.untracked.vstring('HLTConfigProvider','cscTimingBabyMaker')

# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                # 'file:/home/users/fgolf/csc/CMSSW_8_0_17/src/test_new.root'
                                # 'file:/home/users/sicheng/working/CSCtiming/testfile/SingleMuon_296968.root',
                                '/store/data/Run2017B/SingleMuon/AOD/PromptReco-v1/000/297/296/00000/F0DF8991-2259-E711-8ED0-02163E019C8F.root',
                                '/store/data/Run2017B/SingleMuon/AOD/PromptReco-v1/000/297/292/00000/F2EFD821-2259-E711-9324-02163E011810.root',
                            )
)

process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(False)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName+fileExtension
outfileName = 'test.root'
print outfileName

# From RAW
process.p = cms.Path(process.cscTimingBabyMaker)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))

process.e = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
