import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

applyGoodRunList = cms.bool(True)
#applyGoodRunList = cms.bool(False)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(MAXEVENTS_REPLACETAG) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
				'file:BABYOUTPUT_REPLACETAG',
                                )
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    #JSONfile = 'Cert_Collisions2022_355100_362760_Muon.json'
    JSONfile = 'Cert_Collisions2023_366442_370790_Golden.json'
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
    process.source.lumisToProcess.extend(myLumis)

process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscTimingAnalyzer_cfi")
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(False)
process.cscTimingAnalyzer.makePlotsPerChamber = cms.untracked.bool(True)
#process.cscTimingAnalyzer.verbose = cms.untracked.bool(False)
#process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(False)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyUpdatedME11corrections = cms.untracked.bool(False)
process.cscTimingAnalyzer.min_pt = cms.untracked.double(5)
process.cscTimingAnalyzer.max_dz = cms.untracked.double(999)
process.cscTimingAnalyzer.printTimeCorrectionParametersToFile = cms.untracked.bool(True)
process.cscTimingAnalyzer.writeTimeCorrectionParametersToTree = cms.untracked.bool(True)
process.cscTimingAnalyzer.checkCSCstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.checkDCSstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeHeuristicCorrection = cms.untracked.bool(REMOVEHEURISTIC_REPLACETAG) #True when deriving new corrections, false when making new plots
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByRing = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber = cms.untracked.bool(False)
process.cscTimingAnalyzer.fpForHeuristicCorrByChamber = cms.FileInPath('HEURISTICCORR_REPLACETAG')
process.cscTimingAnalyzer.makeNminus1hists = cms.untracked.bool(True)
process.cscTimingAnalyzer.writeTimingStudyBabyTree = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeMatchedStationsRequirement = cms.untracked.bool(False);

applyUpdatedME11correctionsName = '_updateME11corrections'
checkCSCstatusName = '_checkCSCstatus'
removeHeuristicCorrectionName = '_removeHeuristicCorr'

baseFileName = 'ANALYZEROUTPUT_REPLACETAG'
fileExtension = '.root'
outfileName = baseFileName

if process.cscTimingAnalyzer.applyUpdatedME11corrections:
    outfileName = outfileName + applyUpdatedME11correctionsName
if process.cscTimingAnalyzer.checkCSCstatus:
    outfileName = outfileName + checkCSCstatusName
if applyGoodRunList:
    outfileName = outfileName + '_applyGoodRunList'
if process.cscTimingAnalyzer.removeHeuristicCorrection:
    outfileName = outfileName + removeHeuristicCorrectionName
if process.cscTimingAnalyzer.applyNewHeuristicCorrectionByRing:
    outfileName = outfileName + '_newHeuristicCorrByRing'
if process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber:
    outfileName = outfileName + '_newHeuristicCorrByChamber'
if process.cscTimingAnalyzer.removeMatchedStationsRequirement:
    outfileName = outfileName + '_removeMatchedStationsRequirement'

outfileName = outfileName + fileExtension
print (outfileName)

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName) )

process.p = cms.Path(process.cscTimingAnalyzer)
