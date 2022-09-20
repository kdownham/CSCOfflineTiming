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

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v4'
#process.GlobalTag.globaltag = '106X_dataRun2_v26'

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'output/merged/csc_Run357900_premerged'
fileExtension = '.root'
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 10


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
				#'file:/eos/cms/tier0/store/data/Run2022C/Muon/AOD/PromptReco-v1/000/357/441/00000/025f379d-da24-47dd-a6e5-2e442df43c26.root')
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/019f963f-7fa4-4019-b41c-b1382f4f948a.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/03c69590-7ab5-4311-b7f5-714fee4d2c02.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/0498c301-c267-45f6-ba01-a1977498bc3e.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/05fe1620-c20d-4df3-af0a-806ef1bcaff9.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/06d050bb-1663-47c9-822f-54bb128074af.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/07dcd66f-2f11-4518-86e0-519cf3555276.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/086420ee-e52f-454c-89cb-f8e7d343dccb.root',
				'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/666a56c6-005d-4ab7-b6e6-aa168ea0c855.root',
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
