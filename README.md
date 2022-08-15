# CSCOfflineTiming

User need to follow the instructions from the [TWiki](https://twiki.cern.ch/twiki/bin/view/CMS/CSCTimingCalibration).

## Setup
Check out the `setup.sh` script at the same level where you want your CMSSW release to be
```
curl -L https://raw.githubusercontent.com/kdownham/CSCOfflineTiming/master/setup.sh > setup.sh
# Modify the CMSSW version in setup.sh
# edit setup.sh
. setup.sh
```
Note that you don't need a CMSSW release beforehand as the script should check out the version automatically

Go into the Buildfiles `RecoLocalMuon/CSCRecHitD/BuildFile.xml` and `RecoLocalMuon/CSCValidation/BuildFile.xml` 
and delete the following line
```
<flags EDM_PLUGIN="1"/>
```
Then add the following three lines to the bottom of the buildfiles:
```
<export>
<lib name="1"/>
</export>
```
Save and close these files.

Now go to `RecoLocalMuon/CSCRecHitD/python` and add the following file to this directory

https://cmssdt.cern.ch/lxr/source/cfipython/RecoLocalMuon/CSCRecHitD/cscRecHitDProducer_cfi.py

(running the ntuple maker without this file will cause it to crash)


Finally, go back to your source (src) directory and build
```
scram b -j 12
```
##Running the ntuple maker
Configuration files for running the ntuple maker are in `CSCOfflineTiming/CSCTimingBabyMaker/test`.
An example configuration file for running is `single_mu_test_cfg.py`. To run the ntuple maker, modify 
line 44 of this file with an existing data file path on eos and then modify line 18 with the corresponding
global tag for the dataset (which can be found on DAS).

Once the dataset and global tag have been set, run the ntuple maker by executing the following command:
```
cmsRun single_mu_test_cfg.py
```
