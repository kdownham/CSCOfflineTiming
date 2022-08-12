## CONFIG FOR 12_4_X ON RUN3 DATA

import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3
process = cms.Process("CSCTIMING", Run3)

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.EndOfProcess_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")

# 12_4_4 Prompt
process.GlobalTag.globaltag = '122X_dataRun3_Express_v3'

# automatically name the output file correctly and uniquely
baseFileName = 'csc_singleMu_test'
fileExtension = '.root'
outfileName = baseFileName

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# Standard configs

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring(
#                                'root://cmsxrootd.fnal.gov//store/data/Run2018B/SingleMuon/AOD/PromptReco-v2/000/318/820/00000/50043584-317C-E811-8635-FA163EFC186C.root')
#)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring( 
## Access recent AOD from Tier-0          
                                "root://eoscms.cern.ch://eos/cms/tier0//store/data/Commissioning2022/Cosmics/AOD/PromptReco-v1/000/349/467/00000/93c965bd-5536-4f2f-8c74-0b9a1f2aff4b.root"
)                        
)

process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(True)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName+fileExtension
# outfileName = 'test.root'

# From RAW
process.p = cms.Path(process.cscTimingBabyMaker)

#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string(outfileName),
#                               outputCommands = cms.untracked.vstring('drop *',
#                                                                      'keep *_*_*_CSCTIMING'))

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))


process.e = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)









