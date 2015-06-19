import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

applyGoodRunList = cms.bool(True)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
                                'file:/home/users/fgolf/csc/CMSSW_7_4_5/src/CSCOfflineTiming/CSCTimingBabyMaker/test/test.root'
                            )
)

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

if applyGoodRunList:
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    JSONfile = '/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/Cert_246908-247381_13TeV_PromptReco_Collisions15_ZeroTesla_JSON.txt'
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
    process.source.lumisToProcess.extend(myLumis)

process.load("CSCOfflineTiming.CSCTimingAnalyzer.cscTimingAnalyzer_cfi")
process.cscTimingAnalyzer.makeChamberTimingCorrectionValueHists = cms.untracked.bool(True)
process.cscTimingAnalyzer.makePlotsPerChamber = cms.bool(True)
process.cscTimingAnalyzer.verbose = cms.untracked.bool(False)
process.cscTimingAnalyzer.debug = cms.untracked.bool(False)
process.cscTimingAnalyzer.applyUpdatedME11corrections = cms.bool(True)
process.cscTimingAnalyzer.min_pt = cms.double(0)
process.cscTimingAnalyzer.max_dz = cms.double(1)

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
print outfileName

process.cscTimingAnalyzer.outfileName = cms.untracked.string(outfileName)

process.p = cms.Path(process.cscTimingAnalyzer)
