#! /usr/bin/env python

import string
import commands, os, re
import sys 
                      

cmsswSkelFile = ''
configPfx = 'cfg/'
psetPfx = 'pset/'
dataSet = ''
numEvtsTotal = -1
numEvtsPerJob = 20000
numLumisPerJob = 500
filesPerJob = 2
outNtupleName = 'ntuple.root'
storageElement = 'T2_US_UCSD'
tag = 'V07-00-03'
mode = 'remoteGlidein'
dbs_url = 'phys03'
report_every = 1000;
global_tag = '';

isData  = False;

def makeCrab3Config():
    outFileName = dataSet.split('/')[1]+'_'+dataSet.split('/')[2]
    outFile = open(configPfx + outFileName + '.py', 'w')
    print 'Writing CRAB config file: ' + configPfx + outFileName + '.py'

    outFile.write('from WMCore.Configuration import Configuration\n')
    outFile.write('config = Configuration()\n')
    outFile.write('config.section_(\'General\')\n')
    outFile.write('config.General.transferOutputs = True\n')
    outFile.write('config.General.transferLogs = True\n')
    outFile.write('config.General.requestName = \'%s\'\n' % outFileName)
    outFile.write('\n')
    outFile.write('config.section_(\'JobType\')\n')
    outFile.write('config.JobType.pluginName = \'Analysis\'\n')
    outFile.write('config.JobType.psetName = \'%s_cfg.py\'\n' % ('./' + psetPfx + outFileName))
    # outFile.write('config.JobType.outputFiles = [\'%s\']\n' % outNtupleName)
    outFile.write('\n')
    outFile.write('config.section_(\'Data\')\n')
    outFile.write('config.Data.inputDataset = \'%s\'\n' % dataSet)
    outFile.write('config.Data.publication = False\n')
    outFile.write('config.Data.unitsPerJob = %i \n' % filesPerJob)
    # outFile.write('config.Data.unitsPerJob = %i \n' % int(numLumisPerJob))
    outFile.write('config.Data.splitting = \'FileBased\'\n')
    # outFile.write('config.Data.splitting = \'LumiBased\'\n')
    outFile.write('config.Data.inputDBS = \'%s\'\n' % dbs_url)
    #outFile.write('config.Data.ignoreLocality = True\n')
    outFile.write('\n')
    outFile.write('config.section_(\'User\')\n')
    outFile.write('\n')
    outFile.write('config.section_(\'Site\')\n')
    outFile.write('config.Site.storageSite = \'T2_US_UCSD\'\n')
    #outFile.write('config.Site.whitelist = [\'T2_US_Caltech\',\'T2_US_Florida\', \'T2_US_MIT\', \'T2_US_Nebraska\', \'T2_US_Purdue\', \'T2_US_UCSD\', \'T2_US_Vanderbilt\', \'T2_US_Wisconsin\']\n')
    outFile.write('\n')
	 
#
def makeCMSSWConfig(cmsswSkelFile):
    foundOutNtupleFile = False
    foundreportEvery   = False
    foundcmsPath       = False
    inFile = open(cmsswSkelFile, 'r').read().split('\n')
    nlines = 0
    iline  = 0
    for i in inFile:
        nlines += 1
        if i.find(outNtupleName) != -1:
            foundOutNtupleFile = True
        if i.find('reportEvery') != -1:
            foundOutNtupleFile = True
    if foundOutNtupleFile == False:
        print 'The root file you are outputting is not named ntuple.root as it should be for a CMS3 job.'
        print 'Please check the name of the output root file in your PoolOutputModule, and try again'
        print 'Exiting!'
        sys.exit()
    psetPfx = 'pset/'
    outFileName = dataSet.split('/')[1]+'_'+dataSet.split('/')[2] + '_cfg.py'
    print 'Writing CMSSW python config file : ' + psetPfx + outFileName
    outFile = open(psetPfx + outFileName, 'w')
    outFile.write( 'import sys, os' + '\n' + 'sys.path.append( os.getenv("CMSSW_BASE") + "/crab" )' + '\n' )
    for i in inFile:
        iline += 1
        if i.find('reportEvery') != -1:
            outFile.write('process.MessageLogger.cerr.FwkReport.reportEvery = ' + str(report_every) + '\n'); continue

        if i.find('globaltag') != -1:
            outFile.write('process.GlobalTag.globaltag = "' + global_tag + '"\n'); continue

        outFile.write(i)
        if iline < nlines:
            outFile.write('\n')

    outFile.close()

def checkConventions():
    if( not os.path.isdir('./'+configPfx) ):
        print 'Directory for configs (%s) does not exist. Creating it.' % configPfx
        os.makedirs('./'+configPfx)

    if( not os.path.isdir('./'+psetPfx) ):
        print 'Directory for psets (%s) does not exist. Creating it.' % psetPfx
        os.makedirs('./'+psetPfx)

    
    print 'CRAB submission should happen outside of {%s,%s}' % (configPfx, psetPfx)

if len(sys.argv) < 7 :
    print 'Usage: makeCrabFiles.py [OPTIONS]'
    print '\nWhere the required options are: '
    print '\t-CMSSWcfg\tname of the skeleton CMSSW config file '
    print '\t-d\t\tname of dataset'
    print '\t-t\t\tCSCTimingAnalyzer tag'
    print '\nOptional arguments:'
    print '\t-gtag\t\tglobal tag'
    print '\t-isData\t\tFlag to specify if you are running on data.'
    print '\t-strElem\tpreferred storage element. Default is T2_US_UCSD if left unspecified'
    print '\t-nEvts\t\tNumber of events you want to run on. Default is -1'
    print '\t-evtsPerJob\tNumber of events per job. Default is 20000'
    print '\t-filesPerJob\tNumber of files per job. Default is 2'
    #print '\t-n\t\tName of output Ntuple file. Default is ntuple.root'
    print '\t-m\t\tsubmission mode (possible: condor_g, condor, glite). Default is glidein'
    print '\t-dbs\t\tdbs url'
    print '\t-re\t\tMessage Logger modulus for error reporting. Default is 1000'
    sys.exit()


for i in range(0, len(sys.argv)):
    if sys.argv[i] == '-CMSSWcfg':
        cmsswSkelFile = sys.argv[i+1]
    if sys.argv[i] == '-d':
        dataSet = sys.argv[i+1]
    if sys.argv[i] == '-nEvts':
        numEvtsTotal = sys.argv[i+1]
    if sys.argv[i] == '-evtsPerJob':
        numEvtsPerJob = sys.argv[i+1]
    if sys.argv[i] == '-filesPerJob':
        filesEvtsPerJob = sys.argv[i+1]
    if sys.argv[i] == '-lumisPerJob':
        numLumisPerJob = sys.argv[i+1]
    if sys.argv[i] == '-strElem':
        storageElement = sys.argv[i+1]
    if sys.argv[i] == '-t':
        tag  = str(sys.argv[i+1])
    if sys.argv[i] == '-m':
        mode  = str(sys.argv[i+1])
    if sys.argv[i] == '-dbs':
        dbs_url = str(sys.argv[i+1])
    if sys.argv[i] == '-re':
        report_every = str(sys.argv[i+1])
    if sys.argv[i] == '-gtag':
        global_tag = str(sys.argv[i+1])
    if sys.argv[i] == '-isData':
        isData = True

if os.path.exists(cmsswSkelFile) == False:
    print 'CMSSW skeleton file does not exist. Exiting'
    sys.exit()

checkConventions()
makeCMSSWConfig(cmsswSkelFile)
makeCrab3Config()
