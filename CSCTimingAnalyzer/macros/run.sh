#!/bin/bash
mkdir -p plots
alias rot='root -l -b -q'

# void determineHeuristicCorrections (std::string fname, bool byStation, bool byChamber, bool combineME11ab = false, std::string ofname = "")
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root",false,true,true,"cscHeurCorr_by_chamber_Jul5.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root",true,false,true,"cscHeurCorr_by_ring_Jul5.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_removeHeuristicCorr.root",true,false,true,"cscHeurCorr_by_ring_Jul5.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_removeHeuristicCorr.root",false,true,true,"cscHeurCorr_by_chamber_2016H.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/rootfiles/histos_agr_rhc_Sep29.root",false,true,true,"cscHeurCorr_by_chamber_Sep29.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/rootfiles/histos_agr_rhc_Oct3.root",false,true,true,"cscHeurCorr_by_chamber_Oct3.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_92x_removeHeuristicCorr.root",false,true,true,"cscHeurCorr_by_chamber_92X.txt")'

# rot determineHeuristicCorrections.C'("~/working/CSCTiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_applyGoodRunList_removeHeuristicCorr_297292.root",true,false,true,"cscHeurCorr_by_ring_Jul6.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/CMSSW_9_2_4/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_applyGoodRunList_removeHeuristicCorr_297292.root",false,true,true,"cscHeurCorr_by_chamber_Jul6.txt")'

# rot determineHeuristicCorrections.C'("~/working/CSCTiming/rootfiles/histos_applyGoodRunList_removeHeuristicCorr.root",false,true,true,"cscHeurCorr_by_chamber_Jul8.txt")'
# rot determineHeuristicCorrections.C'("~/working/CSCTiming/rootfiles/histos_applyGoodRunList_removeHeuristicCorr.root",true,false,true,"cscHeurCorr_by_ring_Jul8.txt")'

# rot makeMuonTimingPlot.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root")'
# rot combineRechitTimingDistributions.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root")'
# rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root")'
# rot makeMeanTimingPlot.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root",true)'
# rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root",true)'
# rot makeAnodeTimingPlot.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root",true)'
# rot combineAnodeTimingDistributions.C'("~/working/CSCTiming/CMSSW_10_1_0_pre2/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_mcdbv3.root")'

# rot makeMuonTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root", 59.97)'
# rot combineRechitTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root", 59.97)'
# rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root", 59.97)'
# rot makeMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root",true, 59.97)'
# rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root",true, 59.97)'
# rot makeAnodeTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root",true, 59.97)'
# rot combineAnodeTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018_agr.root", 59.97)'

# rot makeMuonTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root", 31.93)'
# rot combineRechitTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root", 31.93)'
# rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root", 31.93)'
# rot makeMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root",true, 31.93)'
# rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root",true, 31.93)'
# rot makeAnodeTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root",true, 31.93)'
# rot combineAnodeTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_prompt_agr.root", 31.93)'

rot makeMuonTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root", 6.48)'
rot combineRechitTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root", 6.48)'
rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root", 6.48)'
rot makeMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root",true, 6.48)'
rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root",true, 6.48)'
rot makeAnodeTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root",true, 6.48)'
rot combineAnodeTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_smu_2018D_LowLV_agr.root", 6.48)'

# rot makeMuonTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root", 10.3)'
# rot combineRechitTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root", 10.3)'
# rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root", 10.3)'
# rot makeMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root",true, 10.3)'
# rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root",true, 10.3)'
# rot makeAnodeTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root",true, 10.3)'
# rot combineAnodeTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_agr_csctiming2018A.root", 10.3)'

# rot makeMuonTimingPlot.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root")'
# rot combineRechitTimingDistributions.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root")'
# rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root")'
# rot makeMeanTimingPlot.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root",true)'
# rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root",true)'
# rot makeAnodeTimingPlot.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root",true)'
# rot combineAnodeTimingDistributions.C'("~/working/CSCTiming/CMSSW_10_2_5/src/CSCOfflineTiming/CSCTimingAnalyzer/test/histos_llv.root")'

# rot dumpPlots.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root")'
# rot makeMuonTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root")'
# rot combineRechitTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root")'
# rot combineSegmentTimingDistributions.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root")'
# rot makeMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root",true)'
# rot makeSegmentMeanTimingPlot.C'("~/working/CSCTiming/rootfiles/histos_removeHeuristicCorr.root",true)'

