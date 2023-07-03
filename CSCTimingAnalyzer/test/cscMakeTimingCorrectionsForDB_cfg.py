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

process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

#process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/019f963f-7fa4-4019-b41c-b1382f4f948a.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/03c69590-7ab5-4311-b7f5-714fee4d2c02.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/0498c301-c267-45f6-ba01-a1977498bc3e.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/05fe1620-c20d-4df3-af0a-806ef1bcaff9.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/06d050bb-1663-47c9-822f-54bb128074af.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/07dcd66f-2f11-4518-86e0-519cf3555276.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/086420ee-e52f-454c-89cb-f8e7d343dccb.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/666a56c6-005d-4ab7-b6e6-aa168ea0c855.root',
                                # '/store/data/Run2017F/SingleMuon/AOD/17Nov2017-v1/50000/A27C5ABB-F3DE-E711-B8ED-008CFAE45364.root'
                                # 'none.root'
                            )
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscMakeTimingCorrectionsForDB_cfi")

process.cscMakeTimingCorrectionsForDB = cms.EDAnalyzer(
    'CSCMakeTimingCorrectionsForDB',
    # Use output from determineHeuristicCorrections.C
    heuristicCorrFileName = cms.untracked.string('/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/heuristic_corrections/heuristicCorrections_byChamber_357900.txt'),
    cableLengthFileName = cms.untracked.string('/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/cable_lengths_by_chamber_v2.txt'),
    outFileName = cms.untracked.string('../data/newAnodeCorrections_Run357900.dat'),
    outChipFileName = cms.untracked.string('../data/timing_corrections/chipSpeed_corrections_for_db_2022.txt'),
    readBadChannels = cms.bool(True),
    readBadChambers = cms.bool(True),
    CSCUseTimingCorrections = cms.bool(True),
    CSCUseGasGainCorrections = cms.bool(True),
    verbose = cms.bool(False),
    updateOnlyNewChambers = cms.untracked.bool(False),
)

process.p = cms.Path(process.cscMakeTimingCorrectionsForDB)
