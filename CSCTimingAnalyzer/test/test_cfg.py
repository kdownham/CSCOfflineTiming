import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

applyGoodRunList = cms.bool(False)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                # '/store/user/fgolf/csc/timing/offline/SingleMu/v3/150630_025216/0000/csc_singleMu_zeroT_1.root',
                                # '/store/user/sicheng/csctiming/2017/SingleMuon/v4/170811_062223/0000/test_35.root',
                                # '/store/user/sicheng/csctiming/2017/SingleMuon/2017C_v1/171001_231716/0000/test_26.root',
                                # '/store/user/sicheng/csctiming/2017/SingleMuon/2017C_v1/171001_231716/0000/test_14.root',
                                # 'file:/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingBabyMaker/test/test_9000.root',
                                'file:/home/users/sicheng/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingBabyMaker/test/test_mcdbv3.root',
                                # '/store/user/sicheng/csctiming/MonteCarlo/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/DYJetsToLL/180312_031243/0000/test_DYjets_92X_196.root',
                                )
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    JSONfile = '/home/users/sicheng/working/CSCTiming/CMSSW_9_3_1/src/CSCOfflineTiming/CSCTimingAnalyzer/test/Cert_294927-302654_13TeV_PromptReco_Collisions17_JSON.txt'
    # JSONfile = '/home/users/sicheng/working/CSCTiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/test/Cert_297292-297723_13TeV_2017_HCAL_DCS_GOOD.txt'
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
    process.source.lumisToProcess.extend(myLumis)

process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscTimingAnalyzer_cfi")
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(False)
process.cscTimingAnalyzer.makePlotsPerChamber = cms.untracked.bool(True)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(False)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyUpdatedME11corrections = cms.untracked.bool(False)
process.cscTimingAnalyzer.min_pt = cms.untracked.double(5)
process.cscTimingAnalyzer.max_dz = cms.untracked.double(999)
process.cscTimingAnalyzer.printTimeCorrectionParametersToFile = cms.untracked.bool(False)
process.cscTimingAnalyzer.writeTimeCorrectionParametersToTree = cms.untracked.bool(False)
process.cscTimingAnalyzer.checkCSCstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.checkDCSstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeHeuristicCorrection = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByRing = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber = cms.untracked.bool(False)
process.cscTimingAnalyzer.makeNminus1hists = cms.untracked.bool(False)
process.cscTimingAnalyzer.writeTimingStudyBabyTree = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeMatchedStationsRequirement = cms.untracked.bool(False);

applyUpdatedME11correctionsName = '_updateME11corrections'
checkCSCstatusName = '_checkCSCstatus'
removeHeuristicCorrectionName = '_removeHeuristicCorr'
# min_pt_name = '_minPt' + str(process.cscTimingAnalyzer.min_pt)
# max_dz_name = '_maxDz' + str(process.cscTimingAnalyzer.max_dz)

baseFileName = 'histos_mcdbv3'
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
