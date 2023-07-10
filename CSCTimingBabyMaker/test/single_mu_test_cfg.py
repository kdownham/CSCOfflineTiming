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
baseFileName = '/eos/user/k/kdownham/CSCOfflineTiming/test_condor/csc_test_Run357900'
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
				'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/019f963f-7fa4-4019-b41c-b1382f4f948a.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/03c69590-7ab5-4311-b7f5-714fee4d2c02.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/0498c301-c267-45f6-ba01-a1977498bc3e.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/05fe1620-c20d-4df3-af0a-806ef1bcaff9.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/06d050bb-1663-47c9-822f-54bb128074af.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/07dcd66f-2f11-4518-86e0-519cf3555276.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/086420ee-e52f-454c-89cb-f8e7d343dccb.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/666a56c6-005d-4ab7-b6e6-aa168ea0c855.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/e220fa12-beb1-4cde-94f1-ec05d537a186.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Validation/RAW2DIGI_RECO/test_anode_and_cathode_RAW2DIGI_RECO_Run357700.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/05d8df7a-21cf-4091-90c3-9a27dd81794c.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/092c704b-40c9-4b88-9589-4232b44b8f47.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/429e1184-a419-4a8d-9381-35d248c054c2.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/4723a1d9-1d13-4e06-b227-6cc44dd3ed25.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022E/Muon/AOD/PromptReco-v1/000/359/812/00000/53d1aff3-b649-4d47-a1fb-c14cb96015a2.root',
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


# Comment this block out if you don't want to read from the local conditions data
#process.CSCTimingSource = cms.ESSource("PoolDBESSource",
#                                      loadAll = cms.bool(True),
#                                      timetype = cms.string('runnumber'),
#                                      toGet = cms.VPSet(cms.PSet(
#                                                                 record = cms.string('CSCChamberTimeCorrectionsRcd'),
#                                                                 tag = cms.string('CSCChamberTimeCorrections')
#                                                                 )),
#                                      connect=cms.string("sqlite_file:conditionsData/NewAnodeOffsets_357900_goodCathodes.db"),
#                                      DBParameters = cms.PSet(
#                                                              authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb'),
#                                                              authenticationMethod = cms.untracked.uint32(1)
#                                                              )
#                                      )
#process.es_prefer_cscTiming = cms.ESPrefer("PoolDBESSource","CSCTimingSource")

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
