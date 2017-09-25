from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SingleMuon_2017B_test3'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'singleMu_cfg.py'
config.JobType.outputFiles = ['histos_applyGoodRunList_removeHeuristicCorr.root']

config.section_('Data')
config.Data.inputDataset = '/SingleMuon/sicheng-v4-224aacb282965b46cd7cf73ce07f5f3d/USER'
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/sicheng/csctiming/2017/'
config.Data.unitsPerJob = 2 
config.Data.splitting = 'FileBased'
config.Data.inputDBS = 'phys03'

config.section_('User')

config.section_('Site')
config.Site.storageSite = 'T2_US_UCSD'
config.Site.whitelist = ['T2_US_*']

config.section_('Debug')
config.Debug.extraJDL = ['+DESIRED_SITES="T2_US_UCSD"']

