import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
print 'process'
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
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag.globaltag = '102X_dataRun2_Prompt_v1'
# process.GlobalTag.globaltag = '102X_dataRun2_Sep2018Rereco_v1'
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = "112X_dataRun2_v7"


# Config CSC for postls1
# process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
# process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
# process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

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
                                # 'file:/home/users/sicheng/working/CSCTiming/testfile/SingleMuon_296968.root',
                                # '/store/data/Run2017B/SingleMuon/AOD/PromptReco-v1/000/297/296/00000/F0DF8991-2259-E711-8ED0-02163E019C8F.root',
                                # '/store/data/Run2017B/SingleMuon/AOD/PromptReco-v1/000/297/292/00000/F2EFD821-2259-E711-9324-02163E011810.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/C84C0657-BD67-9C4F-8706-35862F536F89.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/C8FAD63A-8098-BE44-BDE4-1994868A6050.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/7BF72F9B-C5CA-F64B-9E3B-3966EBE7D486.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/25A89671-8774-BA46-BD03-7FFB21218EF7.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/EFEA0E80-5CD6-8244-87C2-2D593E143C59.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/A7CACFC2-752D-174D-8AB4-A8819272087E.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/C6FDD8FB-BB1B-D042-9B6B-8F9270A673FC.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/50620513-D858-DA40-B143-8F97EFB2D8C4.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/8030DE1B-1EBE-594E-9D6D-A021C4453C65.root',
                                '/store/data/Run2018D/SingleMuon/AOD/PromptReco-v2/000/324/201/00000/CA21A7A4-689E-BD42-A0D5-81A49D179124.root',

                            )
)

process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(False)

# automatically name the output file correctly and uniquely
outfileName = 'test_mcdbv3_test.root'
baseFileName = outfileName.split('.')[0]
outfileName = baseFileName
if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName + '.root'
print 'Output to file: ', outfileName

# From RAW
process.p = cms.Path(process.cscTimingBabyMaker)
print '1'
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))
print '2'
process.e = cms.EndPath(process.out)
print '3'
# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
print '4'
