# CSCOfflineTiming

User need to follow the instructions from the [TWiki](https://twiki.cern.ch/twiki/bin/view/CMS/CSCTimingCalibration).

## Setup
Check out the `setup.sh` script at the same level where you want your CMSSW release to be
```
curl -L https://git.io/vdTDj > setup.sh
# Modify the CMSSW version in setup.sh
# edit setup.sh
. setup.sh
```
Note that you don't need a CMSSW release beforehand as the script should check out the version automatically
