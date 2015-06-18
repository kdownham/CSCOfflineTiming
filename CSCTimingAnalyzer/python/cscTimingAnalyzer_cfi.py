import FWCore.ParameterSet.Config as cms

cscTimingAnalyzer = cms.EDAnalyzer('CSCTimingAnalyzer',
                                   makeChamberTimingCorrectionValueHists = cms.untracked.bool(False),
                                   makePlotsPerChamber = cms.untracked.bool(False),
                                   makePlotsPerLayer = cms.untracked.bool(False),
                                   printTimeCorrectionParametersToFile = cms.untracked.bool(False),
                                   writeTimeCorrectionParametersToTree = cms.untracked.bool(False),
                                   verbose = cms.untracked.bool(False),
                                   debug = cms.untracked.bool(False),
                                   outfileName = cms.untracked.string('histos.root'),
                                   timeParamFileName = cms.untracked.string('cscTimingParameters.root')
)
