from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SingleMuon_Run2017E_PromptReco_v1'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'single_mu_cfg.py'

config.section_('Data')
config.Data.inputDataset = '/SingleMuon/Run2017E-PromptReco-v1/AOD'
config.Data.publication = True
config.Data.unitsPerJob = 200
config.Data.splitting = 'LumiBased'
config.Data.inputDBS = 'global'
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter'
config.Data.outputDatasetTag = '2017E_PromptReco_v1'
config.Data.outLFNDirBase = '/store/user/sicheng/csctiming/2017/'

config.section_('User')

config.section_('Site')
config.Site.storageSite = 'T2_US_UCSD'

# In order to use gcc700 for now
config.JobType.allowUndistributedCMSSW = True
