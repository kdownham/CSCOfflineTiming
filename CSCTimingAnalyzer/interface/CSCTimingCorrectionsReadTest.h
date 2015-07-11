// -*- C++ -*-
//
// Package:    CSCOfflineTiming/CSCTimingCorrectionsReadTest
// Class:      CSCTimingCorrectionsReadTest
// 
/**\class CSCTimingCorrectionsReadTest CSCTimingCorrectionsReadTest.cc CSCOfflineTiming/CSCTimingCorrectionsReadTest/plugins/CSCTimingCorrectionsReadTest.cc

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
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/CSCObjects/interface/CSCChamberTimeCorrections.h"

class CSCDetId;
class CSCIndexerBase;
class CSCChannelMapperBase;

//
// class declaration
//
class CSCTimingCorrectionsReadTest : public edm::EDAnalyzer {
public:
    explicit CSCTimingCorrectionsReadTest(const edm::ParameterSet&);
    ~CSCTimingCorrectionsReadTest();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    edm::ESHandle<CSCChamberTimeCorrections> theChamberTimingCorrections;
    edm::ESHandle<CSCIndexerBase> indexer;
    edm::ESHandle<CSCChannelMapperBase> mapper;

    bool isLoaded_;
};
