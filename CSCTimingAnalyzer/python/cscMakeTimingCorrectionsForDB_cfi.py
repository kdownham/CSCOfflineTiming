import FWCore.ParameterSet.Config as cms

cscMakeTimingCorrectionsForDB = cms.EDAnalyzer('CSCMakeTimingCorrectionsForDB',
                                               heuristicCorrFileName = cms.untracked.string('~/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/heuristic_corrections/heuristicCorrections_byChamber_357900.txt'),
                                               cableLengthFileName = cms.untracked.string('~/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/cable_lengths_by_chamber_v2.txt'),
                                               outFileName = cms.untracked.string('~/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber_Sep29.dat'),
                                               outChipFileName = cms.untracked.string('~/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections/chipSpeed_corrections_for_db_2022.txt'),
                                               readBadChannels = cms.bool(True),
                                               readBadChambers = cms.bool(True),
                                               CSCUseTimingCorrections = cms.bool(True),
                                               CSCUseGasGainCorrections = cms.bool(True),
                                               verbose = cms.bool(False),
                                               updateOnlyNewChambers = cms.untracked.bool(False),
)
