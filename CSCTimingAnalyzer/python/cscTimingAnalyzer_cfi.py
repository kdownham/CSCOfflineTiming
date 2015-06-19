import FWCore.ParameterSet.Config as cms

cscTimingAnalyzer = cms.EDAnalyzer('CSCTimingAnalyzer',
                                   makeChamberTimingCorrectionValueHists = cms.untracked.bool(False),
                                   makePlotsPerChamber = cms.untracked.bool(False),
                                   makePlotsPerLayer = cms.untracked.bool(False),
                                   printTimeCorrectionParametersToFile = cms.untracked.bool(False),
                                   writeTimeCorrectionParametersToTree = cms.untracked.bool(False),
                                   verbose = cms.untracked.bool(False),
                                   debug = cms.untracked.bool(False),
                                   applyUpdatedME11corrections = cms.untracked.bool(False),
                                   checkCSCstatus = cms.untracked.bool(False),
                                   checkDCSstatus = cms.untracked.bool(False),
                                   min_pt = cms.untracked.double(10.),
                                   max_dz = cms.untracked.double(0.5),
                                   outfileName = cms.untracked.string('histos.root'),
                                   timeParamFileName = cms.untracked.string('cscTimingParameters.root')
)
