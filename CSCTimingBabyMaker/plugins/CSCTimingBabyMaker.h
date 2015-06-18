// -*- C++ -*-
//
// Package:    CSCOfflineTiming/CSCTimingBabyMaker
// Class:      CSCTimingBabyMaker
// 
/**\class CSCTimingBabyMaker CSCTimingBabyMaker.cc CSCOfflineTiming/CSCTimingBabyMaker/plugins/CSCTimingBabyMaker.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Tue, 31 Mar 2015 17:07:23 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"

class CSCRecoConditions;
class CSCChamberTimeCorrections;
class CSCIndexerBase;
class CSCChannelMapperBase;

//
// class declaration
//

class CSCTimingBabyMaker : public edm::EDProducer {
public:
    explicit CSCTimingBabyMaker(const edm::ParameterSet&);
    ~CSCTimingBabyMaker();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void beginJob() override;
    virtual void produce(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
      
    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // member functions to get updated skew clear cable length/delays for ME1/1 chambers
    static int getSkewClearCableLengthForME11 (const CSCDetId &id);
    static int getSkewClearCableDelayForME11 (const CSCDetId &id);

    // ----------member data ---------------------------
    edm::ESHandle<CSCChamberTimeCorrections> theChamberTimingCorrections;
    edm::ESHandle<CSCIndexerBase> indexer;
    edm::ESHandle<CSCChannelMapperBase> mapper;
    CSCRecoConditions *recoConditions;
    bool isLoaded_;

    edm::EDGetTokenT<reco::MuonCollection> mu_token;
    edm::EDGetTokenT<reco::VertexCollection> vtx_token;
    edm::EDGetTokenT<reco::TrackCollection> trk_token;

    static const unsigned int ME11P_SKEWCLEAR_CABLE_LENGTHS[36]; // length in centimeters
    static const unsigned int ME11M_SKEWCLEAR_CABLE_LENGTHS[36]; // length in centimeters
    static const unsigned int PPIB_DCFEB_CABLE_LENGTH; // length in centimeters
    static const double SKEWCLEAR_REVD_PROPAGATION_DELAY;  // [ns/m]
    static const double SKEWCLEAR_REVO_PROPAGATION_DELAY;  // don't know the real value, for now use 5 ns/m
    static const double PPIB_LVDS_DELAY; // from M. MatveevP: ~5 ns delay from LVDS receiver and LVDS transmitter on PPIB 
    static const double AVG_CHIP_CORR;
};
