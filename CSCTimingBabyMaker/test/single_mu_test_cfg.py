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
process.GlobalTag.globaltag = '140X_dataRun3_Prompt_v4'
#process.GlobalTag.globaltag = '130X_dataRun3_Prompt_v4'
#process.GlobalTag.globaltag = '106X_dataRun2_v26'

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
#dirname='/eos/user/k/kdownham/CSCOfflineTiming/Run2024G/Run383811/baby_output/'
dirname='/eos/cms/store/group/dpg_csc/comm_csc/csctiming/Run3/2024/Muon1/Run2024G-PromptReco-v1/AOD/'
baseFileName = 'RAW2RECO_validation_2024G_Run383814'
fileExtension = '.root'
outfileName = dirname+baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/086420ee-e52f-454c-89cb-f8e7d343dccb.root',
				#'file:/eos/cms/store/user/valuev/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/666a56c6-005d-4ab7-b6e6-aa168ea0c855.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/019f963f-7fa4-4019-b41c-b1382f4f948a.root',
				#'file:/eos/user/k/kdownham/CSCOfflineTiming/Run2022D/Muon/AOD/PromptReco-v2/000/357/900/00000/03c69590-7ab5-4311-b7f5-714fee4d2c02.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024I/Muon0/AOD/PromptReco-v2/000/386/694/00000/0047c5a5-ee16-4ed2-bbe9-21808e514a54.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2023D/Muon0/AOD/PromptReco-v1/000/369/998/00000/57e8b54f-170d-40f3-a443-6d18eaee1317.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2023D/Muon0/AOD/PromptReco-v2/000/370/665/00000/f4c338fc-cf6c-4bec-af85-c9881f4a91cf.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/01de4889-822e-4d84-b7d5-bdf113311db4.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/13bb8d99-6fac-49bf-bbf0-ab289d67c28c.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/15896336-58fc-411f-ba53-dcd84b380e46.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/2a27a7cd-2e1d-4333-aa94-bf95a16c1ddc.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/2cbe7e80-e83f-42cc-a6a0-2c18c785aa76.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/427a1865-a2db-441e-83df-c0c2a40bba11.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/4cbbfdfb-178b-4be5-8a73-dfd78d6014fa.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/51d224ef-9453-4945-92d5-f4bd065360d0.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/54fc4f22-ece6-41ff-865c-814d5b840356.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/5e3d2ddb-806c-491d-9cdd-cca6d1b3d8bd.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/6a1e6445-2a97-491b-8d70-9d3a835f8ce0.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/6b29095c-7607-4f3f-b94f-49eb8ba385f9.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/769b478a-ed0b-442c-8ca9-081ce1689fbc.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/77fb287e-9bc5-42ab-84c2-d5104bbcb321.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/83bc265c-4756-4a3d-a062-731059deacb7.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/a8a97ab2-e210-49e8-a6d4-1c921405f6ba.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/a95f4480-eaad-4066-92bf-327a3edd929a.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/ab3f3cde-bd78-41d4-8418-b02aaa22228d.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/bc346b43-af71-42bd-ba11-ca7808c10d66.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/c2d07cbc-4146-4f36-b01e-f9a2a604e59b.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/cb822e43-7216-4a68-9ce4-e0cdcf7e1a11.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/dee2d0a6-c24f-435c-86a5-1a2df248e956.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/e16adf03-db56-460f-a1cc-4864866b40a9.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/383/811/00000/f3890e5b-79ba-4a4c-8178-00adce0e1671.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2023D/Muon0/AOD/PromptReco-v1/000/369/998/00000/4ab0fe1a-f3c9-4bbc-abe2-159cddf6bc94.root',
                                #'root://xrootd-cms.infn.it//store/data/Run2023D/Muon0/AOD/PromptReco-v1/000/369/998/00000/c906c11f-070f-4209-a7c4-4a68807c8864.root',
                                #'file:/eos/user/k/kdownham/CSCOfflineTiming/Validation/RAW2DIGI_RECO/RAW2RECO_2022G_Run362654_cathodes357900_mem_1-1-12_fix_anodes362654.root',
                                'file:/eos/cms/store/group/dpg_csc/comm_csc/csctiming/Run3/2024/Muon1/Run2024G-PromptReco-v1/RAW/RAW2RECO_2024G_Run383814_cathodes357900_anodes385801.root',
			)
)



process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)  # nominally set to false
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(True)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
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
#                                      connect=cms.string("sqlite_file:conditionsData/CSCCables_cathode357900_anode362654.db"),
#                                      DBParameters = cms.PSet(
#                                                              authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb'),
#                                                              authenticationMethod = cms.untracked.uint32(1)
#                                                              )
#                                      )
#process.es_prefer_cscTiming = cms.ESPrefer("PoolDBESSource","CSCTimingSource")

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
