#include "CSCOfflineTiming/CSCTimingBabyMaker/plugins/CSCTimingBabyMaker.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/angle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSegmentMatch.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "CalibMuon/CSCCalibration/interface/CSCChannelMapperRecord.h"
#include "CalibMuon/CSCCalibration/interface/CSCChannelMapperBase.h"
#include "RecoLocalMuon/CSCRecHitD/src/CSCRecoConditions.h"
#include "CondFormats/DataRecord/interface/CSCChamberTimeCorrectionsRcd.h"
#include "CondFormats/CSCObjects/interface/CSCChamberTimeCorrections.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerRecord.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerBase.h"
#include "FWCore/Common/interface/TriggerNames.h"

// #include "FWCore/Common/interface/TriggerNames.h"
// #include "DataFormats/Common/interface/TriggerResults.h"

typedef math::XYZTLorentzVectorD LorentzVector;

const unsigned int CSCTimingBabyMaker::PPIB_DCFEB_CABLE_LENGTH = 270; // length in centimeters
const double CSCTimingBabyMaker::SKEWCLEAR_REVD_PROPAGATION_DELAY = 5.2;  // [ns/m]
const double CSCTimingBabyMaker::SKEWCLEAR_REVO_PROPAGATION_DELAY = 5.05;  // don't know the real value, for now use 5 ns/m
const double CSCTimingBabyMaker::PPIB_LVDS_DELAY = 5; // from M. MatveevP: ~5 ns delay from LVDS receiver and LVDS transmitter on PPIB 
const double CSCTimingBabyMaker::AVG_CHIP_CORR = -52.41;

const unsigned int CSCTimingBabyMaker::ME11P_SKEWCLEAR_CABLE_LENGTHS[36] = {1100,1150,1100,1050,1000,1050,1000,1050,1100,1150,1100,1050,900,950,1200,1150,1100,1150,1100,1150,1100,1050,1000,950,900,900,1100,1050,1000,950,900,950,1200,1150,1100,1150};
const unsigned int CSCTimingBabyMaker::ME11M_SKEWCLEAR_CABLE_LENGTHS[36] = {1150,1150,1100,1150,1000,1050,1000,1050,1100,1150,1100,950,900,950,1200,1250,1100,1150,1100,1150,1100,1150,1000,1050,900,950,1000,1050,1050,950,950,950,1200,1150,1100,1150};

//
// constructors and destructor
//
CSCTimingBabyMaker::CSCTimingBabyMaker(const edm::ParameterSet& iConfig) :
    isLoaded_(false)
{
    recoConditions = new CSCRecoConditions(iConfig);

    mu_token  = consumes<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muonTag"));
    vtx_token = consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vtxTag"));
    trk_token = consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("trkTag"));
    bs_token  = consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("bsTag"));
    trgResults_token = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("trgResTag"));    
    debug_ = iConfig.getUntrackedParameter<bool>("debug", false);

    //
    // event level quantities
    //
    produces<int>       ("run" ).setBranchAlias("run" );
    produces<int>       ("lumi").setBranchAlias("lumi");
    produces<unsigned long long> ("evt" ).setBranchAlias("evt" );
    produces<double>    ("bfield").setBranchAlias("bfield");
    produces<bool>      ("cscstatus").setBranchAlias("csc_status");
    produces<unsigned int> ("detectorstatus").setBranchAlias("detector_status");
      
    //
    // muon level quantities
    //
    produces<std::vector<bool> > ("isglobal"  ).setBranchAlias("is_global"  );
    produces<std::vector<bool> > ("istracker" ).setBranchAlias("is_tracker" );
    produces<std::vector<bool> > ("issa"      ).setBranchAlias("is_sa"      );
    produces<std::vector<bool> > ("ispf"      ).setBranchAlias("is_pf"      );

    produces<std::vector<LorentzVector> > ("p4").setBranchAlias("p4");

    produces<std::vector<double> > ("dz").setBranchAlias("dz");
    produces<std::vector<double> > ("d0").setBranchAlias("d0");
    produces<std::vector<double> > ("dzbs").setBranchAlias("dz_bs");
    produces<std::vector<double> > ("d0bs").setBranchAlias("d0_bs");
    produces<std::vector<double> > ("gfitchi2").setBranchAlias("gfit_chi2");
    produces<std::vector<double> > ("gfitndof").setBranchAlias("gfit_ndof");
    produces<std::vector<double> > ("maxOpeningAngleTrack").setBranchAlias("maxOpeningAngleTrack");
    produces<std::vector<double> > ("maxOpeningAngleMuon").setBranchAlias("maxOpeningAngleMuon");    

    produces<std::vector<int> > ("type").setBranchAlias("type");
    produces<std::vector<int> > ("validsiLayers").setBranchAlias("valid_siLayers");
    produces<std::vector<int> > ("validpixelHits").setBranchAlias("valid_pixelHits");    
    produces<std::vector<int> > ("charge").setBranchAlias("charge");
    produces<std::vector<int> > ("trkcharge").setBranchAlias("trk_charge");
    produces<std::vector<int> > ("gfitvalidSTAhits").setBranchAlias("gfit_validSTAhits");
    produces<std::vector<int> > ("gfitvalidSiHits").setBranchAlias("gfit_validSiHits");
    produces<std::vector<int> > ("stavalidHits").setBranchAlias("sta_validHits");
    produces<std::vector<int> > ("numMatchedStations").setBranchAlias("numMatchedStations");
    produces<std::vector<int> > ("numMatchedCSCsegments").setBranchAlias("numMatchedCSCsegments");
    produces<std::vector<int> > ("stationmask").setBranchAlias("station_mask");

    //
    // chamber level quantities
    //
    produces<std::vector<std::vector<double> > > ("distToChamberEdge").setBranchAlias("distToChamberEdge");
    produces<std::vector<std::vector<double> > > ("distToChamberEdgeErr").setBranchAlias("distToChamberEdgeErr");    
    produces<std::vector<std::vector<int> > > ("numCSCsegmentsInChamber").setBranchAlias("numCSCsegmentsInChamber");
    
    //
    // segment level quantities
    //
    produces<std::vector<std::vector<std::vector<unsigned int> > > > ("arbitrationmask").setBranchAlias("arbitration_mask");
    produces<std::vector<std::vector<std::vector<bool> > > > ("isSegmentAndTrackArbitrated").setBranchAlias("isSegmentAndTrackArbitrated");
    produces<std::vector<std::vector<std::vector<int> > > > ("numRecHitsInSegment").setBranchAlias("numRecHitsInSegment");
    
    //
    // rechit level quantities
    //
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("endcap"            ).setBranchAlias("endcap"            );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("station"           ).setBranchAlias("station"           );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("ring"              ).setBranchAlias("ring"              );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("chamber"           ).setBranchAlias("chamber"           );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("layer"             ).setBranchAlias("layer"             );

    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("rhtime"            ).setBranchAlias("rhtime"            );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("twire").setBranchAlias("twire");
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("nstrips"             ).setBranchAlias("nstrips"             );

    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("cfebcablelength"   ).setBranchAlias("cfeb_cable_length" );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("cfebskewdelay"     ).setBranchAlias("cfeb_skew_delay"   );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("cfebtimingcorr"    ).setBranchAlias("cfeb_timing_corr"  );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("cfebcabledelay"    ).setBranchAlias("cfeb_cable_delay"  );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("precision"         ).setBranchAlias("precision"         );    
    produces<std::vector<std::vector<std::vector<std::vector<char> > > > >   ("cfebcablerev"      ).setBranchAlias("cfeb_cable_rev"    );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("anodebxoffset").setBranchAlias("anode_bx_offset");

    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("chipCorr"          ).setBranchAlias("chipCorr"          );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("cfebCorr"          ).setBranchAlias("cfebCorr"          );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("skewClearDelay"    ).setBranchAlias("skewClearDelay"    );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("cfebCableDelay"    ).setBranchAlias("cfebCableDelay"    );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("chamberCorr"       ).setBranchAlias("chamberCorr"       ); // cfebCorr + skewClearDelay + cfebCableDelay

    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("newskewClearDelay"    ).setBranchAlias("new_skewClearDelay"    );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("newcfebCableDelay"    ).setBranchAlias("new_cfebCableDelay"    );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("newchamberCorr"       ).setBranchAlias("new_chamberCorr"       ); // cfebCorr + skewClearDelay + cfebCableDelay
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("newcfebcablelength"   ).setBranchAlias("new_cfeb_cable_length" );
    produces<std::vector<std::vector<std::vector<std::vector<char> > > > >   ("newcfebcablerev"      ).setBranchAlias("new_cfeb_cable_rev"    );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("newcfebskewdelay"     ).setBranchAlias("new_cfeb_skew_delay"   );
    produces<std::vector<std::vector<std::vector<std::vector<int> > > > >    ("newcfebcabledelay"    ).setBranchAlias("new_cfeb_cable_delay"  );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("newcfebCorr"          ).setBranchAlias("new_cfebCorr"          );
    produces<std::vector<std::vector<std::vector<std::vector<double> > > > > ("newchipCorr"          ).setBranchAlias("new_chipCorr"          );
}


CSCTimingBabyMaker::~CSCTimingBabyMaker()
{
 
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CSCTimingBabyMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    //
    // event level quantities
    //
    std::auto_ptr<int> run  (new int);
    std::auto_ptr<int> lumi (new int);
    std::auto_ptr<unsigned long long> evt (new unsigned long long);
    std::auto_ptr<double> bfield (new double);
    std::auto_ptr<bool> csc_status (new bool);     
    std::auto_ptr<unsigned int> detector_status (new unsigned int);    
       
    //
    // muon level quantities
    //
    std::auto_ptr<std::vector<bool> > is_global  (new std::vector<bool>);
    std::auto_ptr<std::vector<bool> > is_tracker (new std::vector<bool>);
    std::auto_ptr<std::vector<bool> > is_sa      (new std::vector<bool>);
    std::auto_ptr<std::vector<bool> > is_pf      (new std::vector<bool>);

    std::auto_ptr<std::vector<LorentzVector> > p4 (new std::vector<LorentzVector>);

    std::auto_ptr<std::vector<double> > dz (new std::vector<double>);
    std::auto_ptr<std::vector<double> > d0 (new std::vector<double>);
    std::auto_ptr<std::vector<double> > dz_bs (new std::vector<double>);
    std::auto_ptr<std::vector<double> > d0_bs (new std::vector<double>);
    std::auto_ptr<std::vector<double> > gfit_chi2 (new std::vector<double>);
    std::auto_ptr<std::vector<double> > gfit_ndof (new std::vector<double>);
    std::auto_ptr<std::vector<double> > maxOpeningAngleTrack (new std::vector<double>);
    std::auto_ptr<std::vector<double> > maxOpeningAngleMuon (new std::vector<double>);

    std::auto_ptr<std::vector<int> > type (new std::vector<int>);
    std::auto_ptr<std::vector<int> > valid_siLayers (new std::vector<int>);
    std::auto_ptr<std::vector<int> > valid_pixelHits (new std::vector<int>);
    std::auto_ptr<std::vector<int> > charge (new std::vector<int>);
    std::auto_ptr<std::vector<int> > trk_charge (new std::vector<int>);
    std::auto_ptr<std::vector<int> > gfit_validSTAhits (new std::vector<int>);
    std::auto_ptr<std::vector<int> > gfit_validSiHits (new std::vector<int>);
    std::auto_ptr<std::vector<int> > sta_validHits (new std::vector<int>);
    std::auto_ptr<std::vector<int> > numMatchedStations (new std::vector<int>);
    std::auto_ptr<std::vector<int> > numMatchedCSCsegments (new std::vector<int>);
    std::auto_ptr<std::vector<int> > station_mask (new std::vector<int>);

    //
    // chamber level quantities
    //
    std::auto_ptr<std::vector<std::vector<double> > > distToChamberEdge (new std::vector<std::vector<double> >);
    std::auto_ptr<std::vector<std::vector<double> > > distToChamberEdgeErr (new std::vector<std::vector<double> >);
    std::auto_ptr<std::vector<std::vector<int> > > numCSCsegmentsInChamber (new std::vector<std::vector<int> >);

    //
    // segment level quantities
    //
    std::auto_ptr<std::vector<std::vector<std::vector<unsigned int> > > > arbitration_mask (new std::vector<std::vector<std::vector<unsigned int> > > );
    std::auto_ptr<std::vector<std::vector<std::vector<bool> > > > isSegmentAndTrackArbitrated (new std::vector<std::vector<std::vector<bool> > > );
    std::auto_ptr<std::vector<std::vector<std::vector<int> > > > numRecHitsInSegment (new std::vector<std::vector<std::vector<int> > > );
    
    //
    // rechit level quantities
    //
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    endcap                (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    station               (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    ring                  (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    chamber               (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    layer                 (new std::vector<std::vector<std::vector<std::vector<int> > > >    );

    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > rhtime                (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > twire                 (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    nstrips               (new std::vector<std::vector<std::vector<std::vector<int> > > >    );

    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_cable_length     (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_skew_delay       (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_timing_corr      (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_cable_delay      (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    precision             (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<char> > > > >   cfeb_cable_rev        (new std::vector<std::vector<std::vector<std::vector<char> > > >   );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > anode_bx_offset       (new std::vector<std::vector<std::vector<std::vector<double> > > > );    

    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > chipCorr              (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > cfebCorr              (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > skewClearDelay        (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > cfebCableDelay        (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > chamberCorr           (new std::vector<std::vector<std::vector<std::vector<double> > > > );

    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > new_skewClearDelay    (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > new_cfebCableDelay    (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > new_chamberCorr       (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_cable_length (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<char> > > > >   new_cfeb_cable_rev    (new std::vector<std::vector<std::vector<std::vector<char> > > >   );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_skew_delay   (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_cable_delay  (new std::vector<std::vector<std::vector<std::vector<int> > > >    );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > new_cfebCorr          (new std::vector<std::vector<std::vector<std::vector<double> > > > );
    std::auto_ptr<std::vector<std::vector<std::vector<std::vector<double> > > > > new_chipCorr          (new std::vector<std::vector<std::vector<std::vector<double> > > > );    

    // edm::Handle<edm::TriggerResults> trigResults_h;
    // iEvent.getByToken(trgResults_token, trigResults_h);
    // if (trigResults_h.isValid())
    // {
    //     edm::TriggerNames trigNames = iEvent.triggerNames(*trigResults_h);
    //     std::vector<std::string> vtrigNames = trigNames.triggerNames();
    //     std::cout << std::endl;
    //     for (auto name : vtrigNames)
    //         std::cout << name << std::endl;
    //     std::cout << std::endl;
    // }
    
    //
    // load chamber timing corrections
    //
    if (!isLoaded_)
    {
        recoConditions->initializeEvent(iSetup);
        iSetup.get<CSCChamberTimeCorrectionsRcd>().get(theChamberTimingCorrections);
        iSetup.get<CSCIndexerRecord>().get(indexer);
        iSetup.get<CSCChannelMapperRecord>().get(mapper);
        isLoaded_ = true;
    }    

    //
    // fill event level quantities
    //
    *run    = iEvent.id().run();
    *lumi   = iEvent.luminosityBlock();
    *evt    = iEvent.id().event();
    *csc_status = false;
    *detector_status = false;
    
    edm::Handle<DcsStatusCollection> dcsHandle;
    iEvent.getByLabel("scalersRawToDigi", dcsHandle);
    
    const double currentToBFieldScaleFactor = 2.09237036221512717e-04;
    double current = -9999/currentToBFieldScaleFactor;

    if( dcsHandle.isValid() && (*dcsHandle).size() > 0 )
    {
        current = (*dcsHandle)[0].magnetCurrent();
        *csc_status = (*dcsHandle)[0].ready(DcsStatus::CSCp) & (*dcsHandle)[0].ready(DcsStatus::CSCm);
        *detector_status = (*dcsHandle)[0].ready();
    }	 

    *bfield = current*currentToBFieldScaleFactor;    
    
    edm::Handle<reco::VertexCollection> vtx_h;
    iEvent.getByToken(vtx_token, vtx_h);
    const reco::VertexCollection *vtx_coll = vtx_h.product();
    reco::Vertex vtx = vtx_coll->front();
    if (!isGoodVertex(vtx)) return;

    edm::Handle<reco::BeamSpot> bs_h;
    iEvent.getByToken(bs_token, bs_h);
    if (!bs_h.isValid()) return;

    edm::Handle<reco::TrackCollection> trks_h;
    iEvent.getByToken(trk_token, trks_h);
    reco::TrackCollection::const_iterator trkIter;

    edm::Handle<reco::MuonCollection> mus_h;
    iEvent.getByToken(mu_token, mus_h);
    if (!mus_h.isValid() || mus_h->size() == 0) return;

    int nGoodMus_ = 0;
    reco::MuonCollection::const_iterator muIter;
    reco::MuonCollection::const_iterator muIter2;
    for (muIter = mus_h->begin(); muIter != mus_h->end(); muIter++)
    {        
        //
        // chamber level quantities
        //
        std::vector<double> v_distToChamberEdge;
        std::vector<double> v_distToChamberEdgeErr;
        std::vector<int> v_numCSCsegmentsInChamber;
        
        //
        // segment level quantities
        //
        std::vector<std::vector<unsigned int> > vv_arbitration_mask;
        std::vector<std::vector<bool> > vv_isSegmentAndTrackArbitrated;
        std::vector<std::vector<int> > vv_numRecHitsInSegment;
        
        //
        // rechit level quantities
        //
        std::vector<std::vector<std::vector<int> > >    vvv_endcap           ;
        std::vector<std::vector<std::vector<int> > >    vvv_station          ;
        std::vector<std::vector<std::vector<int> > >    vvv_ring             ;
        std::vector<std::vector<std::vector<int> > >    vvv_chamber          ;
        std::vector<std::vector<std::vector<int> > >    vvv_layer            ;

        std::vector<std::vector<std::vector<double> > > vvv_rhtime           ;
        std::vector<std::vector<std::vector<double> > > vvv_twire            ;
        std::vector<std::vector<std::vector<int> > >    vvv_nstrips          ;

        std::vector<std::vector<std::vector<int> > >    vvv_cfeb_cable_length;
        std::vector<std::vector<std::vector<int> > >    vvv_cfeb_skew_delay  ;
        std::vector<std::vector<std::vector<int> > >    vvv_cfeb_timing_corr ;
        std::vector<std::vector<std::vector<int> > >    vvv_cfeb_cable_delay ;
        std::vector<std::vector<std::vector<int> > >    vvv_precision        ;
        std::vector<std::vector<std::vector<char> > >   vvv_cfeb_cable_rev   ;
        std::vector<std::vector<std::vector<double> > > vvv_anode_bx_offset  ;

        std::vector<std::vector<std::vector<double> > > vvv_chipCorr         ;
        std::vector<std::vector<std::vector<double> > > vvv_cfebCorr         ;
        std::vector<std::vector<std::vector<double> > > vvv_skewClearDelay   ;
        std::vector<std::vector<std::vector<double> > > vvv_cfebCableDelay   ;
        std::vector<std::vector<std::vector<double> > > vvv_chamberCorr      ;

        std::vector<std::vector<std::vector<double> > > vvv_new_skewClearDelay   ;
        std::vector<std::vector<std::vector<double> > > vvv_new_cfebCableDelay   ;
        std::vector<std::vector<std::vector<double> > > vvv_new_chamberCorr      ;
        std::vector<std::vector<std::vector<int> > >    vvv_new_cfeb_cable_length;
        std::vector<std::vector<std::vector<char> > >   vvv_new_cfeb_cable_rev   ;
        std::vector<std::vector<std::vector<int> > >    vvv_new_cfeb_skew_delay  ;
        std::vector<std::vector<std::vector<int> > >    vvv_new_cfeb_cable_delay ;
        std::vector<std::vector<std::vector<double> > > vvv_new_cfebCorr         ;
        std::vector<std::vector<std::vector<double> > > vvv_new_chipCorr         ;

        
        int numMatchedCSCsegments_ = 0;
        int numCSCrhs_ = 0;
        std::vector<reco::MuonChamberMatch> chamber_matches = muIter->matches();
        if (chamber_matches.size() == 0) continue;
        std::vector<reco::MuonChamberMatch>::const_iterator chIter;
        for (chIter = chamber_matches.begin(); chIter != chamber_matches.end(); chIter++)
        {
            int numCSCsegmentsInChamber_ = 0;
            
            std::vector<reco::MuonSegmentMatch> segment_matches = chIter->segmentMatches;
            if (segment_matches.size() == 0) continue;

            //
            // segment level quantities
            //
            std::vector<unsigned int> v_arbitration_mask;
            std::vector<bool> v_isSegmentAndTrackArbitrated;
            std::vector<int> v_numRecHitsInSegment;

        
            //
            // rechit level quantities
            //
            std::vector<std::vector<int> >    vv_endcap           ;
            std::vector<std::vector<int> >    vv_station          ;
            std::vector<std::vector<int> >    vv_ring             ;
            std::vector<std::vector<int> >    vv_chamber          ;
            std::vector<std::vector<int> >    vv_layer            ;

            std::vector<std::vector<double> > vv_rhtime           ;
            std::vector<std::vector<double> > vv_twire            ;
            std::vector<std::vector<int> >    vv_nstrips          ;

            std::vector<std::vector<int> >    vv_cfeb_cable_length;
            std::vector<std::vector<int> >    vv_cfeb_skew_delay  ;
            std::vector<std::vector<int> >    vv_cfeb_timing_corr ;
            std::vector<std::vector<int> >    vv_cfeb_cable_delay ;
            std::vector<std::vector<int> >    vv_precision        ;
            std::vector<std::vector<char> >   vv_cfeb_cable_rev   ;
            std::vector<std::vector<double> > vv_anode_bx_offset ;

            std::vector<std::vector<double> > vv_chipCorr         ;
            std::vector<std::vector<double> > vv_cfebCorr         ;
            std::vector<std::vector<double> > vv_skewClearDelay   ;
            std::vector<std::vector<double> > vv_cfebCableDelay   ;
            std::vector<std::vector<double> > vv_chamberCorr      ;

            std::vector<std::vector<double> > vv_new_skewClearDelay   ;
            std::vector<std::vector<double> > vv_new_cfebCableDelay   ;
            std::vector<std::vector<double> > vv_new_chamberCorr      ;
            std::vector<std::vector<int> >    vv_new_cfeb_cable_length;
            std::vector<std::vector<char> >   vv_new_cfeb_cable_rev   ;
            std::vector<std::vector<int> >    vv_new_cfeb_skew_delay  ;
            std::vector<std::vector<int> >    vv_new_cfeb_cable_delay ;
            std::vector<std::vector<double> > vv_new_cfebCorr         ;
            std::vector<std::vector<double> > vv_new_chipCorr         ;


            std::vector<reco::MuonSegmentMatch>::const_iterator segIter;
            for (segIter = segment_matches.begin(); segIter != segment_matches.end(); segIter++)
            {
                CSCSegmentRef segRef = segIter->cscSegmentRef;
                if (!segRef.isAvailable() || segRef.isNull() || segRef.get() == 0) continue;
                if (segRef->nRecHits() == 0) continue;
                ++numCSCsegmentsInChamber_;

                //
                // rechit level quantities
                //
                std::vector<int>    v_endcap           ;
                std::vector<int>    v_station          ;
                std::vector<int>    v_ring             ;
                std::vector<int>    v_chamber          ;
                std::vector<int>    v_layer            ;

                std::vector<double> v_rhtime           ;
                std::vector<double> v_twire            ;
                std::vector<int>    v_nstrips          ;

                std::vector<int>    v_cfeb_cable_length;
                std::vector<int>    v_cfeb_skew_delay  ;
                std::vector<int>    v_cfeb_timing_corr ;
                std::vector<int>    v_cfeb_cable_delay ;
                std::vector<int>    v_precision        ;
                std::vector<char>   v_cfeb_cable_rev   ;
                std::vector<double> v_anode_bx_offset ;

                std::vector<double> v_chipCorr         ;
                std::vector<double> v_cfebCorr         ;
                std::vector<double> v_skewClearDelay   ;
                std::vector<double> v_cfebCableDelay   ;
                std::vector<double> v_chamberCorr      ;

                std::vector<double> v_new_skewClearDelay   ;
                std::vector<double> v_new_cfebCableDelay   ;
                std::vector<double> v_new_chamberCorr      ;
                std::vector<int>    v_new_cfeb_cable_length;
                std::vector<char>   v_new_cfeb_cable_rev   ;
                std::vector<int>    v_new_cfeb_skew_delay  ;
                std::vector<int>    v_new_cfeb_cable_delay ;
                std::vector<double> v_new_cfebCorr         ;
                std::vector<double> v_new_chipCorr         ;                    

                const CSCSegment* segment = segRef.get();
                std::vector<CSCRecHit2D> rechits = segment->specificRecHits();
                std::vector<CSCRecHit2D>::const_iterator rhIter;
                numCSCrhs_ += rechits.size();
                for (rhIter = rechits.begin(); rhIter != rechits.end(); rhIter++)
                {                                        
                    CSCDetId id = (CSCDetId)rhIter->cscDetId();
                    v_endcap  .push_back(id.endcap()       );
                    v_station .push_back(id.station()      );
                    v_ring    .push_back(id.ring()         );
                    v_chamber .push_back(id.chamber()      );
                    v_layer   .push_back(id.layer()        );        
                    v_rhtime  .push_back(rhIter->tpeak()   );
                    v_twire   .push_back(rhIter->wireTime());
                    v_nstrips .push_back(rhIter->nStrips());

                    if (debug_)
                    {
                        if (muIter->isGlobalMuon() && segIter->isMask(reco::MuonSegmentMatch::BestInChamberByDR) && segIter->isMask(reco::MuonSegmentMatch::BelongsToTrackByDR))
                        {
                            if (id.ring() == 1 && (id.station() == 3 || id.station() == 4))
                                printf("endcap,station,ring,chamber: %d, %d, %d, %d\n", id.endcap(), id.station(), id.ring(), id.chamber());
                            if (id.endcap() ==2 && id.station() == 4 && id.ring() == 2)
                                printf("endcap,station,ring,chamber: %d, %d, %d, %d\n", id.endcap(), id.station(), id.ring(), id.chamber());
                        }
                    }

                    CSCDetId rawid = mapper->rawCSCDetId(id);
                    int index = indexer->chamberIndex(rawid)-1;
                    v_cfeb_cable_length.push_back(theChamberTimingCorrections->item(index).cfeb_length         );
                    v_cfeb_cable_rev   .push_back(theChamberTimingCorrections->item(index).cfeb_rev            );
                    v_cfeb_skew_delay  .push_back(theChamberTimingCorrections->item(index).cfeb_tmb_skew_delay );
                    v_cfeb_timing_corr .push_back(theChamberTimingCorrections->item(index).cfeb_timing_corr    );
                    v_cfeb_cable_delay .push_back(theChamberTimingCorrections->item(index).cfeb_cable_delay    );
                    v_anode_bx_offset  .push_back(theChamberTimingCorrections->item(index).anode_bx_offset     );

                    int precision_ = theChamberTimingCorrections->precision();
                    v_precision.push_back(precision_);
                        
                    v_cfebCorr      .push_back(v_cfeb_timing_corr.back() * 1./precision_    );
                    v_skewClearDelay.push_back(v_cfeb_skew_delay.back() * 1./precision_     );
                    v_cfebCableDelay.push_back(v_cfeb_cable_delay.back() * 25.              );
                    v_chamberCorr   .push_back(recoConditions->chamberTimingCorrection(id) );
                    
                    unsigned int idCenterStrip = rhIter->nStrips() / 2;
                    if (rhIter->nStrips() > 0)
                    {
                        int centerStrip = rhIter->channels(idCenterStrip);
                        v_chipCorr.push_back(recoConditions->chipCorrection(id, centerStrip));
                    }
                    else
                    {
                        v_chipCorr.push_back(CSCTimingBabyMaker::AVG_CHIP_CORR);
                    }

                    if (id.station() == 1 && (id.ring() == 1 || id.ring() == 4))
                    {
                        v_new_cfeb_cable_length.push_back(CSCTimingBabyMaker::getSkewClearCableLengthForME11(id));
                        v_new_cfeb_cable_rev   .push_back('D');
                        v_new_cfeb_cable_delay .push_back(0);
                        v_new_cfebCorr        .push_back(v_cfeb_timing_corr.back() * 1./precision_);
                        int new_skew_delay_ = CSCTimingBabyMaker::getSkewClearCableDelayForME11(id);
                        v_new_cfeb_skew_delay .push_back(new_skew_delay_);                // includes PPIB-ODMB cable + PPIB-DCFEB cable + PPIB_LVDS delays (in 10 ps unit)           
                        v_new_skewClearDelay  .push_back(new_skew_delay_*1./v_precision.back());          // includes PPIB-ODMB cable + PPIB-DCFEB cable + PPIB_LVDS delays            
                        v_new_cfebCableDelay  .push_back(0);
                        v_new_chamberCorr     .push_back(v_new_cfebCorr.back() + v_new_skewClearDelay.back() + v_new_cfebCableDelay.back());
                        v_new_chipCorr.push_back(CSCTimingBabyMaker::AVG_CHIP_CORR);
                    }
                    else
                    {
                        v_new_cfeb_cable_length .push_back(v_cfeb_cable_length.back() );
                        v_new_cfeb_cable_rev    .push_back(v_cfeb_cable_rev.back()    );
                        v_new_cfeb_cable_delay  .push_back(v_cfeb_cable_delay.back()  );
                        v_new_cfebCorr          .push_back(v_cfebCorr.back()          );
                        v_new_cfeb_skew_delay   .push_back(v_cfeb_skew_delay.back()   );
                        v_new_skewClearDelay    .push_back(v_skewClearDelay.back()    );
                        v_new_cfebCableDelay    .push_back(v_cfebCableDelay.back()    );
                        v_new_chamberCorr       .push_back(v_chamberCorr.back()       );
                        v_new_chipCorr           .push_back(v_chipCorr.back()         );
                    }
                } // end loop over rechits

                v_arbitration_mask.push_back(segIter->mask);

                bool isSegmentAndTrackArbitrated_ = segIter->isMask(reco::MuonSegmentMatch::BestInChamberByDR) && segIter->isMask(reco::MuonSegmentMatch::BelongsToTrackByDR);
                v_isSegmentAndTrackArbitrated.push_back(isSegmentAndTrackArbitrated_);
                if (isSegmentAndTrackArbitrated_) ++numMatchedCSCsegments_;

                v_numRecHitsInSegment.push_back(rechits.size());                

                vv_endcap                .push_back(v_endcap               );
                vv_station               .push_back(v_station              );
                vv_ring                  .push_back(v_ring                 );
                vv_chamber               .push_back(v_chamber              );
                vv_layer                 .push_back(v_layer                );
                                                                            
                vv_rhtime                .push_back(v_rhtime               );
                vv_twire                 .push_back(v_twire                );
                vv_nstrips               .push_back(v_nstrips              );
                                                                            
                vv_cfeb_cable_length     .push_back(v_cfeb_cable_length    );
                vv_cfeb_skew_delay       .push_back(v_cfeb_skew_delay      );
                vv_cfeb_timing_corr      .push_back(v_cfeb_timing_corr     );
                vv_cfeb_cable_delay      .push_back(v_cfeb_cable_delay     );
                vv_precision             .push_back(v_precision            );
                vv_cfeb_cable_rev        .push_back(v_cfeb_cable_rev       );
                vv_anode_bx_offset       .push_back(v_anode_bx_offset      );
                                                                            
                vv_chipCorr              .push_back(v_chipCorr             );
                vv_cfebCorr              .push_back(v_cfebCorr             );
                vv_skewClearDelay        .push_back(v_skewClearDelay       );
                vv_cfebCableDelay        .push_back(v_cfebCableDelay       );
                vv_chamberCorr           .push_back(v_chamberCorr          );
                                                                            
                vv_new_skewClearDelay    .push_back(v_new_skewClearDelay   );
                vv_new_cfebCableDelay    .push_back(v_new_cfebCableDelay   );
                vv_new_chamberCorr       .push_back(v_new_chamberCorr      );
                vv_new_cfeb_cable_length .push_back(v_new_cfeb_cable_length);
                vv_new_cfeb_cable_rev    .push_back(v_new_cfeb_cable_rev   );
                vv_new_cfeb_skew_delay   .push_back(v_new_cfeb_skew_delay  );
                vv_new_cfeb_cable_delay  .push_back(v_new_cfeb_cable_delay );
                vv_new_cfebCorr          .push_back(v_new_cfebCorr         );                
                vv_new_chipCorr          .push_back(v_new_chipCorr         );                

            } // end loop over segment matches            
            
            if (numCSCsegmentsInChamber_ == 0) continue;

            v_distToChamberEdge.push_back(chIter->dist());
            v_distToChamberEdgeErr.push_back(chIter->distErr());
            v_numCSCsegmentsInChamber.push_back(numCSCsegmentsInChamber_);

            vv_arbitration_mask.push_back(v_arbitration_mask);
            vv_isSegmentAndTrackArbitrated.push_back(v_isSegmentAndTrackArbitrated);           
            vv_numRecHitsInSegment.push_back(v_numRecHitsInSegment);                

            vvv_endcap                .push_back(vv_endcap               );
            vvv_station               .push_back(vv_station              );
            vvv_ring                  .push_back(vv_ring                 );
            vvv_chamber               .push_back(vv_chamber              );
            vvv_layer                 .push_back(vv_layer                );
                                                                            
            vvv_rhtime                .push_back(vv_rhtime               );
            vvv_twire                 .push_back(vv_twire                );
            vvv_nstrips               .push_back(vv_nstrips              );
                                                                            
            vvv_cfeb_cable_length     .push_back(vv_cfeb_cable_length    );
            vvv_cfeb_skew_delay       .push_back(vv_cfeb_skew_delay      );
            vvv_cfeb_timing_corr      .push_back(vv_cfeb_timing_corr     );
            vvv_cfeb_cable_delay      .push_back(vv_cfeb_cable_delay     );
            vvv_precision             .push_back(vv_precision            );
            vvv_cfeb_cable_rev        .push_back(vv_cfeb_cable_rev       );
            vvv_anode_bx_offset       .push_back(vv_anode_bx_offset      );
                                                                            
            vvv_chipCorr              .push_back(vv_chipCorr             );
            vvv_cfebCorr              .push_back(vv_cfebCorr             );
            vvv_skewClearDelay        .push_back(vv_skewClearDelay       );
            vvv_cfebCableDelay        .push_back(vv_cfebCableDelay       );
            vvv_chamberCorr           .push_back(vv_chamberCorr          );
                                                                            
            vvv_new_skewClearDelay    .push_back(vv_new_skewClearDelay   );
            vvv_new_cfebCableDelay    .push_back(vv_new_cfebCableDelay   );
            vvv_new_chamberCorr       .push_back(vv_new_chamberCorr      );
            vvv_new_cfeb_cable_length .push_back(vv_new_cfeb_cable_length);
            vvv_new_cfeb_cable_rev    .push_back(vv_new_cfeb_cable_rev   );
            vvv_new_cfeb_skew_delay   .push_back(vv_new_cfeb_skew_delay  );
            vvv_new_cfeb_cable_delay  .push_back(vv_new_cfeb_cable_delay );
            vvv_new_cfebCorr          .push_back(vv_new_cfebCorr         );                
            vvv_new_chipCorr          .push_back(vv_new_chipCorr         );                

        } // end loop over chamber matches

        if (numCSCrhs_ == 0) continue;
        ++nGoodMus_;
               
        numMatchedCSCsegments->push_back(numMatchedCSCsegments_);

        //
        // fill muon level quantities
        //    
        is_global  ->push_back(muIter->isGlobalMuon());
        is_tracker ->push_back(muIter->isTrackerMuon());
        is_sa      ->push_back(muIter->isStandAloneMuon());
        is_pf      ->push_back(muIter->isPFMuon());        
        type       ->push_back(muIter->type());

        p4->push_back(LorentzVector(muIter->p4()));

        charge->push_back(muIter->charge());

        const reco::TrackRef siTrack = muIter->innerTrack();
        const reco::TrackRef globalTrack = muIter->globalTrack();
        const reco::TrackRef staTrack = muIter->outerTrack();

        double max_angle = -999.;
        for (muIter2 = mus_h->begin(); muIter2 != mus_h->end(); muIter2++)        
        {
            if (muIter == muIter2) continue;            
            // if (muIter2->p4().pt() < 10.) continue;
            const reco::TrackRef siTrack2 = muIter2->innerTrack();
            const reco::TrackRef globalTrack2 = muIter2->globalTrack();
            const reco::TrackRef staTrack2 = muIter2->outerTrack();
            
            if (siTrack.isNonnull())
            {
                if (siTrack2.isNonnull())
                {
                    if (fabs(angle(siTrack->momentum(), siTrack2->momentum())) > max_angle) max_angle = angle(siTrack->momentum(), siTrack2->momentum());
                }
                else
                {
                    if (fabs(angle(siTrack->momentum(), muIter2->momentum())) > max_angle) max_angle = angle(siTrack->momentum(), muIter2->momentum());                    
                }
            }
            else
            {
                if (siTrack2.isNonnull())
                {
                    if (fabs(angle(muIter->momentum(), siTrack2->momentum())) > max_angle) max_angle = angle(muIter->momentum(), siTrack2->momentum());
                }
                else
                {
                    if (fabs(angle(muIter->momentum(), muIter2->momentum())) > max_angle) max_angle = angle(muIter->momentum(), muIter2->momentum());                    
                }
            }
        }        

        maxOpeningAngleMuon->push_back(max_angle);

        max_angle = -999.;
        for (trkIter = trks_h->begin(); trkIter != trks_h->end(); trkIter++)
        {
            // if (trkIter->pt() < 10.) continue;
            if (!trkIter->quality(reco::TrackBase::TrackQuality::highPurity)) continue;

            if (siTrack.isNonnull())
            {
                if (fabs(angle(siTrack->momentum(), trkIter->momentum())) > max_angle) max_angle = angle(siTrack->momentum(), trkIter->momentum());
            }
            else
            {
                if (fabs(angle(muIter->momentum(), trkIter->momentum())) > max_angle) max_angle = angle(muIter->momentum(), trkIter->momentum());
            }            
        }

        maxOpeningAngleTrack->push_back(max_angle);
        
        if (siTrack.isNonnull())
        {
            dz ->push_back(  siTrack->dz(vtx.position()));
            d0 ->push_back( siTrack->dxy(vtx.position()));
            dz_bs ->push_back(  siTrack->dz(bs_h->position()));
            d0_bs ->push_back( siTrack->dxy(bs_h->position()));
        }
        else if (globalTrack.isNonnull())
        {
            d0 ->push_back(globalTrack->dxy(vtx.position()));
            dz ->push_back(globalTrack->dz(vtx.position()));
            dz_bs ->push_back(globalTrack->dz(bs_h->position()));
            d0_bs ->push_back(globalTrack->dxy(bs_h->position()));
        }
        else if (staTrack.isNonnull())
        {
            d0 ->push_back(staTrack->dxy(vtx.position()));
            dz ->push_back(staTrack->dz(vtx.position()));
            dz_bs ->push_back(staTrack->dz(bs_h->position()));
            d0_bs ->push_back(staTrack->dxy(bs_h->position()));
        }
        else
        {
            d0->push_back(-9999.);
            dz->push_back(-9999.);
            d0_bs->push_back(-9999.);
            dz_bs->push_back(-9999.);
        }

        if (globalTrack.isNonnull())
        {
            gfit_chi2 ->push_back( globalTrack->chi2());
            gfit_ndof ->push_back( globalTrack->ndof());
            gfit_validSTAhits ->push_back( globalTrack->hitPattern().numberOfValidMuonHits());
            gfit_validSiHits ->push_back( globalTrack->hitPattern().numberOfValidTrackerHits());
        }
        else
        {
            gfit_chi2 ->push_back(-9999.);
            gfit_ndof ->push_back(-9999.);
            gfit_validSTAhits ->push_back(0);
            gfit_validSiHits ->push_back(0);
        }

        if (siTrack.isNonnull())
        {
            valid_siLayers ->push_back( siTrack->hitPattern().trackerLayersWithMeasurement());
            valid_pixelHits ->push_back( siTrack->hitPattern().pixelLayersWithMeasurement());
            trk_charge ->push_back( siTrack->charge());
        }
        else
        {
            valid_siLayers ->push_back(0);
            valid_pixelHits ->push_back(0);
            trk_charge ->push_back(0);
        }

        if (staTrack.isNonnull())
        {
            sta_validHits ->push_back( staTrack->numberOfValidHits());
        }
        else
        {
            sta_validHits ->push_back(0);
        }

        numMatchedStations ->push_back( muIter->numberOfMatchedStations());
        station_mask ->push_back( muIter->stationMask());

        distToChamberEdge->push_back(v_distToChamberEdge);
        distToChamberEdgeErr->push_back(v_distToChamberEdgeErr);
        numCSCsegmentsInChamber->push_back(v_numCSCsegmentsInChamber);

        arbitration_mask->push_back(vv_arbitration_mask);
        isSegmentAndTrackArbitrated->push_back(vv_isSegmentAndTrackArbitrated);
        numRecHitsInSegment->push_back(vv_numRecHitsInSegment);

        endcap                ->push_back(vvv_endcap               );
        station               ->push_back(vvv_station              );
        ring                  ->push_back(vvv_ring                 );
        chamber               ->push_back(vvv_chamber              );
        layer                 ->push_back(vvv_layer                );

        rhtime                ->push_back(vvv_rhtime               );
        twire                 ->push_back(vvv_twire                );
        nstrips               ->push_back(vvv_nstrips              );

        cfeb_cable_length     ->push_back(vvv_cfeb_cable_length    );
        cfeb_skew_delay       ->push_back(vvv_cfeb_skew_delay      );
        cfeb_timing_corr      ->push_back(vvv_cfeb_timing_corr     );
        cfeb_cable_delay      ->push_back(vvv_cfeb_cable_delay     );
        precision             ->push_back(vvv_precision            );
        cfeb_cable_rev        ->push_back(vvv_cfeb_cable_rev       );
        anode_bx_offset       ->push_back(vvv_anode_bx_offset      );

        chipCorr              ->push_back(vvv_chipCorr             );
        cfebCorr              ->push_back(vvv_cfebCorr             );
        skewClearDelay        ->push_back(vvv_skewClearDelay       );
        cfebCableDelay        ->push_back(vvv_cfebCableDelay       );
        chamberCorr           ->push_back(vvv_chamberCorr          );

        new_skewClearDelay    ->push_back(vvv_new_skewClearDelay   );
        new_cfebCableDelay    ->push_back(vvv_new_cfebCableDelay   );
        new_chamberCorr       ->push_back(vvv_new_chamberCorr      );
        new_cfeb_cable_length ->push_back(vvv_new_cfeb_cable_length);
        new_cfeb_cable_rev    ->push_back(vvv_new_cfeb_cable_rev   );
        new_cfeb_skew_delay   ->push_back(vvv_new_cfeb_skew_delay  );
        new_cfeb_cable_delay  ->push_back(vvv_new_cfeb_cable_delay );
        new_cfebCorr          ->push_back(vvv_new_cfebCorr         );
        new_chipCorr          ->push_back(vvv_new_chipCorr         );               

    } // end loop over muons

    if (nGoodMus_ == 0) return;

    //
    // put variables into event
    //
    iEvent.put(run                         , "run"                         );
    iEvent.put(lumi                        , "lumi"                        );
    iEvent.put(evt                         , "evt"                         );
    iEvent.put(bfield                      , "bfield"                      );
    iEvent.put(csc_status                  , "cscstatus"                   );
    iEvent.put(detector_status             , "detectorstatus"              );
    iEvent.put(endcap                      , "endcap"                      );
    iEvent.put(station                     , "station"                     );
    iEvent.put(ring                        , "ring"                        );
    iEvent.put(chamber                     , "chamber"                     );
    iEvent.put(layer                       , "layer"                       );
    iEvent.put(rhtime                      , "rhtime"                      );
    iEvent.put(nstrips                     , "nstrips"                     );
    iEvent.put(is_global                   , "isglobal"                    );
    iEvent.put(is_tracker                  , "istracker"                   );
    iEvent.put(is_sa                       , "issa"                        );
    iEvent.put(is_pf                       , "ispf"                        );
    iEvent.put(p4                          , "p4"                          );
    iEvent.put(dz                          , "dz"                          );
    iEvent.put(d0                          , "d0"                          );
    iEvent.put(dz_bs                       , "dzbs"                        );
    iEvent.put(d0_bs                       , "d0bs"                        );
    iEvent.put(gfit_chi2                   , "gfitchi2"                    );
    iEvent.put(gfit_ndof                   , "gfitndof"                    );
    iEvent.put(maxOpeningAngleTrack        , "maxOpeningAngleTrack"        );
    iEvent.put(maxOpeningAngleMuon         , "maxOpeningAngleMuon"         );
    iEvent.put(type                        , "type"                        );
    iEvent.put(valid_siLayers              , "validsiLayers"               );
    iEvent.put(valid_pixelHits             , "validpixelHits"              );
    iEvent.put(charge                      , "charge"                      );
    iEvent.put(trk_charge                  , "trkcharge"                   );
    iEvent.put(gfit_validSTAhits           , "gfitvalidSTAhits"            );
    iEvent.put(gfit_validSiHits            , "gfitvalidSiHits"             );
    iEvent.put(sta_validHits               , "stavalidHits"                );
    iEvent.put(numMatchedStations          , "numMatchedStations"          );
    iEvent.put(numMatchedCSCsegments       , "numMatchedCSCsegments"       );
    iEvent.put(station_mask                , "stationmask"                 );
    iEvent.put(distToChamberEdge           , "distToChamberEdge"           );
    iEvent.put(distToChamberEdgeErr        , "distToChamberEdgeErr"        );
    iEvent.put(numCSCsegmentsInChamber     , "numCSCsegmentsInChamber"     );
    iEvent.put(arbitration_mask            , "arbitrationmask"             );
    iEvent.put(isSegmentAndTrackArbitrated , "isSegmentAndTrackArbitrated" );
    iEvent.put(numRecHitsInSegment         , "numRecHitsInSegment"         );
    iEvent.put(chipCorr                    , "chipCorr"                    );
    iEvent.put(cfebCorr                    , "cfebCorr"                    );
    iEvent.put(skewClearDelay              , "skewClearDelay"              );
    iEvent.put(cfebCableDelay              , "cfebCableDelay"              );
    iEvent.put(chamberCorr                 , "chamberCorr"                 );
    iEvent.put(cfeb_cable_length           , "cfebcablelength"             );
    iEvent.put(cfeb_cable_rev              , "cfebcablerev"                );
    iEvent.put(cfeb_skew_delay             , "cfebskewdelay"               );
    iEvent.put(cfeb_timing_corr            , "cfebtimingcorr"              );
    iEvent.put(cfeb_cable_delay            , "cfebcabledelay"              );
    iEvent.put(precision                   , "precision"                   );
    iEvent.put(new_skewClearDelay          , "newskewClearDelay"           );
    iEvent.put(new_cfebCableDelay          , "newcfebCableDelay"           );
    iEvent.put(new_chamberCorr             , "newchamberCorr"              );
    iEvent.put(new_cfeb_cable_length       , "newcfebcablelength"          );
    iEvent.put(new_cfeb_cable_rev          , "newcfebcablerev"             );
    iEvent.put(new_cfeb_skew_delay         , "newcfebskewdelay"            );
    iEvent.put(new_cfeb_cable_delay        , "newcfebcabledelay"           );
    iEvent.put(new_cfebCorr                , "newcfebCorr"                 );
    iEvent.put(new_chipCorr                , "newchipCorr"                 );
    iEvent.put(twire                       , "twire"                       );
    iEvent.put(anode_bx_offset             , "anodebxoffset"               );
}

//
// implement good vertex selection
//
bool CSCTimingBabyMaker::isGoodVertex (const reco::Vertex& vertex)
{
    if (vertex.isFake()) return false;
    if (vertex.ndof() < 4) return false;
    if (vertex.position().Rho() > 2) return false;
    if (fabs(vertex.position().Z()) > 24) return false;
    return true;
}

//
// return PPIB_ODMB_CABLE_LENGTH in [cm]
//
int CSCTimingBabyMaker::getSkewClearCableLengthForME11 (const CSCDetId &id)
{
    int station = id.station();
    int chamber = id.chamber();
    int endcap = id.endcap();
    
    assert (station == 1);
    assert(chamber >= 1 && chamber <=36);
    assert (endcap == 1 || endcap == 2);
    
    return (endcap == 1) ? CSCTimingBabyMaker::ME11P_SKEWCLEAR_CABLE_LENGTHS[chamber-1] : CSCTimingBabyMaker::ME11M_SKEWCLEAR_CABLE_LENGTHS[chamber-1];
}

//
// return total delay (PPIB_ODMB + PPIB_DCFEB + PPIB_LVDS) in [ns]
//
int CSCTimingBabyMaker::getSkewClearCableDelayForME11 (const CSCDetId &id)
{
    int cable_length = CSCTimingBabyMaker::getSkewClearCableLengthForME11(id);
    int ppib_odmb_delay = cable_length * CSCTimingBabyMaker::SKEWCLEAR_REVD_PROPAGATION_DELAY;
    int ppib_dcfeb_delay = CSCTimingBabyMaker::PPIB_DCFEB_CABLE_LENGTH * CSCTimingBabyMaker::SKEWCLEAR_REVO_PROPAGATION_DELAY;
    const int precision = 100;
    return int(ppib_odmb_delay + ppib_dcfeb_delay + CSCTimingBabyMaker::PPIB_LVDS_DELAY*precision);
}


// ------------ method called once each job just before starting event loop  ------------
void 
CSCTimingBabyMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CSCTimingBabyMaker::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
  void
  CSCTimingBabyMaker::beginRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
  void
  CSCTimingBabyMaker::endRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
  void
  CSCTimingBabyMaker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
  void
  CSCTimingBabyMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CSCTimingBabyMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCTimingBabyMaker);
