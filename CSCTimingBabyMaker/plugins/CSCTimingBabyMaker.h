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
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/Math/interface/LorentzVector.h"

typedef math::XYZTLorentzVectorD LorentzVector;

class CSCRecoConditions;
class CSCChamberTimeCorrections;
class CSCIndexerBase;
class CSCChannelMapperBase;
class MuonSegmentMatcher;
class MuonServiceProxy;

//
// class declaration
//

class CSCTimingBabyMaker : public edm::EDFilter {
public:
    explicit CSCTimingBabyMaker(const edm::ParameterSet&);
    ~CSCTimingBabyMaker();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
      
    virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // member functions to get updated skew clear cable length/delays for ME1/1 chambers
    bool isGoodVertex (const reco::Vertex& vertex);
    static int getSkewClearCableLengthForME11 (const CSCDetId &id);
    static int getSkewClearCableDelayForME11 (const CSCDetId &id);

    bool isPrunedTrigger (const std::string& name) const;
    void fillTriggerObjInfo (unsigned int triggerIndex, std::vector<int>& ids, std::vector<LorentzVector>& p4s) const;

    // ----------member data ---------------------------
    edm::ESHandle<CSCChamberTimeCorrections> theChamberTimingCorrections;
    edm::ESHandle<CSCIndexerBase> indexer;
    edm::ESHandle<CSCChannelMapperBase> mapper;
    CSCRecoConditions *recoConditions;
    MuonServiceProxy *muonServiceProxy;
    MuonSegmentMatcher *muonSegmentMatcher;
    bool isLoaded_;

    edm::EDGetTokenT<reco::MuonCollection> mu_token;
    edm::EDGetTokenT<reco::VertexCollection> vtx_token;
    edm::EDGetTokenT<reco::TrackCollection> trk_token;
    edm::EDGetTokenT<reco::BeamSpot> bs_token;
    edm::EDGetTokenT<edm::TriggerResults> trgResults_token;
    edm::EDGetTokenT<trigger::TriggerEvent> trgEvent_token;

    edm::Handle<trigger::TriggerEvent> triggerEvent_h;
    edm::Handle<edm::TriggerResults> trigResults_h;

    std::string processName;
    std::vector<std::string> prunedTriggerNames;
    
    HLTConfigProvider hltConfig;

    bool useMuonSegmentMatcher_;
    bool fillTriggerObjects_;    
    bool debug_;    
    
    int nSAmus_;
    int nMusWithMatchedSegments_;
    int nMusWithCSCrhs_;

    static const unsigned int ME11P_SKEWCLEAR_CABLE_LENGTHS[36]; // length in centimeters
    static const unsigned int ME11M_SKEWCLEAR_CABLE_LENGTHS[36]; // length in centimeters
    static const unsigned int PPIB_DCFEB_CABLE_LENGTH; // length in centimeters
    static const double SKEWCLEAR_REVD_PROPAGATION_DELAY;  // [ns/m]
    static const double SKEWCLEAR_REVO_PROPAGATION_DELAY;  // don't know the real value, for now use 5 ns/m
    static const double PPIB_LVDS_DELAY; // from M. MatveevP: ~5 ns delay from LVDS receiver and LVDS transmitter on PPIB 
    static const double AVG_CHIP_CORR;    
};
