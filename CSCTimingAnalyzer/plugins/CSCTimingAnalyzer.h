// -*- C++ -*-
//
// Package:    CSCOfflineTiming/CSCTimingAnalyzer
// Class:      CSCTimingAnalyzer
// 
/**\class CSCTimingAnalyzer CSCTimingAnalyzer.cc CSCOfflineTiming/CSCTimingAnalyzer/plugins/CSCTimingAnalyzer.cc

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
#include <set>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "RecoLocalMuon/CSCValidation/src/CSCValHists.h"
#include "CSCValHists.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "TFile.h"
#include "TTree.h"
#include "TBits.h"
#include "TString.h"

typedef math::XYZTLorentzVectorD LorentzVector;

//
// class declaration
//
class CSCDetId;

class CSCTimingAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit CSCTimingAnalyzer(const edm::ParameterSet&);
  ~CSCTimingAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void loadBranches (const edm::Event&);
  void getVars (size_t muon_index = 9999, size_t chamber_index = 9999, size_t segment_index = 9999, size_t rechit_index = 9999);
  void initVars ();
  void fillMaps (CSCDetId id);
  void fillHistosFromMaps ();
  void fill1DHistFromMapByType (std::map<CSCDetId, double>& m, std::string name, std::string title,
                                int binsx, float xmin, float xmax, std::string folder);
  void fill1DHistFromMapByType (std::map<CSCDetId, int>& m, std::string name, std::string title,
                                int binsx, float xmin, float xmax, std::string folder);
  void fill1DHistFromMapByType (std::map<CSCDetId, std::set<double> >& m, std::string name, std::string title,
                                int binsx, float xmin, float xmax, std::string folder);
  void fill1DHistFromMapByChamber (std::map<CSCDetId, double>& m, std::string name, std::string title,
                                   int binsx, float xmin, float xmax, std::string folder);
  void fill1DHistFromMapByChamber (std::map<CSCDetId, int>& m, std::string name, std::string title,
                                   int binsx, float xmin, float xmax, std::string folder);
  void fill1DHistFromMapByChamber (std::map<CSCDetId, std::set<double> >& m, std::string name, std::string title,
                                   int binsx, float xmin, float xmax, std::string folder);
  void fill1DdiffHistFromMapsByType (std::map<CSCDetId, double>& m1, std::map<CSCDetId, double>& m2, std::string name, std::string title,
                                     int binsx, float xmin, float xmax, std::string folder);
  void fill1DdiffHistFromMapsByType (std::map<CSCDetId, int>& m1, std::map<CSCDetId, int>& m2, std::string name, std::string title,
                                     int binsx, float xmin, float xmax, std::string folder);
  static bool compareMapValuesD (const std::pair<CSCDetId, double>& left, const std::pair<CSCDetId, double>& right) {return (left.second < right.second);}
  static bool compareMapValuesI (const std::pair<CSCDetId, int>& left, const std::pair<CSCDetId, int>& right) {return (left.second < right.second);}
  static bool compareMapValuesSD (const std::pair<CSCDetId, std::set<double> >& left, const std::pair<CSCDetId, std::set<double> >& right) {return ((*left.second.rbegin()) < (*right.second.rbegin()));}
  double getMinHistValFromMap (const std::map<CSCDetId, double>& m);
  double getMaxHistValFromMap (const std::map<CSCDetId, double>& m);
  int getMinHistValFromMap (const std::map<CSCDetId, int>& m);    
  int getMaxHistValFromMap (const std::map<CSCDetId, int>& m);    
  double getMinHistValFromMap (const std::map<CSCDetId, std::set<double> >& m);
  double getMaxHistValFromMap (const std::map<CSCDetId, std::set<double> >& m);
  void printMapToFile (const std::map<CSCDetId, double>& m, std::string filename);
  void printMapToFile (const std::map<CSCDetId, int>& m, std::string filename);
  void printMapToFile (const std::map<CSCDetId, std::set<double> >& m, std::string filename);    
  void setTimingParamBabyBranches (TTree* tree);
  void setTimingParamValues (const CSCDetId& id);
  bool isPOGmuonTight ();
  bool readHeuristicCorrectionsFromFile ();
  void setTimingStudyBabyBranches (TTree* tree);
  double calculateSegmentTime (std::vector<double>& rhts, std::vector<double>& wts);
    
  // outfile where histograms are written
  TFile *outfile_;

  // histogram container and tools
  CSCValHists *histos_;
    
  void fillNm1hists ();

  bool isGlobalMuon () { return is_global; }
  bool passesGlobalChi2 ();
  bool passesGlobalSAhits ();
  bool passesNumStations ();
  bool passesD0 ();
  bool passesDZ ();
  bool passesPixelHits ();
  bool passesSiLayers ();     

  // toggle ME1/1 timing corrections
  bool makeChamberTimingCorrectionValueHists_;
  bool makePlotsPerChamber_;
  bool makePlotsPerLayer_;
  bool printTimeCorrectionParametersToFile_;
  bool writeTimeCorrectionParametersToTree_;
  bool verbose_;
  bool debug_;
  bool applyUpdatedME11corrections_;
  bool checkCSCstatus_;
  bool checkDCSstatus_;
  bool removeHeuristicCorrection_;
  bool applyNewHeuristicCorrectionByRing_;
  bool applyNewHeuristicCorrectionByChamber_;
  bool makeNminus1hists_;
  bool writeTimingStudyBabyTree_;
  bool removeMatchedStationsRequirement_;
  bool combineME11ab_;
  bool requireInsideOutMuons_;
  int min_run_;
  int max_run_;
  double min_pt_;
  double max_dz_;
    
  std::string outfileName_;
  std::string timeParamFileName_;
  std::string fileForHeuristicCorrByRing_;
  std::string fileForHeuristicCorrByChamber_;
  std::string timingStudyBabyFileName_;

  edm::FileInPath fpForHeuristicCorrByRing_;
  edm::FileInPath fpForHeuristicCorrByChamber_;    

  //
  // tokens, grumble
  //
  edm::EDGetTokenT<int> run_token ;
  edm::EDGetTokenT<int> lumi_token;
  edm::EDGetTokenT<unsigned long long> evt_token;
  edm::EDGetTokenT<double> bfield_token;
  edm::EDGetTokenT<bool> csc_status_token;
  edm::EDGetTokenT<unsigned int> detector_status_token;
  edm::EDGetTokenT<TBits> hlt_bits_token;
  edm::EDGetTokenT<std::vector<TString> > hlt_trigNames_token;
  edm::EDGetTokenT<std::vector<unsigned int> > hlt_prescales_token;
  edm::EDGetTokenT<std::vector<bool> > is_global_token ;
  edm::EDGetTokenT<std::vector<bool> > is_tracker_token;
  edm::EDGetTokenT<std::vector<bool> > is_sa_token     ;
  edm::EDGetTokenT<std::vector<bool> > is_pf_token     ;
  edm::EDGetTokenT<std::vector<LorentzVector> > p4_token;
  edm::EDGetTokenT<std::vector<double> > dz_token;
  edm::EDGetTokenT<std::vector<double> > d0_token;
  edm::EDGetTokenT<std::vector<double> > dz_bs_token;
  edm::EDGetTokenT<std::vector<double> > d0_bs_token;
  edm::EDGetTokenT<std::vector<double> > gfit_chi2_token;
  edm::EDGetTokenT<std::vector<double> > gfit_ndof_token;
  edm::EDGetTokenT<std::vector<double> > maxOpeningAngleTrack_token;
  edm::EDGetTokenT<std::vector<double> > maxOpeningAngleMuon_token;
  edm::EDGetTokenT<std::vector<double> > time_token;
  edm::EDGetTokenT<std::vector<double> > timeErr_token;
  edm::EDGetTokenT<std::vector<int> > type_token;
  edm::EDGetTokenT<std::vector<int> > valid_siLayers_token;
  edm::EDGetTokenT<std::vector<int> > valid_pixelHits_token;
  edm::EDGetTokenT<std::vector<int> > charge_token;
  edm::EDGetTokenT<std::vector<int> > trk_charge_token;
  edm::EDGetTokenT<std::vector<int> > gfit_validSTAhits_token;
  edm::EDGetTokenT<std::vector<int> > gfit_validSiHits_token;
  edm::EDGetTokenT<std::vector<int> > sta_validHits_token;
  edm::EDGetTokenT<std::vector<int> > numMatchedStations_token;
  edm::EDGetTokenT<std::vector<int> > numMatchedCSCsegments_token;
  edm::EDGetTokenT<std::vector<int> > station_mask_token;
  edm::EDGetTokenT<std::vector<int> > direction_token;
  edm::EDGetTokenT<std::vector<std::vector<LorentzVector> > > hlt_trigObjs_p4_token;
  edm::EDGetTokenT<std::vector<std::vector<double> > > distToChamberEdge_token;
  edm::EDGetTokenT<std::vector<std::vector<double> > > distToChamberEdgeErr_token;
  edm::EDGetTokenT<std::vector<std::vector<int> > > numCSCsegmentsInChamber_token;
  edm::EDGetTokenT<std::vector<std::vector<int> > > hlt_trigObjs_id_token;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<unsigned int> > > > arbitration_mask_token;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<bool> > > > isSegmentAndTrackArbitrated_token;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<int> > > > numRecHitsInSegment_token;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<double> > > > segmentTime_token;    
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    endcap_token               ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    station_token              ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    ring_token                 ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    chamber_token              ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    layer_token                ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > rhtime_token               ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > twire_token                ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    nstrips_token              ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_cable_length_token    ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_skew_delay_token      ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_timing_corr_token     ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_cable_delay_token     ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    precision_token            ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<char> > > > >   cfeb_cable_rev_token       ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > anode_bx_offset_token      ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > chipCorr_token             ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > cfebCorr_token             ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > skewClearDelay_token       ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > cfebCableDelay_token       ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > chamberCorr_token          ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > new_skewClearDelay_token   ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > new_cfebCableDelay_token   ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > new_chamberCorr_token      ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_cable_length_token;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<char> > > > >   new_cfeb_cable_rev_token   ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_skew_delay_token  ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_cable_delay_token ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > new_cfebCorr_token         ;
  edm::EDGetTokenT<std::vector<std::vector<std::vector<std::vector<double> > > > > new_chipCorr_token         ;
  
  //
  // event level quantities
  //
  edm::Handle<int> run_h  ;
  edm::Handle<int> lumi_h ;
  edm::Handle<unsigned long long> evt_h ;
  edm::Handle<double> bfield_h ;
  edm::Handle<bool> csc_status_h ;
  edm::Handle<unsigned int> detector_status_h ;
  edm::Handle<TBits> hlt_bits_h;
  edm::Handle<std::vector<TString> > hlt_trigNames_h;
  edm::Handle<std::vector<unsigned int> > hlt_prescales_h;
  edm::Handle<std::vector<bool> > is_global_h  ;
  edm::Handle<std::vector<bool> > is_tracker_h ;
  edm::Handle<std::vector<bool> > is_sa_h      ;
  edm::Handle<std::vector<bool> > is_pf_h      ;
  edm::Handle<std::vector<LorentzVector> > p4_h ;
  edm::Handle<std::vector<double> > dz_h ;
  edm::Handle<std::vector<double> > d0_h ;
  edm::Handle<std::vector<double> > dz_bs_h ;
  edm::Handle<std::vector<double> > d0_bs_h ;
  edm::Handle<std::vector<double> > gfit_chi2_h ;
  edm::Handle<std::vector<double> > gfit_ndof_h ;
  edm::Handle<std::vector<double> > maxOpeningAngleTrack_h ;
  edm::Handle<std::vector<double> > maxOpeningAngleMuon_h ;
  edm::Handle<std::vector<double> > time_h;
  edm::Handle<std::vector<double> > timeErr_h;
  edm::Handle<std::vector<int> > type_h ;
  edm::Handle<std::vector<int> > valid_siLayers_h ;
  edm::Handle<std::vector<int> > valid_pixelHits_h ;
  edm::Handle<std::vector<int> > charge_h ;
  edm::Handle<std::vector<int> > trk_charge_h ;
  edm::Handle<std::vector<int> > gfit_validSTAhits_h ;
  edm::Handle<std::vector<int> > gfit_validSiHits_h ;
  edm::Handle<std::vector<int> > sta_validHits_h ;
  edm::Handle<std::vector<int> > numMatchedStations_h ;
  edm::Handle<std::vector<int> > numMatchedCSCsegments_h ;
  edm::Handle<std::vector<int> > station_mask_h ;
  edm::Handle<std::vector<int> > direction_h;
  edm::Handle<std::vector<std::vector<LorentzVector> > > hlt_trigObjs_p4_h;
  edm::Handle<std::vector<std::vector<double> > > distToChamberEdge_h ;
  edm::Handle<std::vector<std::vector<double> > > distToChamberEdgeErr_h ;
  edm::Handle<std::vector<std::vector<int> > > numCSCsegmentsInChamber_h ;
  edm::Handle<std::vector<std::vector<int> > > hlt_trigObjs_id_h;
  edm::Handle<std::vector<std::vector<std::vector<unsigned int> > > > arbitration_mask_h ;
  edm::Handle<std::vector<std::vector<std::vector<bool> > > > isSegmentAndTrackArbitrated_h ;
  edm::Handle<std::vector<std::vector<std::vector<int> > > > numRecHitsInSegment_h ;
  edm::Handle<std::vector<std::vector<std::vector<double> > > > segmentTime_h ;    
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    endcap_h                ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    station_h               ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    ring_h                  ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    chamber_h               ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    layer_h                 ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > rhtime_h                ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > twire_h                 ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    nstrips_h               ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_cable_length_h     ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_skew_delay_h       ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_timing_corr_h      ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    cfeb_cable_delay_h      ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    precision_h             ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<char> > > > >   cfeb_cable_rev_h        ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > anode_bx_offset_h       ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > chipCorr_h              ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > cfebCorr_h              ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > skewClearDelay_h        ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > cfebCableDelay_h        ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > chamberCorr_h           ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > new_skewClearDelay_h    ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > new_cfebCableDelay_h    ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > new_chamberCorr_h       ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_cable_length_h ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<char> > > > >   new_cfeb_cable_rev_h    ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_skew_delay_h   ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<int> > > > >    new_cfeb_cable_delay_h  ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > new_cfebCorr_h          ;
  edm::Handle<std::vector<std::vector<std::vector<std::vector<double> > > > > new_chipCorr_h          ;

  int run  ;
  int lumi ;
  unsigned long long evt ;
  double bfield ;
  bool csc_status ;
  unsigned int detector_status ;
  TBits hlt_bits;
  std::vector<TString> hlt_trigNames;
  std::vector<unsigned int> hlt_prescales;
  std::vector<std::vector<int> > hlt_trigObjs_id;
  std::vector<std::vector<LorentzVector> > hlt_trigObjs_p4;
  bool  is_global  ;
  bool  is_tracker ;
  bool  is_sa      ;
  bool  is_pf      ;
  LorentzVector  p4 ;
  double  dz ;
  double  d0 ;
  double  dz_bs ;
  double  d0_bs ;
  double  gfit_chi2 ;
  double  gfit_ndof ;
  double  maxOpeningAngleTrack ;
  double  maxOpeningAngleMuon ;
  double  time ;
  double  timeErr ;
  int  type ;
  int  valid_siLayers ;
  int  valid_pixelHits ;
  int  charge ;
  int  trk_charge ;
  int  gfit_validSTAhits ;
  int  gfit_validSiHits ;
  int  sta_validHits ;
  int  numMatchedStations ;
  int  numMatchedCSCsegments ;
  int  station_mask ;
  int  direction ;
  double   distToChamberEdge ;
  double   distToChamberEdgeErr ;
  int   numCSCsegmentsInChamber ;
  unsigned int    arbitration_mask ;
  bool    isSegmentAndTrackArbitrated ;
  int    numRecHitsInSegment ;
  double segmentTime;
  int        endcap                ;
  int        station               ;
  int        ring                  ;
  int        chamber               ;
  int        layer                 ;
  double     rhtime                ;
  double     twire                 ;
  int        nstrips               ;
  int        cfeb_cable_length     ;
  int        cfeb_skew_delay       ;
  int        cfeb_timing_corr      ;
  int        cfeb_cable_delay      ;
  int        precision             ;
  char       cfeb_cable_rev        ;
  double     anode_bx_offset       ;
  double     chipCorr              ;
  double     cfebCorr              ;
  double     skewClearDelay        ;
  double     cfebCableDelay        ;
  double     chamberCorr           ;
  double     new_skewClearDelay    ;
  double     new_cfebCableDelay    ;
  double     new_chamberCorr       ;
  int        new_cfeb_cable_length ;
  char       new_cfeb_cable_rev    ;
  int        new_cfeb_skew_delay   ;
  int        new_cfeb_cable_delay  ;
  double     new_cfebCorr          ;
  double     new_chipCorr          ;

  int        b_endcap                ;
  int        b_station               ;
  int        b_ring                  ;
  int        b_chamber               ;
  int        b_cfeb_cable_length     ;
  int        b_cfeb_skew_delay       ;
  int        b_anode_bx_offset       ;
  int        b_cfeb_cable_delay      ;
  double     b_cfebCorr              ;
  double     b_skewClearDelay        ;
  double     b_cfebCableDelay        ;
  double     b_chamberCorr           ;
  double     b_new_skewClearDelay    ;
  double     b_new_cfebCableDelay    ;
  double     b_new_chamberCorr       ;
  int        b_new_cfeb_cable_length ;
  int        b_new_cfeb_skew_delay   ;
  int        b_new_cfeb_cable_delay  ;
  double     b_new_cfebCorr          ;
  double     b_new_heuristicCorr     ;
  char       b_cfeb_cable_rev        ;
  char       b_new_cfeb_cable_rev    ;    
    
  double     b_rhtime;

  std::vector<double> b_chipCorr;
  std::vector<double> b_new_chipCorr;
        
  std::map<CSCDetId, double> m_cfebCableDelay;
  std::map<CSCDetId, double> m_cfebCorr;
  std::map<CSCDetId, double> m_chamberCorr;
  std::map<CSCDetId, double> m_skewClearDelay;
  std::map<CSCDetId, int>    m_cfeb_cable_delay;
  std::map<CSCDetId, int>    m_cfeb_cable_length;
  std::map<CSCDetId, int>    m_cfeb_skew_delay;
  std::map<CSCDetId, int>    m_anode_bx_offset;
  std::map<CSCDetId, char>   m_cfeb_cable_rev;
    
  std::map<CSCDetId, double> m_new_cfebCableDelay;
  std::map<CSCDetId, double> m_new_cfebCorr;
  std::map<CSCDetId, double> m_new_chamberCorr;
  std::map<CSCDetId, double> m_new_skewClearDelay;
  std::map<CSCDetId, int>    m_new_cfeb_cable_delay;
  std::map<CSCDetId, int>    m_new_cfeb_cable_length;
  std::map<CSCDetId, int>    m_new_cfeb_skew_delay;
  std::map<CSCDetId, char>   m_new_cfeb_cable_rev;
    
  std::map<CSCDetId, std::set<double> > m_chipCorr;
  std::map<CSCDetId, std::set<double> > m_new_chipCorr;

  std::map<CSCDetId, double> m_new_heuristicCorr;

  TFile *babyFile;
  TTree *babyTree;

  static const double ME11_AVG_HEURISTIC_CORR;
  static const double NON_ME11_AVG_HEURISTIC_CORR;
};
