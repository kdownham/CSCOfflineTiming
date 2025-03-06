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
dirname='/eos/cms/store/group/dpg_csc/comm_csc/csctiming/Run3/2024/Muon1/Run2024G-PromptReco-v1/AOD/Run385801/BabyOutput/'
baseFileName = 'validate_2024G_Run385801_10k'
fileExtension = '.root'
outfileName = dirname+baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
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
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/4200bf88-e510-4368-85fa-1eda29aeabc3.root', 
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/3bc090a5-ec3b-477b-88dd-8188be524f47.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/4e6aa05a-a4d9-4534-aefc-1590b1a1d3f6.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/d13c135a-7670-4dca-9434-4b4e7dc15768.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/cfea72be-e00c-49ab-8644-16626e63fe70.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/c1cf0b88-301e-4e3b-8fd9-c7c47acb8339.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/1ca7c914-3318-46b1-b62e-876325d0237d.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/83911ef3-2d5a-4eb0-8266-eb2bf590c861.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/3ea1f052-16ef-4d78-b9ba-e963c612ff1e.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/8d6624da-5f9c-43d5-b717-880317b0530b.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/0fd02063-81d9-48bf-b854-1e148bb5812b.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5305eb12-850a-473c-acd4-e64b2e9e39d1.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/19a99bd9-854f-4584-ac6e-ee36c8f5a41c.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/d3ab3f1a-94c2-46b5-99ee-89a917eaa629.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/bddccf9c-8304-4397-b1d4-897f2a5f9ea2.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5770b197-b43a-430c-98a9-e8318c680ab2.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/b3d4ed7b-af60-4499-ab2e-636577d1ee54.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/62c5f404-a34f-4233-910d-053a19cb2635.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/386cd87b-cf11-4100-b12c-d02fab63e563.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/53d81538-0e40-41ec-8acd-8f5e17cc32c1.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/112f7f73-1456-4dbf-b7fe-1f5d9fe6001f.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/f3bbfcbb-a604-4312-a534-f2c8560efb74.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/afe45b1e-1440-4f48-8faa-d74ab4d56329.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/fea3a979-5ad9-45fb-87d8-725583afed2e.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/4b656352-7683-44a7-81e8-7b7c7f65b934.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/7b78e839-57be-40a5-933d-9336989112c0.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/fa7f05f1-587f-4725-9f00-79b546a8bc50.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/fbe7b660-2e75-4316-aa29-44aa3cd2d2dd.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/327f0ba2-6a9d-41e7-a6b0-405a98c0acfa.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5c4c6a9c-9667-4cb1-9318-895c07dd191d.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/f2c10769-1ab9-4775-b3dc-7587cffc6ce6.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/3fd938bc-d50f-499a-b0af-c403c9f8568f.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/7b6a1bf9-0b3e-492e-b250-37f2a693f576.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/d40dbab9-778b-4550-b7b9-66a2f44d9f86.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/e17bfaf0-d6e3-4872-8a5f-8a53faba1541.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/2a79e03d-74b6-4092-ae96-2e0c61f90645.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/948b5285-342f-42ac-acb7-299c84d70843.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/38e8039b-f5ba-4fa9-88bf-f9dc36b9a818.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/2e7e0905-42fa-4a11-967d-9a0a1e110415.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/44f66a19-b7ed-4cd1-9843-0475632bde7e.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/7c8fea19-36db-4398-bb44-77c343c3226f.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/e1119ca3-9760-4e96-8d39-93eab7243abd.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5d73b943-723b-4670-a472-dc46820e6b6a.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/0c3350cc-b22d-412c-9b0f-ecbebc600ea9.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/a31dff91-fc2c-43e4-b1df-b1cc0cedbb8c.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/47d57285-1d23-412f-833b-cc1196bdc9f1.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/95b2b8d5-a4ea-461c-a43a-0f5fbb04b1cf.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/47d68ceb-9742-4db7-98f8-c3e9e9116a8f.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/3d3fccac-3955-45f9-86c5-1bb6e298b59f.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/294553de-f9c2-483c-abe8-0890266e57af.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/c76a87b1-4afd-407c-8be6-567949a795c5.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/47590e63-bca1-431a-84d3-87837cfa37a2.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5d496502-a7b4-44ac-981e-60ee6fe2a479.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/0ec34c57-ee3a-4e17-9bb4-8de5a75f7870.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/75273df8-8e56-4971-af59-03eb44cdcb5f.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/7150d0a3-81ae-441f-974e-9ee90236f2d5.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/3f253b12-fb36-46ca-bdde-72bfcadd3df7.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/4e4e4b09-fdbc-46ba-a909-d95cfbc33525.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/cbbda07f-4af9-4a41-8608-86ff08efb9a2.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/b73d386c-3a0e-470b-bceb-403c0954e82a.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/bc941f0a-6675-43f3-b017-ea9678e4dddc.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/089da145-e2d9-418a-b5aa-cd0a216c7283.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/e7e2e3ba-899e-4e3c-8a2c-fe6dc17997b4.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/d571a905-c7df-4b1b-a7d6-13fede3fb701.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/b4b53b3d-e2dc-4ef2-89f7-0c13bc376d22.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/2bf9e40d-979b-4969-b6a1-b5a100c228a7.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/f4963ade-7d88-4e8f-9f50-a06ddc9e1664.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/9daac7a6-3678-4659-bb0b-bdfc32481952.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/8edcc699-59af-480a-987b-bb1207da1cd5.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/17375a02-90e3-4302-aa32-e31b827e7ef2.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/3a1c5d06-eed7-4a58-a8a0-f9b0083cf062.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/563e3868-d189-432a-8b23-dea4fca9103e.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/c696faff-571d-43fe-b238-0e726e55df47.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/e4f43fe7-672f-4a4f-92a2-adc0654ff527.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/8d3d6fc5-9778-4cf0-9cc0-519b6d70a9d8.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/c3a73acb-3189-4c97-9fd6-206a4cb484d3.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/0631d7cb-6306-4c2e-9867-33229b8946c4.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/28d3de24-caf7-4093-b539-a529f7d4e579.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5a14df5d-7b9e-4b96-a549-3a86ba2c7af2.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/32d8f615-0625-4568-a913-7b2afae5ecbe.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/e905b8a5-2bb4-4062-9e19-70528835a690.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/8b902299-fed2-497b-a847-a272a6ffb4f0.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/1de06f3d-6223-495c-a712-5797961712a1.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/b6517ed8-7720-456a-9126-2e7247445c14.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/16ad9643-8efa-4219-800e-d2fbab9fcdf1.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/332061f3-c1f9-4944-8b5a-f743d1ae8365.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/d6765176-42cb-4dc6-aa20-fba0e1e9873c.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/07d3254e-d53b-4d43-8b08-3019de2850c0.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/5b21addc-7bf3-49fd-a08d-d18d96e3b7d0.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/658837a9-cf63-46b7-8078-a193c2e19874.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/411e14ca-75db-45a4-b8ca-f2838ab28020.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/ad1262d8-7e77-4fad-9fa3-098af035bde0.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/db0d0f03-5f8c-464a-8805-099c26907bde.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/fc8429fc-4658-45fb-b89c-b982991b31ed.root',
                                'root://xrootd-cms.infn.it//store/data/Run2024G/Muon1/AOD/PromptReco-v1/000/385/801/00000/a668fbe5-0923-4c88-8b17-cb60a1d81667.root',
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
