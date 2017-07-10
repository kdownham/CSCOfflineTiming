import FWCore.ParameterSet.Config as cms

cscMakeTimingCorrectionsForDB = cms.EDAnalyzer('CSCMakeTimingCorrectionsForDB',
                                               heuristicCorrFileName = cms.untracked.string('/home/users/sicheng/working/CSCtiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber_combinedME11ab_2017.txt'),
                                               cableLengthFileName = cms.untracked.string('/home/users/sicheng/working/CSCtiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/data/cable_lengths_by_chamber_v2.txt'),
                                               outFileName = cms.untracked.string('/home/users/sicheng/working/CSCtiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber_Jul9.dat'),
                                               outChipFileName = cms.untracked.string('/home/users/sicheng/working/CSCtiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/data/chipSpeed_corrections_for_db_Jul9_test.txt'),
                                               readBadChannels = cms.bool(True),
                                               readBadChambers = cms.bool(True),
                                               CSCUseTimingCorrections = cms.bool(True),
                                               CSCUseGasGainCorrections = cms.bool(True),
                                               verbose = cms.bool(False),
                                               updateOnlyNewChambers = cms.untracked.bool(False),
)
