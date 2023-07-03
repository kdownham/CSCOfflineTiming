import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

#applyGoodRunList = cms.bool(True)
applyGoodRunList = cms.bool(False)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                #'file:/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingBabyMaker/test/output/merged/csc_Run357900_premerged_useMuonSegmentMatcher.root',
				'file:/afs/cern.ch/user/k/kdownham/CSC_Validation/CMSSW_12_4_6/src/CSCOfflineTiming/CSCTimingBabyMaker/test/output/validation/csc_3_22_23_Run357900_useMuonSegmentMatcher.root',
                                )
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    JSONfile = '/home/users/sicheng/working/CSCTiming/CMSSW_9_3_1/src/CSCOfflineTiming/CSCTimingAnalyzer/test/Cert_294927-302654_13TeV_PromptReco_Collisions17_JSON.txt'
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
#process.cscTimingAnalyzer.printTimeCorrectionParametersToFile = cms.untracked.bool(False)
process.cscTimingAnalyzer.printTimeCorrectionParametersToFile = cms.untracked.bool(True)
process.cscTimingAnalyzer.writeTimeCorrectionParametersToTree = cms.untracked.bool(True)
process.cscTimingAnalyzer.checkCSCstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.checkDCSstatus = cms.untracked.bool(False)
#process.cscTimingAnalyzer.removeHeuristicCorrection = cms.untracked.bool(True)    # Remove when deriving new corrections
process.cscTimingAnalyzer.removeHeuristicCorrection = cms.untracked.bool(False)  # Keep when making plots
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByRing = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber = cms.untracked.bool(False)
#process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber = cms.untracked.bool(True)
#process.cscTimingAnalyzer.fpForHeuristicCorrByChamber = cms.FileInPath('CSCOfflineTiming/CSCTimingAnalyzer/data/heuristic_corrections/heuristicCorrections_byChamber_357900.txt') #correct file for hcs
process.cscTimingAnalyzer.fpForHeuristicCorrByChamber = cms.FileInPath('CSCOfflineTiming/CSCTimingAnalyzer/data/heuristic_corrections/heuristicCorrections_byChamber_357900.txt')
process.cscTimingAnalyzer.makeNminus1hists = cms.untracked.bool(True)
process.cscTimingAnalyzer.writeTimingStudyBabyTree = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeMatchedStationsRequirement = cms.untracked.bool(False);

applyUpdatedME11correctionsName = '_updateME11corrections'
checkCSCstatusName = '_checkCSCstatus'
removeHeuristicCorrectionName = '_removeHeuristicCorr'
# min_pt_name = '_minPt' + str(process.cscTimingAnalyzer.min_pt)
# max_dz_name = '_maxDz' + str(process.cscTimingAnalyzer.max_dz)

baseFileName = 'output/Validation/test_anodeCorrections_Run357900'
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
# if process.cscTimingAnalyzer.min_pt > 1:
#     outfileName = outfileName + min_pt_name
# if process.cscTimingAnalyzer.max_dz > 0.5:
#     otufileName = outfileName + max_dz_name

outfileName = outfileName + fileExtension
print (outfileName)

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName) )

process.p = cms.Path(process.cscTimingAnalyzer)
