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
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(True)
process.cscTimingAnalyzer.makePlotsPerChamber = cms.bool(True)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(True)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyUpdatedME11corrections = cms.bool(True)
process.cscTimingAnalyzer.min_pt = cms.double(0)
process.cscTimingAnalyzer.max_dz = cms.double(1)

applyUpdatedME11correctionsName = 'updateME11corrections'
min_pt_name = "minPt%d" & process.cscTimingAnalyzer.min_pt
max_dz_name = "maxDz%d" & process.cscTimingAnalyzer.max_dz

baseFileName = 'histos'
fileExtension = '.root'
outfileName = baseFileName

if process.cscTimingAnalyzer.applyUpdatedME11corrections:
    outfileName = outfileName + applyUpdatedME11correctionsName
if process.cscTimingAnalyzer.min_pt > 1:
    outfileName = outfileName + min_pt_name
if process.cscTimingAnalyzer.max_dz != 0.5:
    otufileName = outfileName + max_dz_name

outfileName = outfileName + fileExtension

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.p = cms.Path(process.cscTimingAnalyzer)
