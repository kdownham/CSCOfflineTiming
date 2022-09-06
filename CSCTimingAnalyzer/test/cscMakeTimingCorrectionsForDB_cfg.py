import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process("CSC")

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# 
process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v4'

process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                'file:/eos/cms/tier0/store/data/Run2022C/Muon/AOD/PromptReco-v1/000/357/441/00000/025f379d-da24-47dd-a6e5-2e442df43c26.root'
                                # '/store/data/Run2017F/SingleMuon/AOD/17Nov2017-v1/50000/A27C5ABB-F3DE-E711-B8ED-008CFAE45364.root'
                                # 'none.root'
                            )
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscMakeTimingCorrectionsForDB_cfi")

process.cscMakeTimingCorrectionsForDB = cms.EDAnalyzer(
    'CSCMakeTimingCorrectionsForDB',
    # Use output from determineHeuristicCorrections.C
    heuristicCorrFileName = cms.untracked.string('/home/users/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/heuristicCorrections.txt'),
    cableLengthFileName = cms.untracked.string('/home/users/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/cable_lengths_by_chamber_v2.txt'),
    outFileName = cms.untracked.string('/home/users/kdownham/CSCValidation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber_unsorted_Run3.dat'),
    outChipFileName = cms.untracked.string('/home/users/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/chipSpeed_corrections_for_db_Run3.txt'),
    readBadChannels = cms.bool(True),
    readBadChambers = cms.bool(True),
    CSCUseTimingCorrections = cms.bool(True),
    CSCUseGasGainCorrections = cms.bool(True),
    verbose = cms.bool(False),
    updateOnlyNewChambers = cms.untracked.bool(False),
)

process.p = cms.Path(process.cscMakeTimingCorrectionsForDB)
