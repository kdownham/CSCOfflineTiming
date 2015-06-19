import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

applyGoodRunList = cms.bool(False)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                # 'file:/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingBabyMaker/test/test.root'
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_1.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_2.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_3.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_4.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_5.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_6.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_7.root',
                                '/store/user/fgolf/csc/timing/offline/SingleMu/v1/150619_013559/0000/csc_singleMu_zeroT_8.root')
)

# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/Cert_246908-247381_13TeV_PromptReco_Collisions15_ZeroTesla_JSON.txt'
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
    process.source.lumisToProcess.extend(myLumis)

process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscTimingAnalyzer_cfi")
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(True)
process.cscTimingAnalyzer.makePlotsPerChamber = cms.untracked.bool(True)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(False)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyUpdatedME11corrections = cms.untracked.bool(True)
process.cscTimingAnalyzer.min_pt = cms.untracked.double(0)
process.cscTimingAnalyzer.max_dz = cms.untracked.double(999)
process.cscTimingAnalyzer.printTimeCorrectionParametersToFile = cms.untracked.bool(True)
process.cscTimingAnalyzer.writeTimeCorrectionParametersToTree = cms.untracked.bool(True)
process.cscTimingAnalyzer.checkCSCstatus = cms.untracked.bool(True)
process.cscTimingAnalyzer.checkDCSstatus = cms.untracked.bool(False)

applyUpdatedME11correctionsName = '_updateME11corrections'
# min_pt_name = '_minPt' + str(process.cscTimingAnalyzer.min_pt)
# max_dz_name = '_maxDz' + str(process.cscTimingAnalyzer.max_dz)

baseFileName = 'histos'
fileExtension = '.root'
outfileName = baseFileName

if process.cscTimingAnalyzer.applyUpdatedME11corrections:
    outfileName = outfileName + applyUpdatedME11correctionsName
# if process.cscTimingAnalyzer.min_pt > 1:
#     outfileName = outfileName + min_pt_name
# if process.cscTimingAnalyzer.max_dz > 0.5:
#     otufileName = outfileName + max_dz_name

outfileName = outfileName + fileExtension
# print outfileName

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.p = cms.Path(process.cscTimingAnalyzer)
