#################################################################
# Template Script for Batch Submission of CSCTimingBabyMaker Jobs
#
# Author:
#	Keegan Downham, UCSB
#################################################################


import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process("CSCTIMING")

process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = cms.untracked.vstring('GLOBALTAG_REPLACETAG')

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = cms.untracked.vstring('OUTPUTNAME_REPLACETAG')
fileExtension = ''
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(MAXEVENTS_REPLACETAG) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
			    'FILENAME_REPLACETAG'
			)
)



process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)  # nominally set to false
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(True)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    #outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName+fileExtension

process.p = cms.Path(process.cscTimingBabyMaker)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))


process.e = cms.EndPath(process.out)


# Comment this block out if you don't want to read from the local conditions data
#process.CSCTimingSource = cms.ESSource("PoolDBESSource",
#                                      loadAll = cms.bool(True),
#                                      timetype = cms.string('runnumber'),
#                                      toGet = cms.VPSet(cms.PSet(
#                                                                 record = cms.string('CSCChamberTimeCorrectionsRcd'),
#                                                                 tag = cms.string('CSCChamberTimeCorrections')
#                                                                 )),
#                                      connect=cms.string("sqlite_file:conditionsData/NewAnodeOffsets_357900_goodCathodes.db"),
#                                      DBParameters = cms.PSet(
#                                                              authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb'),
#                                                              authenticationMethod = cms.untracked.uint32(1)
#                                                              )
#                                      )
#process.es_prefer_cscTiming = cms.ESPrefer("PoolDBESSource","CSCTimingSource")

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
