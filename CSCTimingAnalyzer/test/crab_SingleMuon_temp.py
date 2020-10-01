from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'Analysis_smu_2018LowLV'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
# config.JobType.psetName = 'singleMu_cfg.py'
# config.JobType.outputFiles = ['histos_applyGoodRunList_removeHeuristicCorr.root']
config.JobType.psetName = 'test_cfg.py'
config.JobType.outputFiles = ['histos_applyGoodRunList.root']
# config.JobType.outputFiles = ['histos_testCorrectWireTime_applyGoodRunList_newHeuristicCorrByChamber.root']

config.section_('Data')
# config.Data.inputDataset = '/SingleMuon/sicheng-smu_2018A_rereco-433eed72236d772a97926f27cfe94c12/USER'
# config.Data.inputDataset = '/SingleMuon/sicheng-smu_2018B_rereco-433eed72236d772a97926f27cfe94c12/USER'
# config.Data.inputDataset = '/SingleMuon/sicheng-smu_2018C_rereco-433eed72236d772a97926f27cfe94c12/USER'
config.Data.inputDataset = '/SingleMuon/sicheng-smu_2018D_prompt-430d9978411c1c1e6cded7d65836d167/USER'
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/sicheng/csctiming/2018/'
config.Data.unitsPerJob = 2 
config.Data.splitting = 'FileBased'
config.Data.inputDBS = 'phys03'

config.Data.runRange = '324077-325175'

config.section_('User')

config.section_('Site')
config.Site.storageSite = 'T2_US_UCSD'
config.Site.whitelist = ['T2_US_UCSD']

config.section_('Debug')
config.Debug.extraJDL = ['+DESIRED_SITES="T2_US_UCSD"']

config.JobType.allowUndistributedCMSSW = True
