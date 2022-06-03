import CRABClient
from WMCore.Configuration import Configuration

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
#config.General.requestName = 'SingleMuon_Run2018A_PromptReco_v3'
config.General.requestName = 'SingleMuon_Run2015A_PromptReco_v1'
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
#config.JobType.psetName = 'single_mu_cfg.py'
config.JobType.psetName = 'single_mu_zeroT_cfg.py'

config.section_('Data')
#config.Data.inputDataset = '/SingleMuon/Run2018A-PromptReco-v3/AOD'
config.Data.inputDataset = '/SingleMuon/Run2015A-PromptReco-v1/RECO'
config.Data.publication = True
config.Data.unitsPerJob = 200
config.Data.splitting = 'LumiBased'
config.Data.inputDBS = 'global'
#config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter'
#config.Data.outputDatasetTag = '2018A_PromptReco_v3'
config.Data.outputDatasetTag = '2015A_PromptReco_v1'
config.Data.outLFNDirBase = '/store/user/sicheng/csctiming/2018/'

config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_US_UCSD'
# In order to use gcc700 for now
config.JobType.allowUndistributedCMSSW = True
