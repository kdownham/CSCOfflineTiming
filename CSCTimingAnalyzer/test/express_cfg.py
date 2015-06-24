import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

applyGoodRunList = cms.bool(False)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_1.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_10.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_11.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_13.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_14.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_15.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_16.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_17.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_18.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_19.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_2.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_20.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_21.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_22.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_23.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_24.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_25.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_26.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_27.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_28.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_29.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_3.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_30.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_31.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_32.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_33.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_34.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_35.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_36.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_37.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_38.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_39.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_4.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_40.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_41.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_42.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_43.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_44.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_45.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_46.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_47.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_48.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_49.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_5.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_50.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_51.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_52.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_53.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_54.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_55.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_56.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_57.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_58.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_59.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_6.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_60.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_61.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_62.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_63.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_64.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_65.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_7.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_8.root',
                                '/store/user/fgolf/csc/timing/offline/ExpressPhysics/v1/150622_185949/0000/csc_singleMu_zeroT_9.root')
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    #JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/Cert_246908-247381_13TeV_PromptReco_Collisions15_ZeroTesla_JSON.txt'
    #JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/Cert_246908-247381_13TeV_PromptReco_Collisions15_ZeroTesla_JSON_MuonPhys.txt'
    JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/express_csc.txt'
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
process.cscTimingAnalyzer.checkCSCstatus = cms.untracked.bool(True)
process.cscTimingAnalyzer.checkDCSstatus = cms.untracked.bool(False)
process.cscTimingAnalyzer.removeHeuristicCorrection = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByRing = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyNewHeuristicCorrectionByChamber = cms.untracked.bool(True)
process.cscTimingAnalyzer.makeNminus1hists = cms.untracked.bool(True)

applyUpdatedME11correctionsName = '_updateME11corrections'
checkCSCstatusName = '_checkCSCstatus'
removeHeuristicCorrectionName = '_removeHeuristicCorr'
# min_pt_name = '_minPt' + str(process.cscTimingAnalyzer.min_pt)
# max_dz_name = '_maxDz' + str(process.cscTimingAnalyzer.max_dz)

baseFileName = 'histos_express'
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
# if process.cscTimingAnalyzer.min_pt > 1:
#     outfileName = outfileName + min_pt_name
# if process.cscTimingAnalyzer.max_dz > 0.5:
#     otufileName = outfileName + max_dz_name

outfileName = outfileName + fileExtension
print outfileName

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.p = cms.Path(process.cscTimingAnalyzer)
