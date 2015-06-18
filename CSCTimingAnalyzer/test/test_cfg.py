import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                'file:/home/users/fgolf/csc/CMSSW_7_4_5/src/CSCOfflineTiming/CSCTimingBabyMaker/test/test.root'
                            )
)

process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscTimingAnalyzer_cfi")
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(False)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(True)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)

baseFileName = 'histos'
fileExtension = '.root'
outfileName = baseFileName

outfileName = outfileName+fileExtension

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.p = cms.Path(process.cscTimingAnalyzer)
