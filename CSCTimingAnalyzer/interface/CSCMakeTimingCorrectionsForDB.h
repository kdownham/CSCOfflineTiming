// -*- C++ -*-
//
// Package:    CSCOfflineTiming/CSCMakeTimingCorrectionsForDB
// Class:      CSCMakeTimingCorrectionsForDB
//
/**\class CSCMakeTimingCorrectionsForDB CSCMakeTimingCorrectionsForDB.cc CSCOfflineTiming/CSCMakeTimingCorrectionsForDB/plugins/CSCMakeTimingCorrectionsForDB.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:
//         Created:  Mon, 13 Apr 2015 18:06:49 GMT
//
//


// system include files
#include <memory>
#include <string>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "RecoLocalMuon/CSCValidation/src/CSCValHists.h"
#include "CSCValHists.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "CondFormats/CSCObjects/interface/CSCChamberTimeCorrections.h"

#include "TFile.h"
#include "TTree.h"

class CSCDetId;
class CSCIndexerBase;
class CSCChannelMapperBase;
class CSCRecoConditions;
class CSCDBChipSpeedCorrection;

//
// class declaration
//
class CSCMakeTimingCorrectionsForDB : public edm::EDAnalyzer {
public:
    explicit CSCMakeTimingCorrectionsForDB(const edm::ParameterSet&);
    ~CSCMakeTimingCorrectionsForDB();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
    unsigned int getUpdatedAnodeBXOffset (CSCDetId id);

    edm::ESHandle<CSCChamberTimeCorrections> theChamberTimingCorrections;
    edm::ESHandle<CSCIndexerBase> indexer;
    edm::ESHandle<CSCChannelMapperBase> mapper;
    edm::ESHandle<CSCDBChipSpeedCorrection> theChipCorrections;

    std::string heuristicCorrFileName;
    std::string cableLengthFileName;
    std::string outFileName;
    std::string outChipFileName;
    bool updateOnlyNewChambers_;

    CSCRecoConditions *recoConditions;
    bool isLoaded_;

    std::map<CSCDetId, unsigned int> mid;
    std::map<unsigned int, CSCChamberTimeCorrections::ChamberTimeCorrections> mvals;
    std::map<CSCDetId, int> mcorr;
    std::map<CSCDetId, int> mcable;
    std::map<CSCDetId, int> mcrev;
    std::map<CSCDetId, int> mdelay;

    static const unsigned int NSTATIONS;
};
