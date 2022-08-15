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
Save and close these files, then go back to your source (src) directory and build
```
scram b -j 12
```
