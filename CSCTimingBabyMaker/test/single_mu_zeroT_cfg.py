import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCTIMING")

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# Express for 711 MWGR July 2014
#process.GlobalTag.globaltag = 'GR_E_V38::All'
process.GlobalTag.globaltag = 'GR_P_V55'

# Config CSC for postls1
process.CSCGeometryESModule.useGangedStripsInME1a = cms.bool(False)
process.CSCIndexerESProducer.AlgoName=cms.string("CSCIndexerPostls1")
process.CSCChannelMapperESProducer.AlgoName=cms.string("CSCChannelMapperPostls1")

# automatically name the output file correctly and uniquely
baseFileName = 'csc_singleMu_zeroT'
fileExtension = '.root'
outfileName = baseFileName

#process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# Standard configs
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
# process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('LogicError'))


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/FE6E0461-D10B-E511-A8D0-02163E014428.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/0AA27336-D00B-E511-8CC8-02163E0146AD.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/10A6A70F-CD0B-E511-9AAE-02163E01257B.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/14546E53-C00B-E511-8118-02163E014686.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/1617D82A-D30B-E511-8552-02163E011C5E.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/16B30320-160C-E511-934D-02163E012925.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/20A3AD51-F50B-E511-8ADF-02163E0141FA.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/22504143-D00B-E511-92FC-02163E0143B6.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/26830A76-BB0B-E511-8BAC-02163E01442E.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/26F7EB60-D00B-E511-9428-02163E0140FA.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/2A577F9D-D30B-E511-B4C9-02163E011891.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/30BB282E-BC0B-E511-8557-02163E014668.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/30D9722C-D30B-E511-82F0-02163E0136A3.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/329E5A91-BA0B-E511-8D84-02163E0138C6.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/34943D21-B90B-E511-B7EB-02163E012028.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/36BAC088-BA0B-E511-9C0F-02163E01432F.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/3A727E71-DA0B-E511-AD65-02163E0133DC.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/3C217877-CF0B-E511-98EC-02163E012325.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/4852E685-BB0B-E511-9C37-02163E01457A.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/4E8D9DAE-D10B-E511-9A3A-02163E013556.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/5E64F63E-BB0B-E511-8D84-02163E012060.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/5E97E24F-D60B-E511-B782-02163E0134B0.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/6085AEB2-D00B-E511-B33A-02163E011B58.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/60CFC63B-E50B-E511-BCFE-02163E014695.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/620928DD-CE0B-E511-8140-02163E013556.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/645B9700-BB0B-E511-A156-02163E013556.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/6A0B608C-BA0B-E511-9905-02163E011B3D.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/6ADE32BC-D00B-E511-9160-02163E014239.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/6EC64E79-CF0B-E511-A0F4-02163E011B3E.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/70C975A0-BD0B-E511-A027-02163E014638.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/7870FED1-CC0B-E511-810E-02163E0143EB.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/7AE993F2-BE0B-E511-B423-02163E011B35.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/8406E806-BC0B-E511-9813-02163E014668.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/88308517-D10B-E511-98DF-02163E013570.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/88DB8BD3-CC0B-E511-94F7-02163E014763.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/8E602F36-CD0B-E511-9BC2-02163E011B28.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/96205104-BB0B-E511-AEDC-02163E011AF3.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/A08337A9-D10B-E511-B88F-02163E013556.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/A4313091-DA0B-E511-BD1A-02163E01365C.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/ACBB928F-BA0B-E511-BA84-02163E011942.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/AE1FD51E-CE0B-E511-B9E7-02163E013614.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/B0DB4B38-D00B-E511-BBDD-02163E01374A.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/B699C3D5-D30B-E511-B0D2-02163E011A33.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/BC182B42-CB0B-E511-B030-02163E011922.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/BCB7DF3C-B90B-E511-B2CC-02163E014349.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/C4B31889-E40B-E511-BE1D-02163E014485.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/C64ED45E-ED0B-E511-9ED4-02163E011C7F.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/CAA698CE-DF0B-E511-B799-02163E013774.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/CAC03C32-CD0B-E511-96D2-02163E014686.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/CE138861-CE0B-E511-9739-02163E014334.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/D27188DD-B80B-E511-BF6D-02163E0121D4.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/D64E0730-BB0B-E511-A25A-02163E0126EB.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/D6B7DCBB-CD0B-E511-A63D-02163E01473C.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/DA707950-D00B-E511-8736-02163E0136B6.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/DA76395B-D20B-E511-B4EC-02163E01451D.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/E00BED10-BA0B-E511-A850-02163E0133DC.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/E4A4E376-BB0B-E511-B14E-02163E011883.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/E4D4062A-D30B-E511-9EA2-02163E01475C.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/E6534E7E-BB0B-E511-B71C-02163E0143F2.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/E8C7CE00-BB0B-E511-A0F2-02163E012218.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/EA6C5A4B-BB0B-E511-A801-02163E011AF3.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/F27DC195-BE0B-E511-A5F2-02163E011AF3.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/F2DFCF5A-BB0B-E511-A327-02163E011FEB.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/FCB59D68-DC0B-E511-8C35-02163E014239.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/FE2BCB1B-BB0B-E511-B4B0-02163E01458C.root',
                                '/store/data/Run2015A/SingleMu/RECO/PromptReco-v1/000/246/951/00000/FE6E0461-D10B-E511-A8D0-02163E014428.root')
)

process.load('CSCOfflineTiming.CSCTimingBabyMaker.cscTimingBabyMaker_cfi')
process.cscTimingBabyMaker.debug = cms.untracked.bool(False)
process.cscTimingBabyMaker.useMuonSegmentMatcher = cms.untracked.bool(True)

if process.cscTimingBabyMaker.useMuonSegmentMatcher:
    outfileName = outfileName + '_useMuonSegmentMatcher'
outfileName = outfileName+fileExtension
# outfileName = 'test.root'
print outfileName

# From RAW
process.p = cms.Path(process.cscTimingBabyMaker)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outfileName),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CSCTIMING'))

process.e = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.e)
