import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

applyGoodRunList = cms.bool(True)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_1.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_10.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_11.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_12.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_13.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_2.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_3.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_4.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_5.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_6.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_7.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_8.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_9.root')
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    # JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/Cert_246908-248038_13TeV_PromptReco_Collisions15_ZeroTesla_JSON.txt'
    JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/Cert_246908-248038_13TeV_PromptReco_Collisions15_ZeroTesla_JSON_MuonPhys.txt'
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
    process.source.lumisToProcess.extend(myLumis)

process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscTimingAnalyzer_cfi")
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(False)
process.cscTimingAnalyzer.makePlotsPerChamber = cms.untracked.bool(True)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(False)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyUpdatedME11corrections = cms.untracked.bool(True)
process.cscTimingAnalyzer.min_pt = cms.untracked.double(0)
process.cscTimingAnalyzer.max_dz = cms.untracked.double(999)
process.cscTimingAnalyzer.printTimeCorrectionParametersToFile = cms.untracked.bool(False)
process.cscTimingAnalyzer.writeTimeCorrectionParametersToTree = cms.untracked.bool(False)
process.cscTimingAnalyzer.checkCSCstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.checkDCSstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeHeuristicCorrection = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByRing = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber = cms.untracked.bool(True)
process.cscTimingAnalyzer.makeNminus1hists = cms.untracked.bool(True)
process.cscTimingAnalyzer.writeTimingStudyBabyTree = cms.untracked.bool(True)
process.cscTimingAnalyzer.removeMatchedStationsRequirement = cms.untracked.bool(True);
process.cscTimingAnalyzer.fileForHeuristicCorrByRing = cms.string('/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_ring_segmentMatched.txt')
process.cscTimingAnalyzer.fileForHeuristicCorrByChamber = cms.string('/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/data/timing_corrections_by_chamber_segmentMatched.txt')

applyUpdatedME11correctionsName = '_updateME11corrections'
checkCSCstatusName = '_checkCSCstatus'
removeHeuristicCorrectionName = '_removeHeuristicCorr'
# min_pt_name = '_minPt' + str(process.cscTimingAnalyzer.min_pt)
# max_dz_name = '_maxDz' + str(process.cscTimingAnalyzer.max_dz)

baseFileName = 'histos'
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
print outfileName

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName) )

process.p = cms.Path(process.cscTimingAnalyzer)
