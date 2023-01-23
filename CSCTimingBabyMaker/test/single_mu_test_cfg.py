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
#process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v10'
process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v4'
#process.GlobalTag.globaltag = '106X_dataRun2_v26'

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'output/csc_testAnodeConstants_Run357700_2022D_v2.root'
fileExtension = '.root'
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 100


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/086420ee-e52f-454c-89cb-f8e7d343dccb.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/666a56c6-005d-4ab7-b6e6-aa168ea0c855.root',
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Validation/RAW2DIGI_RECO/test_anode_RAW2DIGI_RECO_Run357700.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/42d423b1-5421-4853-aa4d-e01db6ccaecd.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/451f0188-05dd-4027-a718-82887e50eb14.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/4db79eda-6458-4d48-8674-1ead208db0c4.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/76f3136c-2c7f-453b-bedc-acd023887b12.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/80001d2c-5d41-4f5d-b3de-eddfda6164e2.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/9db95a69-1832-40f3-afb1-7bbd690a73f3.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/9dd31e92-618f-44c2-af6c-b351f593f6a2.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/b0bd1c12-8425-4fa8-a9eb-b21fbf687a79.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/e220fa12-beb1-4cde-94f1-ec05d537a186.root',
			)
)



process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)  # nominally set to false
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
