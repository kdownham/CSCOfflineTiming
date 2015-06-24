import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMINGANALYSIS")

import FWCore.PythonUtilities.LumiList as LumiList
# import FWCore.ParameterSet.Types as CfgTypes

myLumis = LumiList.LumiList(runs = [234664, 234669, 234670, 234671, 234673, 234677, 234679])
print myLumis
myLumis.getCompactList()
myLumis.writeJSON(fileName='/home/users/fgolf/csc/CMSSW_7_4_5_timing/src/CSCOfflineTiming/CSCTimingAnalyzer/json/express_csc.txt')
