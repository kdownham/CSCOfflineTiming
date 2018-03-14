import FWCore.ParameterSet.Config as cms

process = cms.Process("CSC")

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# Express for 711 MWGR July 2014
process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v8'

process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                'file:/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/test/step3_mcdb_bxshift2.root'
                                # '/store/data/Run2017F/SingleMuon/AOD/17Nov2017-v1/50000/A27C5ABB-F3DE-E711-B8ED-008CFAE45364.root'
                                # 'none.root'
                            )
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscMakeTimingCorrectionsForDB_cfi")

process.cscMakeTimingCorrectionsForDB = cms.EDAnalyzer(
    'CSCMakeTimingCorrectionsForDB',
    # Use output from determineHeuristicCorrections.C
    heuristicCorrFileName = cms.untracked.string('/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber.txt'),
    cableLengthFileName = cms.untracked.string('/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/data/cable_lengths_by_chamber_v2.txt'),
    outFileName = cms.untracked.string('/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber_unsorted.dat'),
    outChipFileName = cms.untracked.string('/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/data/chipSpeed_corrections_for_db.txt'),
    readBadChannels = cms.bool(True),
    readBadChambers = cms.bool(True),
    CSCUseTimingCorrections = cms.bool(True),
    CSCUseGasGainCorrections = cms.bool(True),
    verbose = cms.bool(False),
    updateOnlyNewChambers = cms.untracked.bool(False),
)

process.p = cms.Path(process.cscMakeTimingCorrectionsForDB)
