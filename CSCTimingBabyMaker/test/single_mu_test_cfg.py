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
baseFileName = 'output/csc_testConstants_Run359812_2022E.root'
fileExtension = '.root'
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/086420ee-e52f-454c-89cb-f8e7d343dccb.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/666a56c6-005d-4ab7-b6e6-aa168ea0c855.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Validation/RAW2DIGI_RECO/test_RAW2DIGI_RECO_Run357767.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/05d8df7a-21cf-4091-90c3-9a27dd81794c.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/092c704b-40c9-4b88-9589-4232b44b8f47.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/429e1184-a419-4a8d-9381-35d248c054c2.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/4723a1d9-1d13-4e06-b227-6cc44dd3ed25.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/53d1aff3-b649-4d47-a1fb-c14cb96015a2.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/ac44dd8f-c9f6-44d4-b4ed-7137f82c5111.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/b7a40ab3-ac10-4301-81e4-13dc5ca3e265.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/c250e29a-5d75-49c8-92e8-11f84e76b003.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/c8556af9-44af-4c79-b35e-f1200d7486bc.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/c9d1db0f-8733-4ebe-9d89-3af1c5daaf40.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/cc524031-7033-46cd-ab97-f16b79f713e3.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/db3b8c54-22c2-48c3-95fb-516aa6407cc9.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/e887bef7-f7da-48ab-9c63-d74ddc8efe77.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/f00718d1-114c-40f3-928c-92d7a11d3d7d.root',
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
